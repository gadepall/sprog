#include<math.h>

double dif(double x, double y){
    return sin(x) - 2 * y;
}

double theoretical_function(double x){
    return (2 * sin(x) - cos(x) + exp(-2 * x))/5;
}