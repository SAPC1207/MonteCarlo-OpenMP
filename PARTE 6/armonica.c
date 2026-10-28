/* armonica.c -- Parte 6.4: suma armonica H(10^8) con reduccion en punto flotante. */
#include <stdio.h>
#include <omp.h>

int main(void) {
    double h = 0.0;
    #pragma omp parallel for reduction(+:h)
    for (long i = 0; i < 100000000; i++)
        h += 1.0 / (i + 1);
    printf("%d hilos: %.17f\n", omp_get_max_threads(), h);
    return 0;
}
