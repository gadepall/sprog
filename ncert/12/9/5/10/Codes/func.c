#include<stdio.h>
#include <math.h>
void calculate_points_1(float x_0,float y_0,int n,float x[n],float y[n],double h){
	float y_1,x_1;
	x[0]=x_0; //adding x_0,y_0 to the arrays
	y[0]=y_0;
	for(int i=1;i<n;i++){
		y_1=y_0+h/(x_0+y_0);  //as y_n+1=y_n + h*(dy/dx)
		x_1=x_0+h;            //x_n+1=x_n+h
		x[i]=x_1;             //adding new x,y to arrays
		y[i]=y_1;             
		x_0=x_1;              //x_0 becomes x_1 for next iteration
		y_0=y_1;              //y_0 becomes y_1 for next iteration
	}
}
void calculate_points_2(float x_0,float y_0,int n,float x[n],float y[n],double h){
	float y_1,x_1;
	x[0]=x_0; //adding x_0,y_0 to the arrays
	y[0]=y_0;
	for(int i=1;i<n;i++){
		x_1=x_0+h*(x_0+y_0);  //as x_n+1=x_n + h*(dx/dy)
		y_1=y_0+h;            //y_n+1=y_n+h
		x[i]=x_1;             //adding new x,y to arrays
		y[i]=y_1;             
		x_0=x_1;              //x_0 becomes x_1 for next iteration
		y_0=y_1;              //y_0 becomes y_1 for next iteration
	}
}
void calculate_points_3(float x_0,float y_0,int n,float x[n],float y[n],double h){
	float y_1,x_1;
	x[0]=x_0; //adding x_0,y_0 to the arrays
	y[0]=y_0;
	for(int i=1;i<n;i++){
		x_1=(y_0+2*h+1+h*h+x_0)*pow(1+h,i)-(h*i+1+2*h+y_0);  //as x_n=(y_0+2h+1+x_0)(1+h)^n-(y_0+2h+1+hn)
		y_1=y_0+i*h;            //y_n=y_0+nh
		x[i]=x_1;             //adding new x,y to arrays
		y[i]=y_1;             
		x_0=x_1;              //x_0 becomes x_1 for next iteration
		y_0=y_1;              //y_0 becomes y_1 for next iteration
	}
}
int main(){
	float x[10],y[10];
	calculate_points_1(-1,0,10,x,y,0.01);
	for(int i=0;i<10;i++){
		printf("%f ",x[i]);
	}
}
