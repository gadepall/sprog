#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
        double h=0.01, n= 300, x = 0,y=1,yp=y, y_pr = 1, y_pp = 1;
        double a = 1 + h - (h*h) / 2, b = 1 - h + (h*h) / 2, c = -2*h / b;
        double xx = 0, y0 = 1, y1 = 1.01, y2 = 0;
        FILE *file;
	file = fopen("values.dat", "w");
        fprintf(file, "x y\n");
        for(int i=0;i<=n;i++){
            fprintf(file,"%lf %lf ", x,y);
            x += h;
            yp = y;
            y_pp = y_pr;
            y = yp + (h / 2) * (y_pr * (a / b) + c * y + y_pr);
            y_pr = y_pp*(a / b) + (c * yp);
            
            fprintf(file, "%lf %lf\n", xx, y0);
            xx+= h;
            y2 = -2*y1*(2*h*h - 4)/(pow(2-h,2)+pow(h,2)) - y0*((pow(2+h,2)+pow(h,2))/(pow(2-h,2)+pow(h,2)));
            y0 = y1;
            y1 = y2;
        }
	fclose(file);
	printf("Results have been written to values.dat\n");
	return 0;
}






