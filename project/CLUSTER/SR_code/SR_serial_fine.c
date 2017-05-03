/* By Qihao He <qi.he@maine.edu> */
/* This program uses Simpson's Rule to compute PI */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
// #include <errno.h>
#include <math.h>

#define n 1e9 //iteration times
// #define epsilon 2.220446e-16//epsilon
#define epsilon 1e-15//epsilon
/* True value of Pi from Wolfwww.wolframalpha.com */
//64 data points after decimal point
// #define truepivalue 3.1415926535897932384626433832795028841971693993751058209749445923
#define truepivalue 3.141592653589793

double SR_f(double start, double end){
	double x,y;
	double temp1,temp2;
	double sum=0;
	int i;

	for(i=start;i<=end;i++){
		x=(double)(2*i-1)/(double)n;
		temp1= 4.0/(1.0+x*x);
		y=(double)(2*i)/(double)n;
		temp2= 4.0/(1.0+y*y);
		sum+= 4.0*temp1+2*temp2;
	}
	return sum;
}

int main(int argc, char *argv[]) {
	double total_sum;
	double Rerror;//Relative error
	double error;
	double temp1,temp2;
	// long double truepivalue=3.141592653589793;

	temp1=(double)0/(double)n;
	temp2=(double)n/(double)n;
	total_sum=4.0/(1+temp1*temp1)-4.0/(1+temp2*temp2);

	total_sum+=SR_f(1,n/2);//SR_f(start, end)

	total_sum/=(3.0*n);

	printf("Appr pi: %13.15f\n",total_sum);//print Approximate pi
	printf("True pi: %13.15f\n",truepivalue);//true value of pi
	printf("Epsilon: %1.15f\n",epsilon);//epsilon

	/* Relative error in epsilon */
	// printf("epsilon is: %f\n",(double)epsilon);
	Rerror=abs(total_sum-(double)truepivalue)/(double)epsilon;
	error=abs(total_sum-(double)truepivalue);
	printf("Relative error in epsilon is: %f\n",Rerror);
	printf("error is: %f\n",error);
	return 0;
}
