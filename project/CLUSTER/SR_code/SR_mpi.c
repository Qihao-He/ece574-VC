/* By Qihao He <qi.he@maine.edu> */
/* This program uses Simpson's Rule to compute PI */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>
#include <mpi.h>

#define n 1e9 //iteration times
// #define epsilon 2.220446e-16//epsilon
#define epsilon 1e-15//epsilon

/* True value of Pi from Wolframalpha www.wolframalpha.com */
#define truepivalue 3.14159265358979323

double SR_f(int rank,int start,int end){
	double x,y;
	double temp1,temp2;
	double area=0;
	int i;

	for(i=start;i<=end;i++){
		x=(double)(2*i-1)/(double)n;
		temp1= 4.0/(1.0+x*x);
		y=(double)(2*i)/(double)n;
		temp2= 4.0/(1.0+y*y);
		area+= 4.0*temp1+2*temp2;
	}
	printf("Rank%d range from %d to %d,area:%f\n",rank,start,end,area);
	return area;
}

int main(int argc, char *argv[]) {

	double sum=0;
	double total_sum=0;
	double Rerror;//Relative error
	double error;
	int result;
	double temp1,temp2;

	/* measure time */
	double start_time,convolve_start,convolve_end,reduce_start,reduce_end;
	int numtasks,rank;//# of tasks, rank index

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
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	convolve_start=MPI_Wtime();
	/* Only on rank 0 */
	if(rank==0){
		temp1=(double)0/(double)n;
		temp2=(double)n/(double)n;
		total_sum=4.0/(1+temp1*temp1)-4.0/(1+temp2*temp2);
		printf("Rank%d,total_sum:%f\n",rank,total_sum);
	}

	sum=SR_f(rank,rank*n/2/numtasks+1,(rank+1)*n/2/numtasks);
	convolve_end=MPI_Wtime();

	MPI_Barrier(MPI_COMM_WORLD);

	reduce_start=MPI_Wtime();
	/* MPI_reduce */
	MPI_Reduce(&sum,	/* send data */
		&total_sum,	/* receive data */
		1,		/* count */
		MPI_DOUBLE,	/* type */
		MPI_SUM,	/* reduce type */
		0,		/* root */
		MPI_COMM_WORLD);

	if(rank==0){
		total_sum/=(3.0*n);
	}
	reduce_end=MPI_Wtime();

	/* print result on rank 0 */
	if(rank==0){
		printf("Rank%d:\nAppr pi: %13.15f\n",rank,total_sum);
		printf("True pi: %13.15f\n",truepivalue);//true value of pi
		printf("Epsilon: %1.15f\n",epsilon);//epsilon

		/* Relative error in epsilon */
		// printf("epsilon is: %f\n",(double)epsilon);
		Rerror=abs(total_sum-(double)truepivalue)/(double)epsilon;
		error=abs(total_sum-(double)truepivalue);
		printf("Relative error in epsilon is: %f\n",Rerror);
		printf("error is: %f\n",error);

		printf("Load time: %lf\n",convolve_start-start_time);
		printf("Convolve time: %lf\n",convolve_end-convolve_start);
		printf("Reduce time: %lf\n",reduce_end-reduce_start);
		printf("Total time = %lf\n",reduce_end-start_time);
	}

	/* MPI_Finalize at the end */
	MPI_Finalize();
	return 0;
}
