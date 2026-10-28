/* carrera_reduction.c -- Parte 6.2: la misma suma, corregida con reduction.
   Cada hilo suma en su copia privada y al final se combinan. */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    const long N = 10000000;
    double *a = malloc(N * sizeof(double));
    for (long i = 0; i < N; i++) a[i] = 1.0;

    double suma = 0.0;
    #pragma omp parallel for reduction(+:suma)
    for (long i = 0; i < N; i++)
        suma += a[i];                 // cada hilo suma en su copia privada

    printf("suma = %.0f (esperado %ld)\n", suma, N);
    free(a);
    return 0;
}
