// #include <stdio.h>
#include <stdlib.h>

#define ITERS 16
#define Kn 0.6072529351031394
#define PI 3.141592653589793
#define E 2.718281828459045
#define ln10 2.302585092994046

#ifndef NAN
  #define NAN (0.0/0.0)
#endif

float atanV[] = {
    0.7853981633974483, 
    0.4636476090008061, 
    0.24497866312686414, 
    0.12435499454676144, 
    0.06241880999595735, 
    0.031239833430268277, 
    0.015623728620476831, 
    0.007812341060101111, 
    0.0039062301319669718, 
    0.0019531225164788188, 
    0.0009765621895593195, 
    0.0004882812111948983, 
    0.00024414062014936177, 
    0.00012207031189367021, 
    6.103515617420877e-05, 
    3.0517578115526096e-05};

float ePow[] = {
    1.648721270700128, 
    1.284025416687742, 
    1.133148453066826, 
    1.064494458917859, 
    1.031743407499103, 
    1.015747708586686, 
    1.007843097206488, 
    1.003913889338348, 
    1.001955033591003, 
    1.000977039492417, 
    1.000488400478694, 
    1.000244170429748, 
    1.000122077763384, 
    1.000061037018933, 
    1.000030518043791, 
    1.0000152589054785};

float __sincos(float alpha, int c){
  float theta = 0.0;
    float x = 1.0;
    float x1;
    float y = 0.0;
    float y1;
    float P2i = 1.0;
    float sigma = 0.0;
    for(int i = 0;i<ITERS;i++){
        sigma = (theta < alpha) ? 1 : -1;
        theta += sigma * atanV[i];
        x1 = x - sigma * y * P2i;
        y1 = y + x * sigma * P2i;
        x = x1;
        y = y1;
        P2i /= 2;
    }
    if(c) return Kn * x;
    return Kn * y;
}

float sinCord(float alpha){
    return __sincos(alpha, 0);
}

float cosCord(float alpha){
    return __sincos(alpha, 1);
}

float tanCord(float alpha){
    if(alpha == PI/2){
      return NAN;
    }

    float theta = 0.0;
    float x = 1.0;
    float x1;
    float y = 0.0;
    float y1;
    float P2i = 1.0;
    float sigma = 0.0;
    for(int i = 0;i<ITERS;i++){
        sigma = (theta < alpha) ? 1 : -1;
        theta += sigma * atanV[i];
        x1 = x - sigma * y * P2i;
        y1 = y + x * sigma * P2i;
        x = x1;
        y = y1;
        P2i /= 2;
    }

    return y / x;   
}

float atan2Cord(float y, float x){
    /* return thet = atan(y/x)*/
    float sigma = (y>0)?-1:1;
    float x1 = - sigma * y;
    float y1 = sigma * x;
    x = x1;
    y = y1;
    float P2i = 1.0;
    float thet = sigma * PI/2;
    for(int i=0;i<ITERS;i++){
        sigma = (y>0)?-1:1;
        x1 = (x - sigma * P2i * y);
        y1 = y + sigma * P2i * x;
        x = x1;
        y = y1;
        thet += sigma * atanV[i];
        P2i /= 2;
    }
    return -thet;
}

float atanCord(float x){
    return atan2Cord(x, 1);
}

float sqrt_cordic ( float x ){
  int i;
  float P2i;
  float y;

  if ( x < 0.0 ){
    return NAN;
  }

  if ( x == 0.0 ){
    y = 0.0;
    return y;
  }

  if ( x == 1.0 ){
    y = 1.0;
    return y;
  }

  P2i = 1.0;

  if ( x < 1.0 ){
    while ( x <= P2i * P2i ){
      P2i = P2i / 2.0;
    }
    y = P2i;
  }
  else{
    while ( P2i * P2i <= x ){
      P2i = 2.0 * P2i;
    }
    y = P2i / 2.0;
  }

  for ( i = 1; i <= ITERS; i++ ){
    P2i = P2i / 2.0;
    if ( ( y + P2i ) * ( y + P2i ) <= x ){
      y = y + P2i;
    }
  }

  return y;
}

float arcsin_cordic ( float t){

  float P2i = 1.0;
  float sigma = 0;
  float sign_z1;
  float theta = 0.0;
  float x1 = 1.0;
  float x2;
  float y1 = 0.0;
  float y2;

  if ( (1.0 < ( t )) && (-1 > t)){
    return NAN;
  }

  for ( int j = 1; j <= ITERS; j++ ){

    sign_z1 = (x1 < 0)? -1 : 1;

    sigma += (y1 <= t) ? sign_z1 : -sign_z1; 

    for (int i = 1; i <= 2; i++ ){
      x2 =                      x1 - sigma * P2i * y1;
      y2 = sigma * P2i * x1 +                      y1;

      x1 = x2;
      y1 = y2;
    }

    theta  += 2.0 * sigma * atanV[j-1];

    t += t * P2i * P2i;

    P2i /= 2.0;
  }

  return theta;
}

float arccos_cordic ( float t){

  float P2i = 1.0;
  float sigma = 0;
  float sign_z2;
  float theta = 0.0 ;
  float x1 = 1.0;
  float x2;
  float y1 = 1.0;
  float y2;

  if ( (1.0 < ( t )) && (-1 > t)){
    return NAN;
  }

  for (int j = 1; j <= ITERS; j++ ){
    sign_z2 = (y1 < 0)? -1.0 : 1.0;

    sigma += (t<=x1)? sign_z2 : -sign_z2;

    for (int i = 1; i <= 2; i++ ){
      x2 =                      x1 - sigma * P2i * y1;
      y2 = sigma * P2i * x1 +                      y1;

      x1 = x2;
      y1 = y2;
    }

    theta  += 2.0 * sigma * atanV[j-1];

    t += t * P2i * P2i;

    P2i /= 2.0;
  }

  return theta;
// # undef ANGLES_LENGTH
}

float expCord(float x){
    int x_int = (x > 0) ? (int) x : ((int) x) - 1;
    float P2i = 0.5;
    float z = x - x_int;
    float y = 1.0;
    for(int i=0;i<ITERS;i++){
        if(P2i < z){
            z -= P2i;
            y *= ePow[i];
        }
        P2i /= 2;
    }

    if(x_int > 0){
        for(int i=0;i<x_int;i++){
            y *= E;
        }
    }
    else{
        for(int i=0;i<-x_int;i++){
            y /= E;
        }
    }
    return y;
}

float lnCord(float x){

  float fx;
  int k = 0;
  float P2i = 0.5;
  float *w;

  if ( x <= 0.0 ){
    return NAN;
  }

  while ( E <= x ){
    k = k + 1;
    x = x / E;
  }

  while ( x < 1.0 ){
    k -= 1;
    x *= E;
  }

  w = ( float * ) malloc ( ITERS * sizeof ( float ) );

  for (int i = 0; i < ITERS; i++ ){
    w[i] = 0.0;
    if ( ePow[i] < x ){
      w[i] = 1.0;
      x /= ePow[i];
    }
  }

  x -= 1.0;

  x = x * ( 1.0 - ( x / 2.0 ) * ( 1.0 + ( x / 3.0 ) * ( 1.0 -   x / 4.0 )));

  for (int i = 0; i < ITERS; i++ ){
    x += w[i] * P2i;
    P2i /= 2.0;
  }

  fx = x + ( float ) ( k );

  free ( w );

  return fx;

}

float logCord(float x){
    float y2 = lnCord(x);
    return y2 / ln10;
}

float powCord(float a, float p){
  if(a == 0) return 0;
  if(p == 0) return 1;

  float lnx = lnCord(a);
  lnx *= p;
  lnx = expCord(lnx);

  return lnx;
}
