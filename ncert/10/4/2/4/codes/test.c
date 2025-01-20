double newton(double x0,double f1, double f11){
	return x0 - f1/f11 ;
	}
double secant(double x1,double f1,double x0 , double f11){
	return x1 - f1*(x1-x0)/(f1-f11) ;
	}
