#include<stdio.h>
#include<math.h>

double func(double y){
    return pow(y, 7) - 70 * pow(y, 6) + 1225 * pow(y, 5);
}

double funcder(double y){
    return 7 * pow(y, 6) - 420 * pow(y, 5) + 6125 * pow(y, 4);
}

double gradientaccent(double initial_y, double h, double max_y, double toler){
    double y = 10;
    double step = 0;
    while(y <= max_y){
        step = h * funcder(y);
        //  printf("%lf %lf ",y,step);
        // printf("%lf\n",funcder(y));
        y += step;
        if (fabs(step) <= toler) {
            break;
        }
    }
    return y;
}

// double gradientdecent(double initial_y, double h, double min_y, double toler){
//     double y = initial_y;
//     double step = 0;
//     while(y >= min_y){
//         step = -h * funcder(y); 
//         y -= step;
        
//         if (fabs(step) <= toler) {
//             break;
//         }
//     }
//     return y;
// }

// int main(){
//     double initialy=0;
//     double h=1e-10;
//     double toler=1e-6;
//     double maxy=35.000;
//     double y=gradientaccecent(initialy,h,maxy,toler);
//     printf("%lf",y);
//     return 0;
// }