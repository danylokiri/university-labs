#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
mkdir -p results
LOG="results/run.log"; : > "$LOG"
echo "alg,N,P,seq_t,par_t,S,E,max_diff" > results/matmul.csv

# Для стрічкового — P ∈ {1,2,4,8} — N кратні усім цим P.
# Для блокових (fox, cannon) — P ∈ {1,4,16} (квадрати), N кратне sqrt(P).

SIZES=(128 256 384 512 768)

# ---- стрічковий ----
for N in "${SIZES[@]}"; do
  for P in 1 2 4 8; do
    {
      echo "=== ribbon N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/matmul_ribbon "$N"
    } | tee -a "$LOG" >> /tmp/_r$$
    tail -1 /tmp/_r$$ >> results/matmul.csv
    : > /tmp/_r$$
  done
done

# ---- Фокса ----
for N in "${SIZES[@]}"; do
  for P in 1 4 16; do
    q=$(python3 -c "import math; print(int(math.sqrt($P)))")
    if [ $((N % q)) -ne 0 ]; then continue; fi
    {
      echo "=== fox N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/matmul_fox "$N"
    } | tee -a "$LOG" >> /tmp/_f$$
    tail -1 /tmp/_f$$ >> results/matmul.csv
    : > /tmp/_f$$
  done
done

# ---- Кенона ----
for N in "${SIZES[@]}"; do
  for P in 1 4 16; do
    q=$(python3 -c "import math; print(int(math.sqrt($P)))")
    if [ $((N % q)) -ne 0 ]; then continue; fi
    {
      echo "=== cannon N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/matmul_cannon "$N"
    } | tee -a "$LOG" >> /tmp/_c$$
    tail -1 /tmp/_c$$ >> results/matmul.csv
    : > /tmp/_c$$
  done
done

rm -f /tmp/_r$$ /tmp/_f$$ /tmp/_c$$
echo "DONE"
