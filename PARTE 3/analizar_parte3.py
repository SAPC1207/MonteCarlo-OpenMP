"""analizar_parte3.py -- calcula medianas, speedup, eficiencia, f de Amdahl
y metrica de Karp-Flatt a partir de tiempos_crudos.csv, y dibuja speedup.png."""
import csv, statistics
from collections import defaultdict

tiempos, aciertos = defaultdict(list), set()
with open("tiempos_crudos.csv") as fh:
    for fila in csv.DictReader(fh):
        tiempos[int(fila["p"])].append(float(fila["tiempo"]))
        aciertos.add(fila["aciertos"])
ps = sorted(tiempos)
med = {p: statistics.median(tiempos[p]) for p in ps}
prom = {p: statistics.mean(tiempos[p]) for p in ps}
T1 = med[1]
S = {p: T1 / med[p] for p in ps}
E = {p: S[p] / p for p in ps}

print("Aciertos distintos entre todas las ejecuciones:", aciertos, "(debe ser uno solo)")
print()
print(f"{'p':>3} | {'min':>7} {'max':>7} {'promedio':>8} {'MEDIANA':>8} | {'S(p)':>6} {'E(p)':>6} | {'Karp-Flatt e':>12}")
for p in ps:
    kf = (1/S[p] - 1/p) / (1 - 1/p) if p > 1 else float('nan')
    print(f"{p:>3} | {min(tiempos[p]):7.4f} {max(tiempos[p]):7.4f} {prom[p]:8.4f} {med[p]:8.4f} | "
          f"{S[p]:6.3f} {E[p]:6.3f} | {kf:12.4f}")

# f despejado de Amdahl con p = 4:  1/S = (1-f) + f/p  =>  f = (1 - 1/S) / (1 - 1/p)
p0 = 4
f = (1 - 1/S[p0]) / (1 - 1/p0)
amdahl = lambda p: 1 / ((1 - f) + f / p)
print()
print(f"f estimado con p = {p0}: f = (1 - 1/{S[p0]:.4f}) / (1 - 1/{p0}) = {f:.4f}  -> fraccion secuencial 1-f = {1-f:.4f}")
print("Prediccion de Amdahl con ese f:")
for p in ps + [64]:
    medido = f"{S[p]:.3f}" if p in S else "  -  "
    print(f"   p = {p:>2}: Amdahl = {amdahl(p):6.3f} | medido = {medido} | ideal = {p}")
print(f"   p -> infinito: S_max = 1/(1-f) = {1/(1-f):.2f}")

try:
    hib = defaultdict(list)
    with open("hibrido.csv") as fh:
        for fila in csv.DictReader(fh):
            hib[(fila["planificacion"], int(fila["p"]))].append(float(fila["tiempo"]))
    print("\nstatic vs dynamic,65536 (mediana de 5):")
    for p in (8, 12):
        ts, td = statistics.median(hib[("static", p)]), statistics.median(hib[("dynamic65536", p)])
        print(f"   p = {p:>2}: static = {ts:.4f} s (S = {T1/ts:.3f}) | dynamic,65536 = {td:.4f} s (S = {T1/td:.3f})")
except (FileNotFoundError, statistics.StatisticsError):
    pass

try:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    xs = list(range(1, max(ps) + 1))
    fig, ax = plt.subplots(figsize=(6, 4), dpi=150)
    ax.plot(xs, xs, "--", color="gray", label="Ideal lineal S(p) = p")
    ax.plot(xs, [amdahl(x) for x in xs], ":", color="tab:orange", label=f"Amdahl con f = {f:.3f}")
    ax.plot(ps, [S[p] for p in ps], "o-", color="tab:blue", label="Medido (mediana)")
    for p in ps:
        ax.annotate(f"{S[p]:.2f}", (p, S[p]), textcoords="offset points", xytext=(0, 8), ha="center", fontsize=8)
    ax.set_xlabel("Número de hilos p"); ax.set_ylabel("Speedup S(p) = T(1)/T(p)")
    ax.set_title("Speedup de Monte Carlo con OpenMP (N = 10^8)")
    ax.set_xticks(ps); ax.grid(alpha=0.3); ax.legend()
    fig.tight_layout(); fig.savefig("speedup.png")
    print("\nGrafica guardada en speedup.png")
except ImportError:
    print("\nFalta matplotlib: sudo apt install -y python3-matplotlib  y luego  python3 analizar_parte3.py")

# resumen en JSON (lo usa el informe)
import json
res = {"ps": ps, "med": med, "S": S, "E": E, "f": f, "T1": T1,
       "min": {p: min(tiempos[p]) for p in ps}, "max": {p: max(tiempos[p]) for p in ps},
       "prom": prom, "reps": len(tiempos[1])}
try:
    res["hib"] = {f"{k[0]}_{k[1]}": statistics.median(v) for k, v in hib.items()}
except NameError:
    pass
with open("resumen.json", "w") as fh:
    json.dump(res, fh, indent=1)
