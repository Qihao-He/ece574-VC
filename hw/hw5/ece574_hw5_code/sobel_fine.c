/* Example sobel code for ECE574 -- Fall 2015 */
/* By Vince Weaver <vincent.weaver@maine.edu> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <jpeglib.h>

#include "papi.h"

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
	int thread_number;//thread_number
	int NUM_THREADS;

};

/* very inefficient convolve code */
static void *generic_convolve(void *argument) {

	int x,y,k,l,d;
	uint32_t color;
	int sum,depth,width;

	struct image_t *old;
	struct image_t *new;
	int (*filter)[3][3];
	int thread_number;
	int NUM_THREADS;
	struct convolve_data_t *data;


	/* Convert from void pointer to the actual data type */
	data=(struct convolve_data_t *)argument;
	old=data->old;
	new=data->new;
	filter=data->filter;
	thread_number=data->thread_number;
	NUM_THREADS=data->NUM_THREADS;

	depth=old->depth;
	width=old->x*old->depth;

	for(d=0;d<3;d++) {
		for(x=1;x<old->x-1;x++){
			for(y=old->y/NUM_THREADS*thread_number+1;y<old->y/NUM_THREADS*(thread_number+1)-1;y++) {
				//The lines between the split picture are not processed by the program
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
	struct convolve_data_t sobel_data[8];
	int result;
	int NUM_THREADS;
	pthread_t threads[NUM_THREADS];
	long int t;//tasks
	long long s=0;
	long long e=0;//timestamp start & end variables

	/* Check command line usage */
	if (argc<2) {
		fprintf(stderr,"Usage: %s image_file\n",argv[0]);
		return -1;
	}

	result=PAPI_library_init(PAPI_VER_CURRENT);//Initialize
	if (result!=PAPI_VER_CURRENT) {//if the results does not match PAPI_VER_CURRENT
		fprintf(stderr,"Warning!  PAPI error %s\n",
			PAPI_strerror(result));
	}

	/* Load an image */
	load_jpeg(argv[1],&image);

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

	/* convolution */
	
	printf("Please select NUM_THREADS: ");
	scanf("%d",&NUM_THREADS);//get the NUM_THREADS

	s=PAPI_get_real_usec();//timestamp start

	/* Create threads */
	for(t=0;t<NUM_THREADS;t++){
		sobel_data[t].old=&image;//input image index
		sobel_data[t].new=&sobel_x;//out image index
		sobel_data[t].filter=&sobel_x_filter;
		sobel_data[t].thread_number=t;//thread number
		sobel_data[t].NUM_THREADS=NUM_THREADS;//offset

		printf("Creating thread %ld\n", t);
		result=pthread_create(&threads[t],NULL,generic_convolve,(void *)&sobel_data[t]); 
		//pthread_create(thread,attr,start_routine,arg)
		if(result){
			printf("ERROR; return code from pthread_create() is %d\n",result);//ERROR if result is #
			exit(-1);
		}
	}
	
	/* Join threads */
	for(t=0;t<NUM_THREADS;t++){ //pthread join wait til all threads finish
		pthread_join(threads[t],NULL);
	}

	/* Create threads */
	for(t=0;t<NUM_THREADS;t++){
		sobel_data[t].old=&image;//input image index
		sobel_data[t].new=&sobel_y;//out image index
		sobel_data[t].filter=&sobel_y_filter;
		sobel_data[t].thread_number=t;//thread number
		sobel_data[t].NUM_THREADS=NUM_THREADS;//offset

		printf("Creating thread %ld\n", t);
		result=pthread_create(&threads[t],NULL,generic_convolve,(void *)&sobel_data[t]); 
		//pthread_create(thread,attr,start_routine,arg)
		if(result){
			printf("ERROR; return code from pthread_create() is %d\n",result);//ERROR if result is #
			exit(-1);
		}
	}
	
	/* Join threads */
	for(t=0;t<NUM_THREADS;t++){ //pthread join wait til all threads finish
		pthread_join(threads[t],NULL);
	}

	printf("Master thread exiting,validating results\n");
	e=PAPI_get_real_usec();//timestamp end
	printf("Total convolution time is:%lld\n",e-s);//timestamp substract

	/* Combine to form output */
	s=PAPI_get_real_usec();//timestamp start
	combine(&sobel_x,&sobel_y,&new_image);
	e=PAPI_get_real_usec();//timestamp end
	printf("Combine time is:%lld\n",e-s);//timestamp substract

	/* Write data back out to disk */
	s=PAPI_get_real_usec();//timestamp start
	store_jpeg("out.jpg",&new_image);
	e=PAPI_get_real_usec();//timestamp end
	printf("Store_jpeg time is:%lld\n",e-s);//timestamp substract

	PAPI_shutdown();
	pthread_exit(NULL);//pthread exit

	return 0;
}
