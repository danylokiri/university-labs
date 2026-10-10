#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
mkdir -p results
LOG="results/run.log"; : > "$LOG"
echo "method,n,P,seq_t,par_t,S,E,I_par,err" > results/integral.csv

SIZES=(1000000 5000000 10000000 50000000 100000000)
PROCS=(1 2 4 8)

for method in 0 1; do
  for N in "${SIZES[@]}"; do
    for P in "${PROCS[@]}"; do
      {
        echo "=== method=$method n=$N P=$P ==="
        mpirun --oversubscribe -np "$P" build/integral_mpi "$N" "$method"
      } | tee -a "$LOG" >> /tmp/_i$$
      tail -1 /tmp/_i$$ >> results/integral.csv
      : > /tmp/_i$$
    done
  done
done
rm -f /tmp/_i$$
echo "DONE"
