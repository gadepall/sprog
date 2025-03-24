double generate_ypoints(double y , double h ) {
    return y+h;
}
double generate_xpoints(double x , double h , double dybydx){
	return dybydx*h + x;
}
