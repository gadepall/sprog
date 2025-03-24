#include<stdio.h>
#include<math.h>
double h = 0.01; // assuming a value for h(change in x coordinate) which is close to zero.
// xo,yo is a given point on the curve of the required function y = f(x)

// assuming initial conditions (xo,yo)
double xo = 0.5;   
double yo = 2;
// function 'coords' is defined which has no inputs and returns an array(pointer to an array to be precise)  
double *coords(){
	// defining an array of length 2000 to holds all x and y coords of 1000 points.
	// static array is defined in order to be able to return the array at the end of the function which holds 1000 points
	
	static double points[2000];

	// for loop for generating and storing points
	for(int i=0;i<=1998;i=i+2){

		if(i==0){
			points[i]=xo;
			points[i+1]=yo;
		}
		else{
			points[i]=points[i-2]+h;// X_n = X_(n-1)+h 
			points[i+1]=points[i-1] + h*((points[i-2]*points[i-1])/((points[i-1]*points[i-1]*points[i-1])+ (points[i-2]*points[i-2]*points[i-2])));// Y_n = Y_n-1 + h*(dy/dx)
		}
	}
	// returning the array
	return points;
}
// main function to check the calling and implementation of the function by printing the coords
int main(){
	double *pts = coords();
	for(int i=0;i<=1998;i+=2){
		printf("point %d: (%.2f, %.2f)\n",i/2 +1,pts[i],pts[i+1]);
	}

		
	return 0;
}

