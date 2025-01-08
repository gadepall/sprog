double generate_xpoints(double x , double h ) {
    return x+h;
}
double generate_ypoints(double y1 , double y2 , double dybydx , double h){
	return dybydx*h*h - y1 + 2*y2 ;
}
