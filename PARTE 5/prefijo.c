/* prefijo.c -- Parte 5.1: suma prefijo paralelizada A PROPOSITO de forma incorrecta.
   Cada a[i] depende de a[i-1] (dependencia de flujo), asi que con varios
   hilos el resultado sale mal. */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const int N = 1000000;
    long *a = malloc(N * sizeof(long));
    for (int i = 0; i < N; i++) a[i] = 1;          // todos valen 1

    #pragma omp parallel for
    for (int i = 1; i < N; i++)
        a[i] = a[i - 1] + a[i];                    // usa el resultado anterior

    printf("a[N-1] = %ld (esperado %d)\n", a[N - 1], N);
    free(a);
    return 0;
}
