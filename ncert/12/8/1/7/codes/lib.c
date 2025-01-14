#include<math.h>
#include<stdio.h>

//Trapezoid rule to calculate area
double trapez(double startx, double endx, int iters, double (*func)(double)){
	double starty = func(startx);
	double x[iters]; double y[iters];
	x[0] = startx; y[0] = starty;
	double Area = 0;
	double step = (endx-startx)/iters;
	for(int i = 1; i < iters; i++){
		x[i] = x[i-1]+step;
		y[i] = func(x[i]);	
		Area += step*(func(x[i])+func(x[i-1]))/2; 
	}
	printf("Required area = %lf sq.units\n", 2*Area);
}
//Functions specific to our question

//Mathematical function we dervived from the circle equation

double func(double x){
	return sqrt(13.0*13.0-x*x);
}

int main(){
	trapez(13/sqrt(2), 13, 100000, func);
}
