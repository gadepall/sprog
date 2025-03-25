double rk4_2(double x, double *y1, double *y2, double x0, double h, 
           double (*f1)(double, double, double), 
           double (*f2)(double, double, double)) 
{
    double k1_1, k2_1, k3_1, k4_1;
    double k1_2, k2_2, k3_2, k4_2;

    int step_direction = (x > x0) ? -1 : 1;  // Ensure proper stepping direction

    while (fabs(x - x0) > 1e-3) 
    {
        k1_1 = h * f1(x, *y1, *y2);
        k1_2 = h * f2(x, *y1, *y2);

        k2_1 = h * f1(x + h / 2, *y1 + k1_1 / 2, *y2 + k1_2 / 2);
        k2_2 = h * f2(x + h / 2, *y1 + k1_1 / 2, *y2 + k1_2 / 2);

        k3_1 = h * f1(x + h / 2, *y1 + k2_1 / 2, *y2 + k2_2 / 2);
        k3_2 = h * f2(x + h / 2, *y1 + k2_1 / 2, *y2 + k2_2 / 2);

        k4_1 = h * f1(x + h, *y1 + k3_1, *y2 + k3_2);
        k4_2 = h * f2(x + h, *y1 + k3_1, *y2 + k3_2);

        *y1 += (k1_1 + 2 * k2_1 + 2 * k3_1 + k4_1) / 6;
        *y2 += (k1_2 + 2 * k2_2 + 2 * k3_2 + k4_2) / 6;

        x += step_direction * h;  // Step in the correct direction
    }
}
