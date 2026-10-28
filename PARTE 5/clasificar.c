/* clasificar.c -- Parte 5.2: comprobacion experimental de tres bucles.
   Cada bucle se ejecuta en secuencial (arreglo sec) y en paralelo
   (arreglo par) con los mismos datos, y se comparan los resultados. */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000

int main(void) {
    long *sec = malloc((N + 1) * sizeof(long));
    long *par = malloc((N + 1) * sizeof(long));
    int dif;

    /* Bucle 3: a[i] = a[i] * 2  (independiente) */
    for (int i = 0; i <= N; i++) { sec[i] = i; par[i] = i; }
    for (int i = 0; i < N; i++) sec[i] = sec[i] * 2;
    #pragma omp parallel for
    for (int i = 0; i < N; i++) par[i] = par[i] * 2;
    dif = 0;
    for (int i = 0; i <= N; i++) if (sec[i] != par[i]) dif++;
    printf("a[i] = a[i] * 2    -> diferencias: %d\n", dif);

    /* Bucle 2: a[i] = a[i+1] + 1  (anti-dependencia) */
    for (int i = 0; i <= N; i++) { sec[i] = i; par[i] = i; }
    for (int i = 0; i < N; i++) sec[i] = sec[i + 1] + 1;
    #pragma omp parallel for
    for (int i = 0; i < N; i++) par[i] = par[i + 1] + 1;
    dif = 0;
    for (int i = 0; i <= N; i++) if (sec[i] != par[i]) dif++;
    printf("a[i] = a[i+1] + 1  -> diferencias: %d\n", dif);

    /* Bucle 6: s = s + a[i]  (reduccion sin reduction -> carrera) */
    for (int i = 0; i < N; i++) sec[i] = 1;
    long s_sec = 0, s_par = 0;
    for (int i = 0; i < N; i++) s_sec = s_sec + sec[i];
    #pragma omp parallel for
    for (int i = 0; i < N; i++) s_par = s_par + sec[i];
    printf("s = s + a[i]       -> secuencial: %ld, paralelo: %ld\n", s_sec, s_par);

    free(sec); free(par);
    return 0;
}
