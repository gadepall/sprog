#include<math.h>
#include<stdio.h>

double func(double x){
    return (x-1)*(x-1) + 3;
}

double diff(double x){
    return (x-1) *2;
}

// double x_init = -1.5;
// // while(1){

// // }

// while (x_init)
// {
//     1 ==1;
// }

int main(){
    double x_init = -1.5;
    double step = 0.0001;
    while (fabs(diff(x_init)) > 10e-10)
    {
        x_init -= step * diff(x_init);
    }
    printf("%.15lf",x_init);
}
