#include<stdio.h>
#include<math.h>

double h = 0.001; //step size
double f1(double x){
	return sqrt(1-pow(x-1,2));// function for upper curve f(x) = sqrt(1-(x-1)^2)
}

double f2(double x){
	return sqrt(1-pow(x,2));// function for upper curve f(x) = sqrt(1-x^2)
}

double trapezoidal(double x1, double x2, int steps){
	double area = 0.0;
	double x = x1;

	for(int i=0;i<steps;i++){
		double next_x = x+h; // Next x value
		if(next_x > x2) break; //break if greater than x2
		// TRAPEZOIDAL FORMULA
		double avg_height = ((f1(x) - f2(x)) + (f1(next_x) - f2(next_x))) / 2.0;
		area += h * avg_height;
		x = next_x;// increment x
	}
	return area;
}	
