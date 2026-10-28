#!/bin/bash
# ejecutar_parte2.sh -- compila y ejecuta todo lo de la Parte II.
# Uso: bash ejecutar_parte2.sh            (usa la semilla 20261018)
#      bash ejecutar_parte2.sh 12345678   (usa la semilla asignada por el docente)
# Todo queda guardado en resultados_parte2.txt
SEMILLA=${1:-20261018}
N=100000000
P=4                       # hilos para las pruebas de errores y planificacion
SALIDA=resultados_parte2.txt
exec > >(tee "$SALIDA") 2>&1

echo "===== Parte II - $(date) - semilla base: $SEMILLA ====="
echo "Nucleos logicos (nproc): $(nproc)"
echo
echo "----- Compilacion (no debe aparecer ninguna advertencia) -----"
set -e
gcc -O2 -fopenmp -Wall -Wextra -DSEMILLA_BASE=${SEMILLA}u montecarlo_secuencial.c -o montecarlo_secuencial -lm
gcc -O2 -fopenmp -Wall -Wextra -DSEMILLA_BASE=${SEMILLA}u montecarlo.c -o montecarlo -lm
gcc -O2 -fopenmp -Wall -Wextra errores.c -o errores -lm
gcc -O2 -fopenmp -DSEMILLA_BASE=${SEMILLA}u -DDETALLE montecarlo.c -o montecarlo_detalle -lm
gcc -O2 -fopenmp -DSEMILLA_BASE=${SEMILLA}u -DPLANIFICACION=dynamic montecarlo.c -o mc_dynamic1 -lm
gcc -O2 -fopenmp -DSEMILLA_BASE=${SEMILLA}u -DPLANIFICACION="dynamic,65536" montecarlo.c -o mc_dynamic65536 -lm
set +e
echo "OK"
echo

echo "----- II.1 Version secuencial, N = 1e8, 5 ejecuciones -----"
for k in 1 2 3 4 5; do ./montecarlo_secuencial $N; echo; done | tee /tmp/seq.txt
echo -n "Mediana de T(1): "
grep "Tiempo" /tmp/seq.txt | awk '{print $4}' | sort -n | sed -n 3p
echo

echo "----- II.2 Version paralela: reproducibilidad con distinto p -----"
for p in 1 2 4 8 $(nproc); do ./montecarlo $N $p; done
echo
echo "Tambien con OMP_NUM_THREADS en lugar del argumento:"
OMP_NUM_THREADS=3 ./montecarlo $N
echo
echo "Inicio de cada hilo (N = 1000, p = 4): separacion de N/p muestras"
./montecarlo_detalle 1000 4
echo

echo "----- II.3 / II.4 Versiones incorrectas (p = $P), 3 ejecuciones cada una -----"
for m in 1 2 3 4; do
  for k in 1 2 3; do ./errores $m $N $P; done
  echo
done
echo "Referencia para el modo 3: 1 hilo con N/$P muestras"
./errores 3 $((N / P)) 1
echo

echo "----- II.5 Planificacion (p = $P), 3 ejecuciones cada una -----"
echo "schedule(static):";          for k in 1 2 3; do ./montecarlo $N $P; done
echo "schedule(dynamic) [chunk 1]:"; for k in 1 2 3; do ./mc_dynamic1 $N $P; done
echo "schedule(dynamic,65536):";   for k in 1 2 3; do ./mc_dynamic65536 $N $P; done
echo
echo "===== Fin. Resultados guardados en $SALIDA ====="
