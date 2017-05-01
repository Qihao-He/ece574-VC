/* By Qihao He <qi.he@maine.edu> */
/* This program uses Simpson's Rule to compute PI */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#define n 50

double f (int i){
	double x;
	x=(double)i/(double)n;
	return 4.0/(1.0+x*x);
}

int main(int argc, char *argv[]) {
	double area;
	int i;
	area=f(0)-f(n);
	for(i=1;i<=n/2;i++) area+=4.0*f(2*i-1)+2*f(2*i);
	area/=(3.0*n);
	printf("Approximation of pi: %13.11f\n",area);
	return 0;
}
