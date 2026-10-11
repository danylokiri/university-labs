#!/usr/bin/env bash
set -u
# Серія експериментів: фіксуємо T1 (послідовний) і Tp (MPI) для декількох (N, P).
# Для Фокса P має бути повним квадратом і N має бути кратним q=sqrt(P).
# Обираємо P = {1, 4, 9, 16} і набір N кратний lcm(1,2,3,4)=12 та досить великих
# для того, щоб обчислення були значущими.

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT" || exit 1
mkdir -p results

SUMMARY="results/summary.csv"
LOG_SEQ="results/seq_runs.csv"
LOG_MPI="results/mpi_runs.csv"

: > "$SUMMARY"
: > "$LOG_SEQ"
: > "$LOG_MPI"
echo "N,P,q,T1,Tp,S,E,max_diff" > "$SUMMARY"
echo "N,T1,checksum" > "$LOG_SEQ"
echo "N,P,q,Ttotal,Tcompute,Tscatter,Tgather,checksum,max_diff" > "$LOG_MPI"

SIZES=(240 360 480 600 720)
P_LIST=(1 4 9 16)

# 1. Для кожного N рахуємо T1 послідовною програмою
declare -A T1_MAP
for N in "${SIZES[@]}"; do
    echo ">>> seq  N=$N"
    REF="results/ref_${N}.bin"
    OUT=$(./build/fox_seq -n "$N" --csv --save "$REF")
    echo "$OUT"
    # N,T1,checksum
    echo "$OUT" >> "$LOG_SEQ"
    T1=$(echo "$OUT" | awk -F, '{print $2}')
    T1_MAP[$N]="$T1"
done

# 2. Для кожного (N, P) запускаємо Fox MPI і порівнюємо з еталоном
for N in "${SIZES[@]}"; do
    REF="results/ref_${N}.bin"
    T1="${T1_MAP[$N]}"
    for P in "${P_LIST[@]}"; do
        Q=$(python3 -c "import math;print(int(round(math.sqrt($P))))")
        if [ $((Q*Q)) -ne "$P" ]; then
            echo "skip P=$P (not square)"
            continue
        fi
        if [ $((N % Q)) -ne 0 ]; then
            echo "skip N=$N P=$P (N not divisible by q=$Q)"
            continue
        fi
        echo ">>> mpi  N=$N P=$P q=$Q"
        OUT=$(mpirun --oversubscribe -np "$P" ./build/fox_mpi -n "$N" --check "$REF" --csv)
        echo "$OUT"
        echo "$OUT" >> "$LOG_MPI"
        TP=$(echo "$OUT" | awk -F, '{print $4}')
        MD=$(echo "$OUT" | awk -F, '{print $9}')
        S=$(python3 -c "print(0 if float('$TP')<=0 else float('$T1')/float('$TP'))")
        E=$(python3 -c "print(0 if $P<=0 else float('$S')/$P)")
        printf "%s,%s,%s,%s,%s,%.6f,%.6f,%s\n" \
            "$N" "$P" "$Q" "$T1" "$TP" "$S" "$E" "$MD" >> "$SUMMARY"
    done
done

echo
echo "Done. See $SUMMARY"
