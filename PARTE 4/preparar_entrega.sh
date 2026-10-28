#!/bin/bash
# preparar_entrega.sh -- deja el repositorio listo para entregar.
# Se ejecuta desde ~/montecarlo/parte4 (o desde cualquier lado: usa rutas absolutas).
REPO=~/montecarlo
DESTINO="/mnt/d/USER/Desktop/PARCIAL MONTE CARLO/CUARTA PARTE"
cd "$REPO" || { echo "No existe $REPO"; exit 1; }

echo "== 1. montecarlo.c final en la raiz del repositorio =="
cp parte3/montecarlo.c ./montecarlo.c

echo "== 2. Compilacion exacta que pide el enunciado (debe quedar vacia = sin advertencias) =="
gcc -O2 -fopenmp montecarlo.c -o montecarlo -lm 2> compilacion.txt
gcc -O2 -fopenmp -Wall -Wextra montecarlo.c -o montecarlo -lm 2>> compilacion.txt
if [ -s compilacion.txt ]; then echo "HAY ADVERTENCIAS:"; cat compilacion.txt; else echo "OK: sin advertencias"; fi
rm -f compilacion.txt

echo "== 3. Prueba rapida =="
./montecarlo 100000000 4

echo "== 4. .gitignore para no subir ejecutables =="
cat > .gitignore <<'EOG'
montecarlo
montecarlo_secuencial
montecarlo_detalle
errores
error_vs_n
mc_*
*.o
EOG

echo "== 5. Commit final =="
git add .gitignore montecarlo.c README.md parte4/ 2>/dev/null
git add -A
git commit -m "Parte IV: informe del proceso, bitacora de IA y montecarlo.c final en la raiz"
echo
git log --oneline

echo "== 6. Comprimido del repositorio (incluye el historial .git) =="
command -v zip >/dev/null || sudo apt install -y zip
cd ~
rm -f entrega_montecarlo.zip
zip -rq entrega_montecarlo.zip montecarlo -x "montecarlo/montecarlo" "montecarlo/*/montecarlo" \
    "montecarlo/*/montecarlo_secuencial" "montecarlo/*/errores" "montecarlo/*/mc_*" \
    "montecarlo/*/montecarlo_detalle" "montecarlo/error_vs_n" "montecarlo/*/error_vs_n"
cp entrega_montecarlo.zip "$DESTINO/"
echo "Listo: entrega_montecarlo.zip copiado en $DESTINO"
