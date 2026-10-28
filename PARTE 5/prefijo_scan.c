/* prefijo_scan.c -- EXTRA: suma prefijo paralela CORRECTA con la directiva scan
   (OpenMP 5.0). GCC la acepta desde la version 10, aunque la macro _OPENMP
   siga reportando 201511 (4.5), porque GCC aun no implementa toda la 5.0. */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    const int N = 1000000;
    long *a = malloc(N * sizeof(long));
    for (int i = 0; i < N; i++) a[i] = 1;

    long acum = 0;
    #pragma omp parallel for reduction(inscan, +:acum)
    for (int i = 0; i < N; i++) {
        acum += a[i];                    // fase 1: cada hilo suma su bloque
        #pragma omp scan inclusive(acum)
        a[i] = acum;                     // fase 2: se escribe el prefijo ya corregido
    }

    int errores = 0;
    for (int i = 0; i < N; i++) if (a[i] != i + 1) errores++;
    printf("p = %2d | a[N-1] = %ld (esperado %d) | errores: %d\n",
           omp_get_max_threads(), a[N - 1], N, errores);
    free(a);
    return 0;
}
