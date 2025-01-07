#include<stdio.h>
#include<math.h>

float h = 0.001; // global variable

// function to calculate derivative
float func(float x){
      return 1.00/(x*(x*(x)-1.00));
}

// solution
void solution(float *x,float *y,int n){
        float k1,k2,k3,k4,k;
        for(int i=1;i<=n;i++){
	
	k1 = h*func(*x);
	k2 = h*func(*x+h/2);
	k3 = h*func(*x+h/2);
	k4 = h*func(*x+h);
	k = (1.0/6.0)*(k1 + 2*k2 + 2*k3 + k4);
	*y += k; // finding the next value of y
	*x += h; // increment in x
	return;
	}
}
