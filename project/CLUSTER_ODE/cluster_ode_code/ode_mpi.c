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

int main(int argc, char **argv) {

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
