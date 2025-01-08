#include <stdio.h>

#include<math.h>

double h=0.01;
double coordinate(double x, double y){

return y + h * (-2 * exp(-2 * x) - 3 * exp(-3 * x));
}
