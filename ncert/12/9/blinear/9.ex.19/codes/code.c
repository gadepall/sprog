#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
        double h=0.01, n= 300, x = 0,y=0;
        FILE *file;
	file = fopen("values.dat", "w");
        fprintf(file, "x y\n");
        for(int i=0;i<=n;i++){
            fprintf(file,"%lf %lf\n", x,y);
            x += h;
            y = (y*(1+h/2) + (cos(x)+cos(x-h))*h/2)/(1 - h/2);
        }
	fclose(file);
	printf("Results have been written to values.dat\n");
	return 0;
}






