#include <stdio.h>
#include <openssl/rand.h>
#include <math.h>
//1 - tails; 0 - heads
#define NUM_REPETITIONS 10000
int sim_bernoulli(double p){
    unsigned char random_byte;
    if (RAND_bytes(&random_byte, 1) != 1) {
        fprintf(stderr, "Error generating random byte\n");
        exit(1);
    }
  /*
  * we generate a random number between 0 and 255, then scale it down to a number from   
  * 0 to 1 and we retun 0 only if the number is between 0 and p
  */ 
    double random_value = random_byte / 255.0;
    return random_value < p ? 0 : 1; 
}

void sim_binomial(int n, double p, double *count_arr){
  for(int i = 0; i < n+1; i++){
    count_arr[i] = 0;
  }
  for(int i = 0; i <= NUM_REPETITIONS; i++){
      int count = 0;
    for(int j = 0; j < n; j++){
      count += sim_bernoulli(p); 
    }
    count_arr[count]++;
  }
  for (int i = 0; i <= n; i++){
    count_arr[i] /= (float) NUM_REPETITIONS;
    printf("Probability of R.V = %d is %lf\n", i, count_arr[i]);
  }
}

int factorial(int n){
  if (n < 0){
    return 0;
  } else if (n == 0 || n == 1){
    return 1;
  } else {
    int res = 1;
    for (int i = 2; i <= n; i++){
      res *= i;
    }
    return res;
  }
}

void theoretical_binomial(int n, double p, double *probability_arr){
  for(int i = 0; i < n+1; i++){
    probability_arr[i] = (float)factorial(n)/(factorial(i) * factorial(n - i)) * pow(p, i) * pow(1-p,n - i);
    printf("%lf\n", probability_arr[i]);
  }
}

void theoretical_cdf(int n, double p, double *probability_arr){
  theoretical_binomial(n, p, probability_arr);
  for (int i= 1; i < n+1; i++){
    probability_arr[i] += probability_arr[i-1];
  }
}

void sim_cdf(int n, double p, double *probability_arr){
  sim_binomial(n, p, probability_arr);
  for (int i= 1; i < n+1; i++){
    probability_arr[i] += probability_arr[i-1];
  }
}
