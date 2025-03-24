#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define trials 1000000

int main() {
  int RWcount = 0, WRcount = 0, WWcount = 0;
  srand(time(NULL));

  for (int i = 0; i < trials; i++){
    int firstRed = (rand() % 4 == 0);
    int secondRed = (!firstRed && (rand() % 3 == 0));
    if (firstRed)
      RWcount++;
    else if (secondRed)
      WRcount++;
    else
      WWcount++;
  }

  printf("P(RW) = %lf\n", RWcount / (double)trials);
  printf("P(WR) = %lf\n", WRcount / (double)trials);
  printf("P(WW) = %lf\n", WWcount / (double)trials);

  return 0;
}
