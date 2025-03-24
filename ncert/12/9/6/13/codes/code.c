#include <stdio.h>
#include<math.h>
double y_values(double y, double h, double x){
	return (y + h*(-2*y*tan(x) + sin(x)));
}
