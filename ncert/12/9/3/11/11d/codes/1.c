#include<stdlib.h>

void points(double h,double y[]){
     double x=0.0;
    
     for(int i=1;i<=100;i++){
     y[i]=y[i-1]+h*x;
     x+=h;
     }
}
