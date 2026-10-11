#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
mkdir -p results
LOG="results/run.log"; : > "$LOG"
echo "alg,N,P,seq_t,par_t,S,E,verify" > results/sort.csv

# quick sort (варіант 15, базовий N = 10000) + більші розміри
QSIZES=(10000 50000 100000 500000 1000000)
# odd-even (варіант 15, базовий N = 15000) + інші
ESIZES=(15000 50000 100000 500000)

PROCS=(1 2 4 8)

for N in "${QSIZES[@]}"; do
  for P in "${PROCS[@]}"; do
    {
      echo "=== quick N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/sort_mpi "$N" 2
    } | tee -a "$LOG" >> /tmp/_q$$
    tail -1 /tmp/_q$$ >> results/sort.csv
    : > /tmp/_q$$
  done
done

for N in "${ESIZES[@]}"; do
  for P in "${PROCS[@]}"; do
    {
      echo "=== oddeven N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/sort_mpi "$N" 3
    } | tee -a "$LOG" >> /tmp/_o$$
    tail -1 /tmp/_o$$ >> results/sort.csv
    : > /tmp/_o$$
  done
done
rm -f /tmp/_q$$ /tmp/_o$$
echo "DONE"
