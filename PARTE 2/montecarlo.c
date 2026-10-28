/* montecarlo.c -- estimacion de pi por Monte Carlo (acierto y fallo) con OpenMP.
   Autor: Sergio Andres Pinzon Castellanos - Introduccion a la computacion paralela (28661), UIS

   Compilar: gcc -O2 -fopenmp montecarlo.c -o montecarlo -lm
   Uso:      ./montecarlo [N] [p]
             N = numero de muestras (por defecto 1e8)
             p = numero de hilos (si no se da, se usa OMP_NUM_THREADS)

   Idea del manejo de semillas:
   rand_r de glibc es un generador lineal congruencial (LCG) de 32 bits:
   cada llamada aplica 3 veces  s <- 1103515245*s + 12345 (mod 2^32),
   asi que cada muestra (x, y) consume exactamente 6 pasos del LCG.
   La muestra i usa entonces los pasos [6i, 6i+6) de UNA sola secuencia que
   empieza en SEMILLA_BASE. Cuando un hilo empieza un bloque de iteraciones
   en la muestra i, "salta" directamente al estado 6i con salto_lcg() en
   O(log i) operaciones. Asi:
     - cada hilo tiene su propio estado (privado) -> sin condicion de carrera;
     - los bloques de los hilos son disjuntos -> los flujos no se solapan y la
       semilla de cada hilo queda separada exactamente N/p muestras de la del
       siguiente (con schedule(static));
     - el conjunto de muestras es el mismo para cualquier p -> la estimacion
       es identica bit a bit a la secuencial, sea cual sea el numero de hilos. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#ifndef SEMILLA_BASE
#define SEMILLA_BASE 20261018u     /* <-- reemplazar por la semilla asignada por el docente */
#endif

#ifndef PLANIFICACION
#define PLANIFICACION static       /* para II.5: -DPLANIFICACION="dynamic,65536" */
#endif

#define LCG_A 1103515245u
#define LCG_C 12345u

/* Devuelve el estado del LCG despues de avanzar k pasos desde 'semilla'.
   Usa exponenciacion binaria: (A, C) se compone consigo mismo en cada bit de k. */
static unsigned int salto_lcg(unsigned int semilla, unsigned long k) {
    unsigned int a = LCG_A, c = LCG_C;      /* transformacion de 2^j pasos  */
    unsigned int a_tot = 1u, c_tot = 0u;    /* transformacion acumulada      */
    while (k > 0) {
        if (k & 1ul) {
            a_tot = a_tot * a;
            c_tot = c_tot * a + c;
        }
        c = c * a + c;                       /* duplicar: f(f(s)) */
        a = a * a;
        k >>= 1;
    }
    return a_tot * semilla + c_tot;          /* aritmetica mod 2^32 automatica */
}

int main(int argc, char *argv[]) {
    long N = (argc > 1) ? atol(argv[1]) : 100000000L;
    if (argc > 2) omp_set_num_threads(atoi(argv[2]));

    long dentro = 0;
    unsigned int estado = SEMILLA_BASE;  /* copia privada por hilo (firstprivate) */
    long siguiente = -1;                 /* proxima muestra que el hilo espera     */
    int p = 1;

    double t0 = omp_get_wtime();
    #pragma omp parallel for schedule(PLANIFICACION) \
            firstprivate(estado, siguiente) reduction(+:dentro)
    for (long i = 0; i < N; i++) {
        if (i != siguiente) {            /* inicio de un bloque nuevo para este hilo */
            estado = salto_lcg(SEMILLA_BASE, 6ul * (unsigned long)i);
#ifdef DETALLE
            printf("  hilo %d empieza en la muestra %ld (estado %u)\n",
                   omp_get_thread_num(), i, estado);
#endif
        }
        siguiente = i + 1;
        if (i == 0) p = omp_get_num_threads();

        double x = rand_r(&estado) / (double)RAND_MAX;
        double y = rand_r(&estado) / (double)RAND_MAX;
        if (x * x + y * y <= 1.0)
            dentro++;
    }
    double t1 = omp_get_wtime();

    double pi_est = 4.0 * dentro / (double)N;
    printf("p = %2d | aciertos = %ld | pi = %.10f | error = %.2e | tiempo = %.4f s\n",
           p, dentro, pi_est, fabs(pi_est - M_PI), t1 - t0);
    return 0;
}
