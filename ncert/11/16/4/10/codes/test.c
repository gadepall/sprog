#include<stdio.h>
#include<stdlib.h>
#include<time.h>

double prob(){
	srand(time(NULL));
	int count = 0;
	for(int i=0;i<10000;i++){
		int a = rand() % 10;
		int b = rand() % 10;
		if(a == b){
			continue;
		}
		int c = rand() % 10;
		if(c == a || c == b){
			continue;
		}
		int d = rand() % 10;
		if(d == a || d == b || d == c){
			continue;
		}
		count++;
	}
	return 1.0/count;
}

int main(){
	printf("%lf\n", prob());
	return 0;
}
