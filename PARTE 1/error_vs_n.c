/* error_vs_n.c -- mide el error cuadratico medio del estimador de pi
   para varios N, repitiendo con R semillas distintas. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(void){
  long Ns[] = {10000L, 40000L, 1000000L, 4000000L, 100000000L};
  int R = 20;
  for (int k = 0; k < 5; k++){
    long N = Ns[k]; double se = 0.0;
    for (int r = 0; r < R; r++){
      unsigned int estado = 20261018u + 7919u*r; long dentro = 0;
      for (long i = 0; i < N; i++){
        double x = rand_r(&estado)/(double)RAND_MAX;
        double y = rand_r(&estado)/(double)RAND_MAX;
        if (x*x + y*y <= 1.0) dentro++;
      }
      double e = 4.0*dentro/(double)N - M_PI; se += e*e;
    }
    double rmse = sqrt(se/R), teo = 4.0*sqrt((M_PI/4)*(1-M_PI/4)/N);
    printf("%ld %.6f %.6f\n", N, rmse, teo);
  }
  return 0;
}
