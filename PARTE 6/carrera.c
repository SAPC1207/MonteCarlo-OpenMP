/* carrera.c -- Parte 6.1: condicion de carrera A PROPOSITO.
   Todos los hilos suman sobre la misma variable sin proteccion. */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    const long N = 10000000;
    double *a = malloc(N * sizeof(double));
    for (long i = 0; i < N; i++) a[i] = 1.0;

    double suma = 0.0;
    #pragma omp parallel for
    for (long i = 0; i < N; i++)
        suma += a[i];                 // varios hilos escriben la misma variable

    printf("suma = %.0f (esperado %ld)\n", suma, N);
    free(a);
    return 0;
}
