/* reducciones.c -- Parte 6.3: maximo, minimo y conteo en un solo bucle,
   con tres reducciones, comparado con la version secuencial. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(void) {
    const long N = 10000000;
    double *a = malloc(N * sizeof(double));
    for (long i = 0; i < N; i++)
        a[i] = sin(0.001 * i) * (i % 1000);

    /* Version secuencial (referencia) */
    double mx_s = -1e300, mn_s = 1e300;
    long pos_s = 0;
    for (long i = 0; i < N; i++) {
        if (a[i] > mx_s) mx_s = a[i];
        if (a[i] < mn_s) mn_s = a[i];
        if (a[i] > 0) pos_s++;
    }

    /* Version paralela con tres reducciones */
    double mx = -1e300, mn = 1e300;
    long positivos = 0;
    #pragma omp parallel for reduction(max:mx) reduction(min:mn) reduction(+:positivos)
    for (long i = 0; i < N; i++) {
        if (a[i] > mx) mx = a[i];
        if (a[i] < mn) mn = a[i];
        if (a[i] > 0) positivos++;
    }

    printf("Hilos: %d\n", omp_get_max_threads());
    printf("Maximo:    %.15f %.15f\n", mx_s, mx);
    printf("Minimo:    %.15f %.15f\n", mn_s, mn);
    printf("Positivos: %ld %ld\n", pos_s, positivos);
    free(a);
    return 0;
}
