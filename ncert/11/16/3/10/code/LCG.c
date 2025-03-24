#include<math.h>
#include<stdio.h>

long int LCG(long int seed, int iterations){

    long int a = 1103515245;
    long int m = pow(2, 31);
    long int c = 123545;

    // int count[6] = {0};
    for(int i=0;i<iterations;i++){
        seed = (a*seed + c) % m;
        int l = (int)seed%6;
        // count[l]+=1;
    }
    return seed;
}

int main(){
    int iterations = 10000000;

    long int a = 1103515245;
    long int m = pow(2, 31);
    long int c = 123545;

    long int seed = 1232344;

    int count[6] = {0};
    for(int i=0;i<iterations;i++){
        seed = (a*seed + c) % m;
        int l = (int)seed%6;
        count[l]+=1;
    }

    for(int i=0;i<6;i++){
        printf("%d\n",count[i]);
    }
}

