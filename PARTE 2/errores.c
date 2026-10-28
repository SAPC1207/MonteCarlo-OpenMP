/* errores.c -- versiones INCORRECTAS a proposito, para evidenciar II.3 y II.4.
   Compilar: gcc -O2 -fopenmp errores.c -o errores -lm
   Uso:      ./errores <modo> [N] [p]
     modo 1: todos los hilos llaman rand() con el estado global (srand una vez)
     modo 2: todos los hilos llaman rand_r() sobre UNA variable de estado compartida
     modo 3: cada hilo tiene su estado, pero TODOS con la misma semilla
     modo 4: semillas correctas, pero 'dentro++' compartido SIN reduction */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int  modo = (argc > 1) ? atoi(argv[1]) : 1;
    long N    = (argc > 2) ? atol(argv[2]) : 100000000L;
    if (argc > 3) omp_set_num_threads(atoi(argv[3]));
    const unsigned int base = 20261018u;
    long dentro = 0;
    double t0 = omp_get_wtime();

    if (modo == 1) {                         /* rand() global */
        srand(base);
        #pragma omp parallel for reduction(+:dentro)
        for (long i = 0; i < N; i++) {
            double x = rand() / (double)RAND_MAX;
            double y = rand() / (double)RAND_MAX;
            if (x * x + y * y <= 1.0) dentro++;
        }
    } else if (modo == 2) {                  /* rand_r sobre estado compartido */
        unsigned int compartido = base;
        #pragma omp parallel for reduction(+:dentro)
        for (long i = 0; i < N; i++) {
            double x = rand_r(&compartido) / (double)RAND_MAX;
            double y = rand_r(&compartido) / (double)RAND_MAX;
            if (x * x + y * y <= 1.0) dentro++;
        }
    } else if (modo == 3) {                  /* misma semilla en todos los hilos */
        #pragma omp parallel reduction(+:dentro)
        {
            unsigned int estado = base;      /* privado, pero igual para todos */
            #pragma omp for
            for (long i = 0; i < N; i++) {
                double x = rand_r(&estado) / (double)RAND_MAX;
                double y = rand_r(&estado) / (double)RAND_MAX;
                if (x * x + y * y <= 1.0) dentro++;
            }
        }
    } else if (modo == 4) {                  /* sin reduction: carrera sobre dentro */
        #pragma omp parallel
        {
            unsigned int estado = base + 7919u * (unsigned int)omp_get_thread_num();
            #pragma omp for
            for (long i = 0; i < N; i++) {
                double x = rand_r(&estado) / (double)RAND_MAX;
                double y = rand_r(&estado) / (double)RAND_MAX;
                if (x * x + y * y <= 1.0) dentro++;   /* lectura-modificacion-escritura no atomica */
            }
        }
    }
    double t1 = omp_get_wtime();
    double pi_est = 4.0 * dentro / (double)N;
    printf("modo %d | p = %d | aciertos = %ld | pi = %.6f | error = %.2e | tiempo = %.3f s\n",
           modo, omp_get_max_threads(), dentro, pi_est, fabs(pi_est - M_PI), t1 - t0);
    return 0;
}
