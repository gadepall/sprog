#include <stdio.h>
#include <math.h>

double nr(double x){

    double result = x-((pow(x,2)-(3*x)-1)/((2*x)-3));
    return result;
}
