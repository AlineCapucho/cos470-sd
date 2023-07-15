#!/bin/bash
# testes de funcionalidade

# teste 0
declare -a N=("2")
R="10"
K="2"
directory="teste-0"

mkdir ${directory}

for n in "${N[@]}"; do
  for ((i=0; i<n; ++i)) do
    ./client ${R} ${K}
  done

  wait $(jobs -p)

  python3 log_validator.py ${R} ${n} ./log.txt ./resultado.txt

  cp log.txt ${directory}/log-${n}.txt
  rm log.txt

  cp resultado.txt ${directory}/resultado-${n}.txt
  rm resultado.txt
done

# teste 1
declare -a N=("2" "4" "8" "16" "32")
R="10"
K="2"
directory="teste-1"

mkdir ${directory}

for n in "${N[@]}"; do
  for ((i=0; i<n; ++i)) do
    ./client ${R} ${K}
  done

  wait $(jobs -p)

  python3 log_validator.py ${R} ${n} ./log.txt ./resultado.txt

  cp log.txt ${directory}/log-${n}.txt
  rm log.txt

  cp resultado.txt ${directory}/resultado-${n}.txt
  rm resultado.txt
done

# teste 2
declare -a N=("2" "4" "8" "16" "32" "64")
R="5"
K="1"
directory="teste-2"

mkdir ${directory}

for n in "${N[@]}"; do
  for ((i=0; i<n; ++i)) do
    ./client ${R} ${K}
  done

  wait $(jobs -p)

  python3 log_validator.py ${R} ${K} ./log.txt ./resultado.txt

  cp log.txt ${directory}/log-${n}.txt
  rm log.txt

  cp resultado.txt ${directory}/resultado-${n}.txt
  rm resultado.txt
done

# teste 3
declare -a N=("2" "4" "8" "16" "32" "64" "128")
R="3"
K="0"
directory="teste-3"

mkdir ${directory}

for n in "${N[@]}"; do
  for ((i=0; i<n; ++i)) do
    ./client ${R} ${K}
  done

  wait $(jobs -p)

  python3 log_validator.py ${R} ${K} ./log.txt ./resultado.txt

  cp log.txt ${directory}/log-${n}.txt
  rm log.txt

  cp resultado.txt ${directory}/resultado-${n}.txt
  rm resultado.txt
done
