#include <stdio.h>

float yAB(float y_n, float h){
	return (y_n + h*(2.5));
}

float yBC(float y_n, float h){
	return (y_n - h);
}

float yCA(float y_n, float h){
	return (y_n + h*(0.75));
}

float x(float x_n, float h){
	return (x_n + h);
}

float y_valAB(float x){
	return (2.5*x-5);
}

float y_valBC(float x){
	return (-x+9);
}

float y_valCA(float x){
	return (0.75*x-1.5);
}
