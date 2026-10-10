#!/usr/bin/env bash
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 1

SUMMARY="results/summary.csv"
LOG_SEQ="results/seq_runs.csv"
LOG_MPI="results/mpi_runs.csv"

SIZES=(840 960 1200)
P_LIST=(1 4 9 16)

for N in "${SIZES[@]}"; do
    echo ">>> seq  N=$N"
    REF="results/ref_${N}.bin"
    OUT=$(./build/fox_seq -n "$N" --csv --save "$REF")
    echo "$OUT"
    echo "$OUT" >> "$LOG_SEQ"
    T1=$(echo "$OUT" | awk -F, '{print $2}')
    for P in "${P_LIST[@]}"; do
        Q=$(python3 -c "import math;print(int(round(math.sqrt($P))))")
        if [ $((Q*Q)) -ne "$P" ]; then continue; fi
        if [ $((N % Q)) -ne 0 ]; then continue; fi
        echo ">>> mpi  N=$N P=$P q=$Q"
        OUT=$(mpirun --oversubscribe -np "$P" ./build/fox_mpi -n "$N" --check "$REF" --csv)
        echo "$OUT"
        echo "$OUT" >> "$LOG_MPI"
        TP=$(echo "$OUT" | awk -F, '{print $4}')
        MD=$(echo "$OUT" | awk -F, '{print $9}')
        S=$(python3 -c "print(float('$T1')/float('$TP'))")
        E=$(python3 -c "print(float('$S')/$P)")
        printf "%s,%s,%s,%s,%s,%.6f,%.6f,%s\n" \
            "$N" "$P" "$Q" "$T1" "$TP" "$S" "$E" "$MD" >> "$SUMMARY"
    done
done
