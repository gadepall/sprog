double h=0.01;
double msin(double x) {
    double ypp = 0, yp = 0.009999833334166664,y;
    int c = 0;
    c = (x>0)? 1:-1;
    x = (x>0)? x : -x;
    for(double i= 0; i<x;i+= h){
        y = 2*yp - ypp - (h*h*yp);
        ypp = yp;
        yp = y;
    }
    return c*y;
}

double mcos(double x) {
    double ypp = 1, yp = 0.99995,y;
    x = (x>0)? x : -x;
    for(double i= 0;i<x;i+= h){
        y = 2*yp - ypp - (h*h*yp);
        ypp = yp;
        yp = y;
    }
    return y;
}
double mtan(double x){
  int c = (x<0)?-1:1;
  double xx = (x>0)?x:-x;
  double yp = 0,y,k1,k2;
    for(double i= 0;i<xx;i+= h){
        k1 = h*(1+(yp*yp));
        k2 = h*(1+((yp+k1/2)*(yp+k1/2)));
        y = yp + (k1+k2)/2;
        yp = y;
    }
    return c*y;
} 
double mln(double x){
  double yp = -4.605,y,k1,k2;
    for(double i= h;i < x;i+= h){
        k1 = h/(i);
        k2 = h/(i+h);
        y = yp + (k1+k2)/2;
        yp = y;
    }
    return y;
}
double mlog(double x){
  return 0.4342944819*mln(x);
}
double mexp(double x){
  double c = 1;
  double yp = 1,y,k1,k2;
    for(double i= 0;i<x;i+= h){
        k1 = c*h*yp;
        k2 = c*h*(yp+k1/2);
        y = yp + (k1+k2)/2;
        yp = y;
    }
    return y;
}
double mpow(double a, double x){
  return mexp(x*mln(a));
}

double masin(double x){
  double yp = x, y;
  for(int i=0;(i<100)||((yp-y < 1e-3)&&(yp-y > -1e-3));i++){
    y = yp - (msin(yp)-x)/mcos(yp);
  }
  return y;
}

double macos(double x){
  double yp = x, y;
  for(int i=0;(i<100)||((yp-y < 1e-3)&&(yp-y > -1e-3));i++){
    y = yp + (mcos(yp)-x)/msin(yp);
  }
  return y;
}
double matan(double x){
  int c = (x<0)?-1:1;
  double xx = (x>0)?x:-x;
  double yp = 0,y,k1,k2;
    for(double i= 0;i<xx;i+= h){
        k1 = h/(1+(i*i));
        k2 = h*(1+((i+h/2)*(i+h/2)));
        y = yp + (k1+k2)/2;
        yp = y;
    }
    return c*y;
} 
