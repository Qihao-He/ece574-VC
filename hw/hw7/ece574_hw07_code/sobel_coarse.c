/* Example sobel code for ECE574 -- Fall 2015 */
/* By Vince Weaver <vincent.weaver@maine.edu> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#include <jpeglib.h>

#include <mpi.h>
#define ARRAYSIZE 3//image.x, image.y,image.depth are the 3 element of the array

/* Filters */
static int sobel_x_filter[3][3]={{-1,0,+1},{-2,0,+2},{-1,0,+1}};
static int sobel_y_filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,+1}};

/* Structure describing the image */
struct image_t {
	int x;
	int y;
	int depth;	/* bytes */
	unsigned char *pixels;
};

struct convolve_data_t {
	struct image_t *old;
	struct image_t *new;
	int (*filter)[3][3];
	int ystart;
	int yend;
};

/* Modify generic_convolve so it takes a range of y to operate on. Then
calculate this y range based on the rank and size parameters. MPI_Gather will
gather from the *start* of each buffer and put it in the proper place in the
results. So you hanve to modify the convolve routine to store the output
starting at offset 0, rather than at offset ystart. */

/* very inefficient convolve code */
static void *generic_convolve(void *argument) {

	int x,y,k,l,d;
	uint32_t color;
	int sum,depth,width;

	struct image_t *old;
	struct image_t *new;
	int (*filter)[3][3];
	struct convolve_data_t *data;
	int ystart, yend;

	/* Convert from void pointer to the actual data type */
	data=(struct convolve_data_t *)argument;
	old=data->old;
	new=data->new;
	filter=data->filter;

	ystart=data->ystart;
	yend=data->yend;

	depth=old->depth;
	width=old->x*old->depth;

	if (ystart==0) ystart=1;// edge problem
	if (yend==old->y) yend=old->y-1;

	for(d=0;d<3;d++) {
	   for(x=1;x<old->x-1;x++) {
	     for(y=ystart;y<yend;y++) {
		sum=0;
		for(k=-1;k<2;k++) {
		   for(l=-1;l<2;l++) {
			color=old->pixels[((y+l)*width)+(x*depth+d+k*depth)];
			sum+=color * (*filter)[k+1][l+1];
		   }
		}

		if (sum<0) sum=0;
		if (sum>255) sum=255;

		new->pixels[(y*width)+x*depth+d]=sum;

	     }
	   }
	}
	/* Moving the range from ystart to yend to offset 0. */
	for(y=ystart;y<yend;y++){
		new->pixels[((y-ystart+1)*width)+x*depth+d]=
		new->pixels[(y*width)+x*depth+d];
	}
	return NULL;
}


static int load_jpeg(char *filename, struct image_t *image) {

	FILE *fff;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW output_data;
	unsigned int scanline_len;
	int scanline_count=0;

	fff=fopen(filename,"rb");
	if (fff==NULL) {
		fprintf(stderr, "Could not load %s: %s\n",
			filename, strerror(errno));
		return -1;
	}

	/* set up jpeg error routines */
	cinfo.err = jpeg_std_error(&jerr);

	/* Initialize cinfo */
	jpeg_create_decompress(&cinfo);

	/* Set input file */
	jpeg_stdio_src(&cinfo, fff);

	/* read header */
	jpeg_read_header(&cinfo, TRUE);

	/* Start decompressor */
	jpeg_start_decompress(&cinfo);

	printf("output_width=%d, output_height=%d, output_components=%d\n",
		cinfo.output_width,
		cinfo.output_height,
		cinfo.output_components);

	image->x=cinfo.output_width;
	image->y=cinfo.output_height;
	image->depth=cinfo.output_components;

	scanline_len = cinfo.output_width * cinfo.output_components;
	image->pixels=malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);

	while (scanline_count < cinfo.output_height) {
		output_data = (image->pixels + (scanline_count * scanline_len));
		jpeg_read_scanlines(&cinfo, &output_data, 1);
		scanline_count++;
	}

	/* Finish decompressing */
	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fclose(fff);

	return 0;
}

static int store_jpeg(char *filename, struct image_t *image) {

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int quality=90; /* % */
	int i;

	FILE *fff;

	JSAMPROW row_pointer[1];
	int row_stride;

	/* setup error handler */
	cinfo.err = jpeg_std_error(&jerr);

	/* initialize jpeg compression object */
	jpeg_create_compress(&cinfo);

	/* Open file */
	fff = fopen(filename, "wb");
	if (fff==NULL) {
		fprintf(stderr, "can't open %s: %s\n",
			filename,strerror(errno));
		return -1;
	}

	jpeg_stdio_dest(&cinfo, fff);

	/* Set compression parameters */
	cinfo.image_width = image->x;
	cinfo.image_height = image->y;
	cinfo.input_components = image->depth;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	/* start compressing */
	jpeg_start_compress(&cinfo, TRUE);

	row_stride=image->x*image->depth;

	for(i=0;i<image->y;i++) {
		row_pointer[0] = & image->pixels[i * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* finish compressing */
	jpeg_finish_compress(&cinfo);

	/* close file */
	fclose(fff);

	/* clean up */
	jpeg_destroy_compress(&cinfo);

	return 0;
}

static int combine(struct image_t *s_x,
			struct image_t *s_y,
			struct image_t *new) {
	int i;
	int out;

	for(i=0;i<( s_x->depth * s_x->x * s_x->y );i++) {

		out=sqrt(
			(s_x->pixels[i]*s_x->pixels[i])+
			(s_y->pixels[i]*s_y->pixels[i])
			);
		if (out>255) out=255;
		if (out<0) out=0;
		new->pixels[i]=out;
	}

	return 0;
}

int main(int argc, char **argv) {

	struct image_t image,sobel_x,sobel_y,new_image;
	struct convolve_data_t sobel_data[2];
	double start_time,load_time=0,store_time,convolve_time=0,combine_time;
	int result,i;
	int A[ARRAYSIZE]; //A Buffer
	int numtasks,rank;//# of tasks, rank index
	MPI_Status Stat;
	unsigned long int arraysize_image; //arraysize of the image
	int *gather_sobel_x=NULL;//receive buffer for soble_x
	int *gather_sobel_y=NULL;//receive buffer for soble_y

	/* Check command line usage */
	if (argc<2) {
		fprintf(stderr,"Usage: %s image_file\n",argv[0]);
		return -1;
	}

	/* Initialize MPI */
	result = MPI_Init(&argc,&argv);
	if (result != MPI_SUCCESS) {
		printf ("Error starting MPI program!.\n");
		MPI_Abort(MPI_COMM_WORLD, result);
	}

	/* Calls MPI_wtime() to get the wallclock times for Load, Convolve, Combine,
	and Store like we did with PAPI in the OpenMP code. You only need to record
	and print these from rank 0. */
	start_time=MPI_Wtime();

	/* Get number of tasks and our process number (rank) */
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);//number os tasks
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);//process number(rank)

	printf("R%d: Number of tasks= %d My rank= %d\n",
		rank,numtasks,rank);

	/* Only load the jpeg in rank 0 */
	if (rank==0) {
		/* Initialize Array */
		printf("R0: Initializing array\n");
		/* Load an image */
		load_jpeg(argv[1],&image);

		/* Send the image parameters (image.x, image.y, image.depth) to all other
		ranks, sending an array of 3 INTS */
		A[0]= image.x;
		A[1]= image.y;
		A[2]= image.depth;

		for(i=1;i<numtasks;i++) {
			printf("R0: Sending %d ints to Rank%d\n",
				ARRAYSIZE,i);
			result = MPI_Send(A,/* buffer */
					ARRAYSIZE,			/* count */
					MPI_INT,				/* type */
					i,							/* destination */
					13,							/* tag */
					MPI_COMM_WORLD);

			/* Malloc image.pixels input image in the non rank-0 threads */
			printf("MALLOC R%d\n",i);
			image.pixels=malloc(image.x*image.y*image.depth*sizeof(char));
		}
		load_time=MPI_Wtime();

		/* In Rank 0, get the size of the image */
		arraysize_image=image.x*image.y*image.depth*sizeof(char);

		gather_sobel_x=malloc(arraysize_image);//dynamically allocate Memory
		gather_sobel_y=malloc(arraysize_image);//dynamically allocate Memory
	}
	else {
		/* MPI_Recv is required for other processes */
		/* This is run by all the non-master processes */
		result = MPI_Recv(A,/* buffer */
				ARRAYSIZE,			/* count */
				MPI_INT,				/* type */
				0,							/* source */
				13,							/* tag */
				MPI_COMM_WORLD,	/* communicator */
				&Stat);					/* status */

		arraysize_image=image.x*image.y*image.depth*sizeof(char);
	}

	/* other processes also has the arraysize_image */
	printf("R%d buffer A[0] is %d, A[1] is %d, A[2] is %d\n",rank,A[0],A[1],A[2]);

	/* debug for negative count */
	printf("2nd R%d The arraysize_image is:%lu\n",rank,arraysize_image);

/* BUG: R0 AND R1 HAVE DIFFERENT arraysize_image:

$ cat slurm.coarse.haswell-ep.1053.err

real	0m0.253s
user	0m0.004s
sys	0m0.012s
ece574-10@haswell-ep:~/QH_repository/ece574-VC/hw/hw7/ece574_hw07_code$ cat slurm.coarse.haswell-ep.1053.out
R0: Number of tasks= 2 My rank= 0
R0: Initializing array
R1: Number of tasks= 2 My rank= 1
output_width=3888, output_height=2592, output_components=3
R0: Sending 3 ints to Rank1
MALLOC R1
1st R0 The arraysize_image is:30233088
2nd R0 The arraysize_image is:30233088
R0 Report error 0
2nd R1 The arraysize_image is:1410377472
R1 Report error 0

===================================================================================
=   BAD TERMINATION OF ONE OF YOUR APPLICATION PROCESSES
=   PID 22414 RUNNING AT haswell-ep
=   EXIT CODE: 11
=   CLEANING UP REMAINING PROCESSES
=   YOU CAN IGNORE THE BELOW CLEANUP MESSAGES
===================================================================================
YOUR APPLICATION TERMINATED WITH THE EXIT STRING: Segmentation fault (signal 11)
This typically refers to a problem with your application.
Please see the FAQ page for debugging suggestions

*/

	/* Use MPI_Bcast() to broadcast the entire image data from rank0 to all the
	other ranks. You want to broadcast “image.pixels”, not all of image (remember,
 	MPI you can’t send structs, just arrays). */
printf("R%d Report error 0\n",rank);

	MPI_Bcast(image.pixels,	/* buffer */
		arraysize_image,			/* count */
		MPI_CHAR,							/* type */
		0,										/* root source */
		MPI_COMM_WORLD);

printf("R%d Report error 1\n",rank);

	/* Allocate space for output image */
	new_image.x=image.x;
	new_image.y=image.y;
	new_image.depth=image.depth;
	new_image.pixels=malloc(image.x*image.y*image.depth*sizeof(char));

	/* Allocate space for output image */
	sobel_x.x=image.x;
	sobel_x.y=image.y;
	sobel_x.depth=image.depth;
	sobel_x.pixels=malloc(image.x*image.y*image.depth*sizeof(char));

	/* Allocate space for output image */
	sobel_y.x=image.x;
	sobel_y.y=image.y;
	sobel_y.depth=image.depth;
	sobel_y.pixels=malloc(image.x*image.y*image.depth*sizeof(char));

	/* Calculate this y range based on the rank and size parameters. */
	/* Each rank should work on part of the image ranging form ystart to yend. */
	/* convolution */
	sobel_data[0].old=&image;
	sobel_data[0].new=&sobel_x;
	sobel_data[0].filter=&sobel_x_filter;
	sobel_data[0].ystart=rank*image.y/numtasks;//rank*(size_img)
	sobel_data[0].yend=(rank+1)*image.y/numtasks;//(rank+1)*(size_img)/numtasks
	generic_convolve((void *)&sobel_data[0]);

	sobel_data[1].old=&image;
	sobel_data[1].new=&sobel_y;
	sobel_data[1].filter=&sobel_y_filter;
	sobel_data[1].ystart=rank*image.y/numtasks;//rank*(size_img)
	sobel_data[1].yend=(rank+1)*image.y/numtasks;//(rank+1)*(size_img)/numtasks
	generic_convolve((void *)&sobel_data[1]);

/* Use MPI_Gather() to gather results in rank 0 */
MPI_Gather(sobel_x.pixels,	/* send buffer */
	arraysize_image,					/* count */
	MPI_CHAR,									/* type */
	gather_sobel_x,						/* receive buffer */
	arraysize_image,					/* count */
	MPI_CHAR,									/* type */
	0,												/* root source */
	MPI_COMM_WORLD);

MPI_Gather(sobel_y.pixels,	/* send buffer */
	arraysize_image,					/* count */
	MPI_CHAR,									/* type */
	gather_sobel_y,						/* receive buffer */
	arraysize_image,					/* count */
	MPI_CHAR,									/* type */
	0,												/* root source */
	MPI_COMM_WORLD);

if (rank==0){
	convolve_time=MPI_Wtime();

	/* On rank 0 alone, run combine to form output */
	combine(&sobel_x,&sobel_y,&new_image);
	combine_time=MPI_Wtime();

	/* On rank 0 alone, write the output to a file */
	store_jpeg("out.jpg",&new_image);
	store_time=MPI_Wtime();

	printf("Load time: %lf\n",load_time-start_time);
	printf("Convolve time: %lf\n",convolve_time-load_time);
	printf("Combine time: %lf\n",combine_time-convolve_time);
	printf("Store time: %lf\n",store_time-combine_time);
	printf("Total time = %lf\n",store_time-start_time);
}

/* MPI_Finalize at the end */
MPI_Finalize();
return 0;
}
