#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

//Let the differential equation we want to solve is (an)*(yn)+(an-1)*(yn-1)+...+(a0)*(y)+c=0 where yi is ith derivative of y and ai is a constant
//The output of the recorddata function will be all the values of yk = y(x+kh) in an array where k is an integer

double** matrixgen(int order, double coefficients[order+2], double stepsize){
	//Generates the matrix which must be multiplied to yk to get yk+1
	double** outputmatrix = identity(order+1);
	//initializing with identity matrix and changing the elements
	for(int i=1; i<order; i++){
		outputmatrix[i][i+1] = stepsize;
	}
	//changing the elements just beside diagonal elements
	outputmatrix[order][0] = -1/coefficients[0]*stepsize;
	for(int i=1; i<order+1; i++){
		outputmatrix[order][i] = (-coefficients[order+1-i]/coefficients[0])*stepsize;
	}
	outputmatrix[order][order] += 1;
	//changing the last row elements
	return outputmatrix;
}

double* recorddata(double lowerbound, double upperbound, int order,  double coefficients[order+2], double initialconditions[order], double stepsize){
	double** vector_y = createMat(order+1,1);
	vector_y[0][0] = coefficients[order+1];
	for(int i=0;i<order;i++){
		vector_y[i+1][0] = initialconditions[i];
	}
	//The vector_y is [c, y(t), y'(t), ..., yn-1(t)]
	//It is initialised to the initial conditions which are part of input given by user 
	double** matrix = matrixgen(order, coefficients, stepsize);
	//the matrix which has to be multiplied is generated
	int no_datapoints = ((upperbound-lowerbound)/stepsize);
	//It is max no of points which we want to plot, The lower bound is min value of x, upper bound is max value of x, stepsize is the value of h
	double* yvalues = malloc(no_datapoints*sizeof(double));
	for(int i = 0; i<no_datapoints; i++){
	//generating the vector_y for each value of t = lowerbound+stepsize*i until t reaches upperbound so all the vector_y values for each t are recorded at some point of time
		vector_y = Matmul(matrix,vector_y,order+1,order+1,1);
		yvalues[i] = vector_y[1][0];
		//recorded y(t) in each iteration in yvalues which should be the output
	}
	return yvalues;
}
int main(){
    int order = 2; //The differential which I have to solve is y''-2y'+2y = 0
    double coefficients[] = {1.0, -2.0, 2.0, 0.0};
    double initialconditions[] = {1.0, 0.0}; //Took initial conditions as y(0) = 1, y'(0) = 0 
    double lowerbound = 0.0;
    double upperbound = 10.0;
    //taking the datapoints between 0 and 10
    double stepsize = 0.001;// h = 0.001

    // Record data using the provided recorddata function
    double* results = recorddata(lowerbound, upperbound, order, coefficients, initialconditions, stepsize);

    // Print results
    int no_datapoints = (int)((upperbound - lowerbound) / stepsize);
    for (int i = 0; i < no_datapoints; i++) {
    	printf("This is value at x=%lf :: %lf\n",(i+1)*stepsize,results[i]);
    }
    free(results); // Free memory for the result array

    return 0;
}
