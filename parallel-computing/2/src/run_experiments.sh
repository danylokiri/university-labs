#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
mkdir -p results
echo "N,P,seq_t,par_t,S,E,verify" > results/minmax.csv
echo "N,P,seq_t,par_t,S,E,abs_err" > results/average.csv

MLOG="results/minmax_run.log";  : > "$MLOG"
ALOG="results/average_run.log"; : > "$ALOG"

SIZES=(500000 1000000 2000000 5000000 10000000)
PROCS=(1 2 4 8)

for N in "${SIZES[@]}"; do
  for P in "${PROCS[@]}"; do
    {
      echo "=== minmax N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/minmax_mpi "$N" 5
    } | tee -a "$MLOG" >> /tmp/_m$$
    tail -1 /tmp/_m$$ >> results/minmax.csv
    : > /tmp/_m$$

    {
      echo "=== average N=$N P=$P ==="
      mpirun --oversubscribe -np "$P" build/average_mpi "$N" 5
    } | tee -a "$ALOG" >> /tmp/_a$$
    tail -1 /tmp/_a$$ >> results/average.csv
    : > /tmp/_a$$
  done
done
rm -f /tmp/_m$$ /tmp/_a$$
echo "DONE"
