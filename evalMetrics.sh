#!/bin/bash


# Arguments
declare -i numTests=$1

declare input=$2
declare -i numIterations=$3
declare -i numAnts=$4
declare -i evapRate=$5

# Running experiments
for (( i = 0; i < ${numTests}; ++i )); do
	./bin/antOptimization ${numAnts} ${numIterations} ${evapRate} ${input} | \
		grep -oP "Length: \K\d+"
done
