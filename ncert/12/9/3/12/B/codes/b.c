#include <stdio.h>
#include <math.h>
// increasing the value of x by h where x_n means the new values of the x
float x_n(float x,float h){
	return x+h;
}
// we have already assumed the y_0 to find y_1 we are using finite differences method for first derivative
float y_1(float y_0,float dy,float h){
	return h*dy+y_0; // here dy means first derivative of y at x=0 i.e y'(0) 
}
// to find the values of y from y2
float y_n(float y_1,float y_0,float h,float x_0){//here y_1 and y_0 represents y_n-1 and y_n-2
	return y_1*(2-h*x_0) + y_0*(-1+h*x_0-h*h*x_0) + h*h*x_0;//after substituting the y' and y" values in the given equation i got y_n+2=y_n-1(2-hx_n)+ y_n(1+hx_n-h^2x_n) + h^2x_n
}
float f(float x, float y, float v) {
    return x - x * v - x * y;
}
// To calculate k1_y and k1_z
void k1(float h, float x, float y, float z, float *k1_y, float *k1_z) {
    *k1_y = h * z;
    *k1_z = h * f(x, y, z);//here x,y,z represents x_n,y_n,z_n
}

// To calculate k2_y and k2_z
void k2(float h, float x, float y, float z, float k1_y, float k1_z, float *k2_y, float *k2_z) {
    *k2_y = h * (z + k1_z / 2);
    *k2_z = h * f(x + h / 2, y + k1_y / 2, z + k1_z / 2);
}

// To calculate k3_y and k3_z
void k3(float h, float x, float y, float z, float k2_y, float k2_z, float *k3_y, float *k3_z) {
    *k3_y = h * (z + k2_z / 2);
    *k3_z = h * f(x + h / 2, y + k2_y / 2, z + k2_z / 2);
}

// To caculate k4_y and k4_z
void k4(float h, float x, float y, float z, float k3_y, float k3_z, float *k4_y, float *k4_z) {
    *k4_y = h * (z + k3_z);
    *k4_z = h * f(x + h, y + k3_y, z + k3_z);
}

