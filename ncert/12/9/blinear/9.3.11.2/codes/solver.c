#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

//Let the differential equation we want to solve is (an)*(yn)+(an-1)*(yn-1)+...+(a0)*(y)+c=0 where yi is ith derivative of y and ai is a constant
//The output of the recorddata function will be all the values of yk = y(x+kh) in an array where k is an integer

double* recorddata_trapezoid(double lowerbound, double upperbound, double stepsize, double initialy0, double initialyprime0){
	int no_datapoints = (int)((upperbound-lowerbound)/stepsize)+1;
	double* y_values = malloc(no_datapoints*sizeof(double));
	double y1 = initialy0;
	double y2 = initialyprime0;
	for(int i=0; i<no_datapoints; i++){
		y_values[i] = y1;
		y1 = (y1*(4+stepsize*stepsize)+4*y2*stepsize)/(4-stepsize*stepsize);
		y2 = (y2*(4+stepsize*stepsize)+4*y1*stepsize)/(4-stepsize*stepsize);
	}
	return y_values;
}

double* recorddata_ztransform(double lowerbound, double upperbound, double stepsize, double initialy0, double initialyprime0){
	int no_datapoints = (int)((upperbound-lowerbound)/stepsize)+1;
	double* y_values = malloc(no_datapoints*sizeof(double));
	y_values[0] = initialy0;
	y_values[1] = initialy0 + stepsize*initialyprime0;
	double alpha1 = -(stepsize - 2) / (stepsize + 2), alpha2 = 1 / alpha1;
	for(int i = 1; i<(no_datapoints-1); i++){
		y_values[i+1] = (alpha1 + alpha2) * y_values[i] - y_values[i-1];
	}
	return y_values;
}
