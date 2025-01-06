#include <stdio.h>
#include <math.h>

int get_points(double initial_x, double final_x, double step_size, int positive, double* x_vals, double* y_vals)
{
    int data_points = (final_x - initial_x) / step_size + 1; //No. of pointsfor iteration
    for (int i = 0; i < data_points; i++) {
        if (fabs(initial_x + 1) < 1e-6 || fabs(initial_x - 1) < 1e-6) {  // Handling y=0 case
            y_vals[i] = 0;
        } else {
            double y = sqrt(1 - initial_x * initial_x);
            y_vals[i] = positive ? y : -y; // to determine the positive or negative semicircle points
        }
        x_vals[i] = initial_x;
        initial_x += step_size;
    }
    return data_points; // Returning the number of points generated
}

