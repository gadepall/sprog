#include <stdio.h>
#include<math.h>
int h=0.1,a=4,b=3;
double smolarea(double x,double y){


return (b * sqrt(1 - (x * x) / (a * a)))+((-9/16)*h*(x/(b * sqrt(1 - (x * x) / (a * a)))));


}
