double generate_xpoints(double x , double h ) {
    return x+h;
}
double generate_ypoints(double y , double h , double dybydx){
	return dybydx*h + y;
}
