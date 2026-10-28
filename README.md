# Integración por Monte Carlo con OpenMP

Sergio Andrés Pinzón Castellanos — Introducción a la computación paralela (28661), UIS.

Estimación de π por el método de acierto y fallo, paralelizada con OpenMP.

## Compilar y ejecutar (Linux / WSL)

```bash
gcc -O2 -fopenmp montecarlo.c -o montecarlo -lm
./montecarlo 100000000 4          # N muestras, p hilos
OMP_NUM_THREADS=8 ./montecarlo    # o elegir p con la variable de entorno
```

El resultado es idéntico para cualquier p (78 540 833 aciertos con la semilla 20261018),
porque cada hilo salta con `salto_lcg()` al punto exacto de la secuencia de `rand_r`
donde empieza su bloque de muestras.

## Estructura

| Carpeta | Contenido |
|---|---|
| `montecarlo.c` | Versión paralela final (entregable) |
| `error_vs_n.c`, `resultados_parte1.txt` | Parte I: verificación de la convergencia 1/√N |
| `parte2/` | Versión secuencial con tiempo, versión paralela, `errores.c` (versiones incorrectas a propósito) y `ejecutar_parte2.sh` |
| `parte3/` | `ejecutar_parte3.sh` (mediciones), `analizar_parte3.py` (speedup, Amdahl, Karp–Flatt), datos crudos, `speedup.png`, `lscpu.txt` |
| `parte4/` | Informe del proceso con la bitácora de IA (PDF) |

Equipo de medición: Intel Core i5-1235U (2 núcleos P + 8 núcleos E, 12 hilos), Ubuntu sobre WSL, GCC 13.
