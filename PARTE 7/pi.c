/* pi.c -- Parte 7: pi por la regla del punto medio sobre la integral de
   4/(1+x^2) entre 0 y 1. Imprime: hilos, tiempo de pared, tiempo de CPU, error. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    const long N = (argc > 1) ? atol(argv[1]) : 200000000;
    const double dx = 1.0 / N;
    double suma = 0.0;

    clock_t c0 = clock();              // tiempo de CPU
    double t0 = omp_get_wtime();       // tiempo de pared

    #pragma omp parallel for reduction(+:suma)
    for (long i = 0; i < N; i++) {
        double x = (i + 0.5) * dx;
        suma += 4.0 / (1.0 + x * x);
    }

    double t1 = omp_get_wtime();
    clock_t c1 = clock();
    double pi = suma * dx;
    printf("%d %.6f %.6f %.3e\n", omp_get_max_threads(), t1 - t0,
           (double)(c1 - c0) / CLOCKS_PER_SEC, fabs(pi - M_PI));
    return 0;
}
