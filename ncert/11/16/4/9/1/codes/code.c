#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TRIALS 50000

double Prob()
{
	srand(time(NULL)) ;
	int samplespace[5] = {0, 1, 3, 5, 7} ;
	int condition[2] = {5, 7} ;
	int count = 0.0 ;
	for ( int i=0; i<NUM_TRIALS; i++ )
	{
		int arr[4] = {condition[rand()%2], samplespace[rand()%5], samplespace[rand()%5], samplespace[rand()%5]} ;
		int temp = arr[3] + 10*arr[2] + 100*arr[1] + 1000*arr[0] ;
		if ( (arr[3] == 5 || arr[3] == 0) && (temp != 5000) )
		{
			count += 1 ;
		}

	}
	double probability = (double) count / NUM_TRIALS ;
	return probability ;

}
