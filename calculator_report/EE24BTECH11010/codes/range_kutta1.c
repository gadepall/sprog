double rk4_1(double x, double y, double x0, double h, double (*function)(double, double)) 
{
    double k1, k2, k3, k4;
    
    // Ensure integration proceeds in the correct direction
    int step_direction = (x > x0) ? -1 : 1; 

    while (fabs(x - x0) > 1e-5) 
    {
        k1 = h * function(x, y);
        k2 = h * function(x + h / 2, y + k1 / 2);
        k3 = h * function(x + h / 2, y + k2 / 2);
        k4 = h * function(x + h, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x += step_direction * h;  // Ensure step direction is correct
    }
    
    return y;
}
