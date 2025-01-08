#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
// Function is ((x*cos(y/x) + y*(sin(y/x)))*y)/((-x*cos(y/x) + y*(sin(y/x)))*x)
// It is required check whether x or/and denominator is going to zero, as it will cause the result to go to infinity.
double derivative(double x, double y){
    if(x==0 || y==0) return 0;
    else{
      double num = (x*cos(y/x) + y*(sin(y/x)))*y;
      double den = (-x*cos(y/x) + y*(sin(y/x)))*x;
      if(den == 0 && num == 0) return 0;
      else{
        return num/den;
      }
    }
}
void frontbackstepper(double x1, double y1, double h1, double h2, double n,FILE *file){
  double x0 = x1, x00 = x1, y0 = y1, y00 = y1;
  for(int j=0.01;j<n;j++){
          for(int i=0;i<5000;i++){
            //Setting the derivative limit, and if the limit is exceeded, it reverses the step and increases the y-value by a bit.
            if(1/derivative(x0,y0) >20) {x0-= 0.01; y0+= h2; continue;}
            else {
              // Going throught the small step
              x0 += ((1/derivative(x0,y0))*h2);y0 += h2;
            }
            if(1/derivative(x00,y00) > 20) {x00 += 0.01; y00 -= h2;continue;}
            else {
              x00 -= ((1/derivative(x00,y00))*h2);y00 -= h2;
            }
            //Below are the boundary conditions bounding the graph so that it remains within a 40X40 Cartesian plane
            if(x0<40 && x0>-0 && y0<40 && y0>-0) {fprintf(file, "%.02lf %.02lf\n", x0,y0);} else continue;
            if(x00>-0 && x00<40 && y00<40 && y00>-0) { fprintf(file, "%.02lf %.02lf\n", x00,y00);} else continue;
          }
          x1 -= derivative(x1,y1)*h1;
          y1 -= h1;
          x0 = x1; x00 = x1;
          y0 = y1; y00 = y1;
  }
}

double dist(double x, double y){
  return sqrt((x*x)+(y*y));
}

int main() {
        double x0 = 1, y0 = 1, h2 = 0.01, h1 = 1;
        double x00 = x0, y00 = y0, x1 = x0,y1 = y0, x11 = x00, y11 = y00, xstore = x1, ystore = y1;
        FILE *file;
	file = fopen("values.dat", "w");
        fprintf(file, "x y\n");
        for(double i=0.1;i<=h1;i+=0.001){
            frontbackstepper(x1,y1,i,h2,5,file);
        }
	fclose(file);
	printf("Results have been written to values.dat\n");
	return 0;
}






