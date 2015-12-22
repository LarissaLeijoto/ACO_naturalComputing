#!/bin/bash


# Arguments
declare -i numTests=$1

declare input=$2
declare -i numIterations=$3
declare -i numAnts=$4
declare -i antStep=$5

logfile="$(basename ${input})"
logfile="${logfile//.txt}.log"
echo -n > $logfile

# Running experiments
for (( i = 0; i < ${numTests}; ++i )); do
	for (( j = 10; j <= ${numAnts}; j += ${antStep} )); do
		for(( wite = 100; wite <= ${numIterations}; wite = wite + 100)); do
			for (( k = 10; k < 100; k += 20 )); do
				echo -e "${j}\t${wite}\t${k}\t${input}"
			done
		done
	done
done | xargs -P 4 -I {} bash -c '
	value=$(./bin/antOptimization {} | grep -oP "Length: \K\d+")
	echo -e "{}\t${value}";' >> ${logfile}
