#!/bin/bash
# ejecutar_parte5.sh -- compila y ejecuta todo lo de la Parte 5.
# Uso: bash ejecutar_parte5.sh      -> guarda todo en resultados_parte5.txt
exec > >(tee resultados_parte5.txt) 2>&1

echo "===== Parte 5 - Dependencias entre iteraciones - $(date) ====="
echo "Compilador : $(gcc --version | head -1)"
echo "_OPENMP    : $(echo | cpp -fopenmp -dM | grep -i _OPENMP | awk '{print $3}')"
echo "nproc      : $(nproc)"
echo "CPU        : $(lscpu | grep 'Model name' | sed 's/.*: *//')"
echo

echo "----- Compilacion con -Wall -fopenmp (si no aparece nada, no hubo avisos) -----"
gcc -Wall -fopenmp prefijo.c -o prefijo
gcc -Wall -fopenmp clasificar.c -o clasificar
gcc -Wall -fopenmp prefijo_scan.c -o prefijo_scan
echo "[fin de la compilacion]"
echo

echo "----- 5.1 prefijo.c con 1, 2, 4, 8 y 12 hilos -----"
for p in 1 2 4 8 12; do
  echo -n "p = $p : "; OMP_NUM_THREADS=$p ./prefijo
done
echo
echo "Repeticion con 4 hilos (3 veces) para ver si cambia:"
for r in 1 2 3; do OMP_NUM_THREADS=4 ./prefijo; done
echo

echo "----- 5.2 clasificar.c, 3 ejecuciones con 4 hilos -----"
for r in 1 2 3; do OMP_NUM_THREADS=4 ./clasificar; echo "---"; done
echo
echo "----- 5.2 clasificar.c con 12 hilos (para comparar) -----"
OMP_NUM_THREADS=12 ./clasificar
echo

echo "----- EXTRA: suma prefijo correcta con '#pragma omp scan' -----"
for p in 1 2 4 8 12; do OMP_NUM_THREADS=$p ./prefijo_scan; done
echo
echo "===== Fin. Resultados en resultados_parte5.txt ====="
