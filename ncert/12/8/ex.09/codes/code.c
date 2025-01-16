#include <stdio.h>
#include <math.h>  // Include math.h for fabs function

__attribute__((visibility("default"))) double Area(double x1, double y1, double x2, double y2, double x3, double y3) {
    return 0.5 * fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));  // Use fabs for floating-point values
}

