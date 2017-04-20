/* Based on code from here: http://devblogs.nvidia.com/parallelforall/easy-introduction-cuda-c-and-c/ */


#include <stdio.h>
#include <stdlib.h>

#define N (1000*1000*8)

/* Calculate SAXPY, single-precision vector math 	*/
/* y[i]=a*x[i]+y[i]					*/

__global__
void saxpy (int n, float a, float *x, float *y) {

	int i=blockIdx.x*blockDim.x+threadIdx.x;

	/* Only run calculation if we are in range */
	/* where i is valid.  It can be out of range */
	/* if our vector is shorter than a */
	/* multiple of the blocksize */

	if (i<n) {
		y[i]=a*x[i]+y[i];
	}
}

int main(int argc, char **argv) {

	int i;
	float *x, *y, *dev_x, *dev_y;
	float a;

	float loops=1.0;

	if (argc>1) {
		loops=atof(argv[1]);
	}

	/* Allocate vectors on CPU */
	x=(float *)malloc(N*sizeof(float));
	y=(float *)malloc(N*sizeof(float));

	/* Allocate vectors on GPU */
	cudaMalloc((void **)&dev_x,N*sizeof(float));
	cudaMalloc((void **)&dev_y,N*sizeof(float));

	/* Initialize the host vectors */
	for(i=0;i<N;i++) {
		x[i]=(float)i;
		y[i]=(float)(10.0*i);
	}

	cudaMemcpy(dev_x,x,N*sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_y,y,N*sizeof(float),cudaMemcpyHostToDevice);

	printf("Size: %d\n",(N+255)/256);

	/* Perform SAXPY */
	for(a=0;a<loops;a+=1.0) {
		saxpy<<<(N+255)/256,256>>>(N,a,dev_x,dev_y);
	}

	cudaMemcpy(y,dev_y,N*sizeof(float),cudaMemcpyDeviceToHost);

	/* results */
	i=100;
	printf("y[%d]=%f %f\n",i,y[i],y[i+1]);

	/* 0: a=0, x=100, y=1000 ::::::: y=1000 */
	/* 1: a=1, x=100, y=1000 ::::::: y=1100 */
	/* 2: a=2, x=100, y=1100 ::::::: y=1300 */
	/* 3: a=3, x=100, y=1300 ::::::: y=1600 */
	/* 4: a=4, x=100, y=1600 ::::::: y=2000 */

	cudaFree(dev_x);
	cudaFree(dev_y);

	return 0;
}

