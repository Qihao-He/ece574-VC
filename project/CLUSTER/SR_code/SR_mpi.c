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
#define truePIvalue 3.1415926535897932384626433832795028841971693993751058209749445923

double f(int i){
	double x;
	x=(double)i/(double)n;
	return 4.0/(1.0+x*x);
}

int main(int argc, char *argv[]) {

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

/* Reading the image from all nodes rather than just rank 0 */
		load_jpeg(argv[1],&image);

		load_time=MPI_Wtime();

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
