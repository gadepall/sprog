#include <stdio.h>
#include <math.h>

// Function to compute the update rule
double update_hc(double hc, double h, double alpha, double tan_alpha_squared) {
    double term1 = 1;
    double term2 = -4 * hc / h;
    double term3 = 3 * pow(hc, 2) / pow(h, 2);
    double dV_dhc = M_PI * pow(h, 2) * tan_alpha_squared * (term1 + term2 + term3);
    return hc + alpha * dV_dhc;
}
