/* MPI Send Example */
#include <stdio.h>
#include "mpi.h"

#define ARRAYSIZE 1024*1024

int main(int argc, char **argv) {

	int numtasks, rank;
	int result,i;
	int A[ARRAYSIZE];
	MPI_Status Stat;
	int count;

	/* Initialize MPI */
	result = MPI_Init(&argc,&argv);
	if (result != MPI_SUCCESS) {
		printf ("Error starting MPI program!.\n");
		MPI_Abort(MPI_COMM_WORLD, result);
	}

	/* Get number of tasks and our process number (rank) */
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	printf("R%d: Number of tasks= %d My rank= %d\n",
			rank,numtasks,rank);

	if (rank==0) {

		/* Initialize Array */
		printf("R0: Initializing array\n");
		for(i=0;i<ARRAYSIZE;i++) {
			A[i]=1;
		}

		/* Send the array to other processes */
		for(i=1;i<numtasks;i++) {
			printf("R0: Sending %d ints to %d\n",
				ARRAYSIZE,i);
			result = MPI_Send(A,		/* buffer */
					ARRAYSIZE,	/* count */
					MPI_INT,	/* type */
					i,		/* destination */
					13,		/* tag */
					MPI_COMM_WORLD);
		}
	}
	else {
		/* This is run by all the non-master processes */

		result = MPI_Recv(A,		/* buffer */
				ARRAYSIZE,	/* count */
				MPI_INT,	/* type */
				0,		/* source */
				13,		/* tag */
				MPI_COMM_WORLD,	/* communicator */
				&Stat);		/* status */
		/* You can get data from the status result */
		result = MPI_Get_count(&Stat, MPI_INT, &count);
		printf("\tR%d: Received %d ints "
			"from task %d with tag %d \n",
			rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
	}

	int sum=0,remote_sum=0;

	/* Calculate our chunk of the sum */
	for(i=rank*(ARRAYSIZE/numtasks);i<(rank+1)*(ARRAYSIZE/numtasks);i++) {
		sum+=A[i];
	}

	if (rank==0) {

		/* Master waits for sum from all processes */
		for(i=1;i<numtasks;i++) {
			result = MPI_Recv(&remote_sum, /* buffer */
					1,		/* count */
					MPI_INT,	/* type */
					MPI_ANY_SOURCE,	/* source */
					13,		/* tag */
					MPI_COMM_WORLD,
					&Stat);
			result = MPI_Get_count(&Stat, MPI_INT, &count);
			printf("\tR%d: Received %d integer (value=%d) "
				"from task %d with tag %d \n",
				rank,count,remote_sum,
				Stat.MPI_SOURCE, Stat.MPI_TAG);
			sum+=remote_sum;

		}
		printf("R0: Final Total: %d\n",sum);

	}
	else {
		printf("\tR%d Sending %d\n",rank,sum);
		result = MPI_Send(&sum,		/* buffer */
				1,		/* count */
				MPI_INT,	/* type */
				0,		/* destination */
				13,		/* tag */
				MPI_COMM_WORLD);

	}
	MPI_Finalize();
}
