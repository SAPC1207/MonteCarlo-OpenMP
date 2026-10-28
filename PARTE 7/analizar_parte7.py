"""analizar_parte7.py -- tabla de speedup/eficiencia, tabla de 7.3 y grafica speedup_parte7.png."""
import json, re
m72 = [l.split() for l in open("med_72.txt") if l.strip()]
T = {int(x[0]): float(x[1]) for x in m72}
T1 = T[1]
print("p | Tp mediano (s) | S = T1/Tp | E = S/p")
for p in sorted(T):
    print(f"{p:>2} | {T[p]:.6f} | {T1/T[p]:.2f} | {T1/T[p]/p:.2f}")
m73 = [l.split() for l in open("med_73.txt") if l.strip()]
print("\nN | T 1 hilo | T 8 hilos | speedup | error (1 hilo)")
f73 = []
for x in m73:
    n, t1, t8, err = int(x[0]), float(x[2]), float(x[6]), x[4]
    print(f"{n:>9} | {t1:.6f} | {t8:.6f} | {t1/t8:.2f} | {err}")
    f73.append({"N": n, "t1": t1, "t8": t8, "err": err})
costo = {int(a): float(b) for a, b in re.findall(r"(\d+) hilos: ([\d.]+) us", open("costo.txt").read())}
json.dump({"m72": m72, "f73": f73, "costo": costo}, open("datos_parte7.json", "w"), indent=1)
try:
    import matplotlib; matplotlib.use("Agg"); import matplotlib.pyplot as plt
    ps = sorted(T); S = [T1 / T[p] for p in ps]
    fig, ax = plt.subplots(figsize=(6, 4), dpi=150)
    ax.plot([1, max(ps)], [1, max(ps)], "--", color="gray", label="Ideal S = p")
    ax.plot(ps, S, "o-", color="tab:blue", label="Medido (mediana de 5)")
    for p, s in zip(ps, S):
        ax.annotate(f"{s:.2f}", (p, s), textcoords="offset points", xytext=(0, 8), ha="center", fontsize=8)
    ax.set_xlabel("Número de hilos p"); ax.set_ylabel("Speedup S = T1 / Tp")
    ax.set_title("Speedup del cálculo de π (N = 2·10^8)"); ax.set_xticks(ps); ax.grid(alpha=.3); ax.legend()
    fig.tight_layout(); fig.savefig("speedup_parte7.png"); print("\nGrafica: speedup_parte7.png")
except ImportError:
    print("Falta matplotlib: sudo apt install -y python3-matplotlib")
