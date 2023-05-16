#!/bin/bash

declare -a N=("1" "10" "100" "1000")
declare -a Np_Nc=("1 1" "1 2" "1 4" "1 8" "2 1" "4 1" "8 1")

for n in "${N[@]}"; do
	for np_nc in "${Np_Nc[@]}"; do
		read -a tup <<< "$np_nc"
		for ((i=0; i<10; ++i)) do
			./producer_consumer ${n} ${np_nc}
		done
	done
done
