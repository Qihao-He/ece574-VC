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
// #define epsilon 2.220446e-16//epsilon
#define epsilon 1e-15//epsilon

/* True value of Pi from Wolfwww.wolframalpha.com */
#define truepivalue 3.14159265358979323

double SR_f(double start, double end){
	double x,y;
	double temp1,temp2;
	double area=0;
	int i;

	for(i=1;i<=n/2;i++){
		x=(double)(2*i-1)/(double)n;
		temp1= 4.0/(1.0+x*x);
		y=(double)(2*i)/(double)n;
		temp2= 4.0/(1.0+y*y);
		area+= 4.0*temp1+2*temp2;
	}
	return area;
}

int main(int argc, char *argv[]) {

	double area;
	double Rerror;//Relative error
	double error;

	//count
	//receive buffer, send buffer
	/* measure time */
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

	/* Each rank should work on part of the image ranging form ystart to yend. */
	// SR_f(start, end);//Call SR_f function
	SR_f(rank*n/numtasks, (rank+1)*n/numtasks);//Call SR_f function

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
