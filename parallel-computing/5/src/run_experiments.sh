#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
mkdir -p results
LOG="results/run.log"; : > "$LOG"
echo "N,P,seq_t,par_t,S,E,residual,diff" > results/gauss.csv

SIZES=(128 256 512 768 1024)
PROCS=(1 2 4 8)

for N in "${SIZES[@]}"; do
  for P in "${PROCS[@]}"; do
    {
      echo "=== N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/gauss_mpi "$N"
    } | tee -a "$LOG" >> /tmp/_g$$
    tail -1 /tmp/_g$$ >> results/gauss.csv
    : > /tmp/_g$$
  done
done
rm -f /tmp/_g$$
echo "DONE"
