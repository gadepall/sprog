#include <stdio.h>
#include <math.h>

//Function for y=|x|x
double f(double x){
	return (fabs(x)*x);
}

double Area(double a,double b,int n){
	double h=(b-a)/n;
	double area=0.0;

	for(int i=0;i<=n;i++){
		double x=a+i*h;
		if(i==0||i==n){
			area+=fabs(f(x));
		}
		else{
			area+=(2*fabs(f(x)));
		}
	}
	area = (area*h)/2;
	return area;
}
