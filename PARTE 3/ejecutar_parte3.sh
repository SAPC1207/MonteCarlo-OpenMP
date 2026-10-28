#!/bin/bash
# ejecutar_parte3.sh -- mide T(p) para la Parte III y genera tabla y grafica.
# Uso: bash ejecutar_parte3.sh            (semilla 20261018)
#      bash ejecutar_parte3.sh 12345678   (otra semilla)
# Salidas: tiempos_crudos.csv, hibrido.csv, lscpu.txt, resultados_parte3.txt, speedup.png
SEMILLA=${1:-20261018}
N=${N:-100000000}
REPS=7                        # al menos 5 (pide el enunciado); uso 7
HILOS="1 2 4 8 12"            # 12 = numero de CPU logicas de mi equipo
exec > >(tee resultados_parte3.txt) 2>&1

echo "===== Parte III - $(date) - N = $N - semilla $SEMILLA ====="
lscpu > lscpu.txt
echo "CPU: $(grep 'Model name' lscpu.txt | sed 's/.*: *//') | nproc = $(nproc)"

set -e
gcc -O2 -fopenmp -Wall -Wextra -DSEMILLA_BASE=${SEMILLA}u montecarlo.c -o montecarlo -lm
gcc -O2 -fopenmp -Wall -Wextra -DSEMILLA_BASE=${SEMILLA}u -DPLANIFICACION="dynamic,65536" montecarlo.c -o mc_dynamic -lm
set +e

echo "Calentamiento (no se registra)..."
./montecarlo $N 12 > /dev/null; ./montecarlo $N 1 > /dev/null

# Rondas intercaladas: en cada repeticion se mide cada p una vez, para que
# cambios de temperatura/frecuencia afecten por igual a todos los p.
echo "rep,p,tiempo,aciertos" > tiempos_crudos.csv
for r in $(seq 1 $REPS); do
  for p in $HILOS; do
    linea=$(./montecarlo $N $p)
    t=$(echo "$linea" | sed 's/.*tiempo = \([0-9.]*\) s.*/\1/')
    a=$(echo "$linea" | sed 's/.*aciertos = \([0-9]*\).*/\1/')
    echo "$r,$p,$t,$a" >> tiempos_crudos.csv
    echo "rep $r  p = $p  T = $t s  aciertos = $a"
  done
done

# Prueba extra para III.3: static vs dynamic,65536 con nucleos hibridos (P + E)
echo "planificacion,p,tiempo" > hibrido.csv
for r in $(seq 1 5); do
  for p in 8 12; do
    for prog in montecarlo mc_dynamic; do
      t=$(./$prog $N $p | sed 's/.*tiempo = \([0-9.]*\) s.*/\1/')
      [ "$prog" = montecarlo ] && nom=static || nom=dynamic65536
      echo "$nom,$p,$t" >> hibrido.csv
    done
  done
done
echo "Prueba static vs dynamic terminada."
echo
python3 analizar_parte3.py
echo "===== Fin ====="
