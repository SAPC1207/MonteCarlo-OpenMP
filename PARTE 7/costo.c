/* costo.c -- (de la Parte 2) costo de abrir y cerrar una region paralela vacia. */
#include <stdio.h>
#include <omp.h>

int main(void) {
    const int R = 100000;
    volatile int x = 0;
    double t0 = omp_get_wtime();
    for (int r = 0; r < R; r++) {
        #pragma omp parallel
        { x = 1; }
    }
    double t1 = omp_get_wtime();
    printf("%d hilos: %.3f us por region\n", omp_get_max_threads(), (t1 - t0) / R * 1e6);
    return 0;
}
