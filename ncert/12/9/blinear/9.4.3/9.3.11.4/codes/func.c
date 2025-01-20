void calculate_y(int n, double h, double *y){
    // Initial conditions
    y[0] = 0.0;  // y_0 = 0
    y[1] = 0.0;  // y_1 = 0 (from y'(0) = 0)
    // Calculate y values using the difference equation
    for(int i = 1; i < n - 1; i++){
	    y[i + 1] = 2*y[i] - y[i - 1] + h*h;
    }
}
