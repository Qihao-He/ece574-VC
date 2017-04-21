#include <stdio.h>
#include <stdlib.h>

#define N (1000*1000*8)

int main(int argc, char **argv) {

	int i,j;
	float *x,*y;

	int loops=1;

	if (argc>1) {
		loops=atoi(argv[1]);
	}

	x=malloc(N*sizeof(float));
	y=malloc(N*sizeof(float));
	if ((x==NULL) || (y==NULL)) {
		printf("Error allocating memory!\n");
		return -1;
	}

	/* Fill the host arrays with values */
	for(i=0;i<N;i++) {
		x[i]=(float)i;
		y[i]=(float)(10.0*i);
	}

	/* SAXPY, single-percision y=a*x+y */
	for(j=0;j<loops;j++) {
		for(i=0;i<N;i++) {
			y[i]=(float)j*x[i]+y[i];
		}
	}

	/* results */
	i=100;
	printf("y[%d]=%f\n",i,y[i]);

	/* 0: a=0, x=100, y=1000 ::::::: y=1000 */
        /* 1: a=1, x=100, y=1000 ::::::: y=1100 */
        /* 2: a=2, x=100, y=1100 ::::::: y=1300 */
        /* 3: a=3, x=100, y=1300 ::::::: y=1600 */
        /* 4: a=4, x=100, y=1600 ::::::: y=2000 */

	free(x);
	free(y);

	return 0;
}
