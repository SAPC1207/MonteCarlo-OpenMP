#!/bin/bash
# ejecutar_parte6.sh -- compila y ejecuta todo lo de la Parte 6.
# Uso: bash ejecutar_parte6.sh   -> guarda todo en resultados_parte6.txt
exec > >(tee resultados_parte6.txt) 2>&1
echo "===== Parte 6 - Condiciones de carrera y reduction - $(date) ====="
echo "Compilador : $(gcc --version | head -1)"
echo "nproc      : $(nproc)"
echo "CPU        : $(lscpu | grep 'Model name' | sed 's/.*: *//')"
echo

echo "----- Compilacion con -Wall -fopenmp -----"
gcc -Wall -fopenmp carrera.c -o carrera
gcc -Wall -fopenmp carrera_reduction.c -o carrera_reduction
gcc -Wall -fopenmp reducciones.c -o reducciones -lm
gcc -Wall -fopenmp armonica.c -o armonica
echo "[fin de la compilacion]"
echo "Prueba: reducciones.c SIN -lm (para ver el error de enlazado):"
gcc -Wall -fopenmp reducciones.c -o reducciones_sin_lm 2>&1 | grep -i "undefined\|error" | head -3
rm -f reducciones_sin_lm
echo

echo "----- 6.1 carrera.c -----"
echo -n "1 hilo  : "; OMP_NUM_THREADS=1 ./carrera
echo "4 hilos, 5 ejecuciones:"
for r in 1 2 3 4 5; do OMP_NUM_THREADS=4 ./carrera; done
echo "12 hilos, 5 ejecuciones:"
for r in 1 2 3 4 5; do OMP_NUM_THREADS=12 ./carrera; done
echo

echo "----- 6.2 carrera_reduction.c -----"
echo "4 hilos, 5 ejecuciones:"
for r in 1 2 3 4 5; do OMP_NUM_THREADS=4 ./carrera_reduction; done
echo "12 hilos, 5 ejecuciones:"
for r in 1 2 3 4 5; do OMP_NUM_THREADS=12 ./carrera_reduction; done
echo

echo "----- 6.3 reducciones.c -----"
OMP_NUM_THREADS=4 ./reducciones
OMP_NUM_THREADS=12 ./reducciones
echo

echo "----- 6.4 armonica.c con 1, 2, 3, 4, 8 y 12 hilos -----"
for p in 1 2 3 4 8 12; do OMP_NUM_THREADS=$p ./armonica; done
echo "Repeticion con 4 hilos (3 veces):"
for r in 1 2 3; do OMP_NUM_THREADS=4 ./armonica; done
echo
echo "===== Fin. Resultados en resultados_parte6.txt ====="
