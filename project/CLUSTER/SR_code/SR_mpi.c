/* By Qihao He <qi.he@maine.edu> */
/* This program uses Simpson's Rule to compute PI */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>
#include <mpi.h>

#define n 1e8 //iteration times
#define epsilon 2.220446e-16//epsilon

/* True value of Pi from Wolfwww.wolframalpha.com */
#define truepivalue 3.1415926535897932384626433832795028841971693993751058209749445923

double f(int i){
	double x;
	x=(double)i/(double)n;
	return 4.0/(1.0+x*x);
}

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
	return NULL;
}

int main(int argc, char *argv[]) {

	double area;
	double i;
	double Rerror;//Relative error
	double error;
	//count
	//receive buffer, send buffer

	double start_time,load_time=0,store_time,convolve_time=0,combine_time;

	int numtasks,rank;//# of tasks, rank index

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

	MPI_Bcast(image.pixels,	/* buffer */
		arraysize_image,			/* count */
		MPI_CHAR,							/* type */
		0,										/* root source */
		MPI_COMM_WORLD);

		/* kernel */
// rank*image.y/numtasks;//rank*(size_img)
// (rank+1)*image.y/numtasks;//(rank+1)*(size_img)/numtasks





/* Use MPI_Gather() to gather results in rank 0 */
MPI_Gather(sobel_y.pixels,	/* send buffer */
	arraysize_image/numtasks,	/* count */
	MPI_CHAR,									/* type */
	gather_sobel_y,						/* receive buffer */
	arraysize_image/numtasks,	/* count */
	MPI_CHAR,									/* type */
	0,												/* root source */
	MPI_COMM_WORLD);

/* MPI_Finalize at the end */
MPI_Finalize();
return 0;
}
