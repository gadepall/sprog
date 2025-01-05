#include<stdio.h>
#include<math.h>
//Xco function to get x-coordinates and store them in a pointer to a static array
double *Xco(){
	static double xco[10000];
	double h=0.001;
	xco[0]=0;//initial condition
	for(int i=1;i<10000;i++){
		xco[i]=xco[i-1]+h;
	}
	return xco;
}
//Yco function to get y-coordinates and store them in a pointer to a static array

double *Yco(){
	static double yco[10000];
	double h=0.001;
	yco[0]=1;//initial condition
	double *x=Xco();
	for(int i=1;i<10000;i++){
		//y-coordinate calculation
		yco[i]=yco[i-1]+(((2*(pow(x[i],2)))+x[i])/((1+x[i]+(pow(x[i],2))+(pow(x[i],3)))))*h;

	}

	return yco;
}

int main(){
	double* cox=Xco();//pointer to x-coordinates array 
	double* coy=Yco();//pointer to y-coordinates array
	//verifying if the points are stored in the arrays 
	printf("THE COORDINATES OF THE POINTS ARE:\n");
	for(int i=0;i<10000;i++){
		printf("points:(%.2f,%.2f)",cox[i],coy[i]);
		printf("\n");
	}
	return 0;
}





