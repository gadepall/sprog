#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000
int main() {
	srand(time(0));
	int count = 0;
	for (int i = 0; i < N; i++) {
		int roll = (rand() % 6) + 1;
		if (roll >= 3) { count++;
		}
	}
	double probability = (double)count / N; 
	printf("Estimated Probability: %lf\n", probability);
	return 0; 
}
