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
