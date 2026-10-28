#!/bin/bash
# medir_parte7.sh -- todas las mediciones de la Parte 7.
# Uso: bash medir_parte7.sh   -> resultados_parte7.txt, med_72.txt, med_73.txt, speedup_parte7.png
exec > >(tee resultados_parte7.txt) 2>&1
echo "===== Parte 7 - pi y medicion de rendimiento - $(date) ====="
echo "Compilador : $(gcc --version | head -1)"
echo "nproc      : $(nproc)"
echo "CPU        : $(lscpu | grep 'Model name' | sed 's/.*: *//')"
gcc -O2 -fopenmp pi.c -o pi -lm || exit 1
gcc -O2 -fopenmp costo.c -o costo || exit 1
echo "Calentamiento..."; OMP_NUM_THREADS=12 ./pi > /dev/null; ./pi > /dev/null
echo

echo "----- 7.1 Medir bien el tiempo (hilos, pared, CPU, error) -----"
for p in 1 2 4 8; do OMP_NUM_THREADS=$p ./pi; done
echo

echo "----- 7.2 Speedup: mediana de 5 ejecuciones (N = 2e8) -----"
: > med_72.txt
for p in 1 2 4 8 12 16; do
  for r in 1 2 3 4 5; do OMP_NUM_THREADS=$p ./pi; done | sort -n -k2 | sed -n 3p | tee -a med_72.txt
done
echo

echo "----- 7.3 Cuando vale la pena: mediana de 5, 1 hilo vs 8 hilos -----"
: > med_73.txt
for n in 1000 10000 100000 1000000 10000000 100000000; do
  t1=$(for r in 1 2 3 4 5; do OMP_NUM_THREADS=1 ./pi $n; done | sort -n -k2 | sed -n 3p)
  t8=$(for r in 1 2 3 4 5; do OMP_NUM_THREADS=8 ./pi $n; done | sort -n -k2 | sed -n 3p)
  echo "N = $n | 1 hilo: $t1 | 8 hilos: $t8"
  echo "$n $t1 $t8" >> med_73.txt
done
echo

echo "----- Costo de fork-join en este equipo (costo.c, Parte 2) -----"
for p in 1 2 4 8 12; do OMP_NUM_THREADS=$p ./costo; done | tee costo.txt
echo

python3 analizar_parte7.py
echo "===== Fin. Resultados en resultados_parte7.txt ====="
