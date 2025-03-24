#include<math.h>
#include <stdio.h>
typedef struct{
    double r_opt;
    double h_opt;
}Result;

double gradient(double r, double S){
	return (0.5)*(S - 6*M_PI*r*r);
}
Result gradient_ascent(double S, double lr, double tol, int itr) {
    double r = 1.0;  // Initial guess
    double grad;
    for(int i = 0;i < itr;i++) {
        grad = gradient(r, S);
        if(fabs(grad) < tol){
            break;
        }
        r += lr*grad;
    }
    Result res;
    res.r_opt = r;
    res.h_opt = (S - 2*M_PI*r*r)/(2*M_PI*r);  // Compute the corresponding height
    return res;
}
