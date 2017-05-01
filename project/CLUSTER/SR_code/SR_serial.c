/* By Qihao He <qi.he@maine.edu> */
/* This program uses Simpson's Rule to compute PI */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#define n 1e9 //iteration times

/* True value of Pi from Wolfwww.wolframalpha.com */
#define truePIvalue 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756

double f (int i){
	double x;
	x=(double)i/(double)n;
	return 4.0/(1.0+x*x);
}

int main(int argc, char *argv[]) {
	double area;
	int i;
	double Rerror;//Relative error

	area=f(0)-f(n);
	for(i=1;i<=n/2;i++) area+=4.0*f(2*i-1)+2*f(2*i);
	area/=(3.0*n);
	printf("Approximation of pi: %13.64f\n",area);//print pi

	/* Relative error in epsilon */
	Rerror=abs(area-(double)truePIvalue);
	printf("Relative error in epsilon is: %f\n",Rerror)
	return 0;
}
