#!/bin/bash

declare -a N=("10000000" "100000000" "1000000000")
declare -a K=("1" "2" "4" "8" "16" "32" "64" "128" "256")

for k in "${K[@]}"; do
	for n in "${N[@]}"; do
		for ((i=0; i<10; ++i)) do
			./adder ${n} ${k}
		done
	done
done