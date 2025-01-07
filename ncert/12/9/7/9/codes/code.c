#include <stdio.h>
#include <math.h>

void finite_difference(double x0, double y0, double x_end, double h, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    double x = x0, y = y0;
    fprintf(file, "# x\t\ty\n");  // Comment header for .dat file

    while (x <= x_end) {
        fprintf(file, "%lf\t%lf\n", x, y);  // Write x and y values to file
        y = y - h *((1+y*y)*exp(1)/(1+exp(2*x)));  // Update y using finite differences
        x = x + h;  // Increment x by step size h
    }

    fclose(file);
}

