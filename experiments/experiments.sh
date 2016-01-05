#!/bin/bash

# Halts the script when a nonzero value is returned from a command.
set -e

RESULTS=experiments/results.txt
NUMBER_OF_ITER=10

function index_test {
    echo "Indexing and compressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        START=`date +%s%N`
        for ((i=0; i<NUMBER_OF_ITER; i++)); do bin/ipmt index $1; done
        END=`date +%s%N`
    else
        START=`gdate +%s%N`
        for ((i=0; i<NUMBER_OF_ITER; i++)); do bin/ipmt index $1; done
        END=`gdate +%s%N`
    fi
    TOTAL_TIME=$(((END-START)/1000000))
    TOTAL_TIME=$((TOTAL_TIME/NUMBER_OF_ITER))
    echo $TOTAL_TIME >> $RESULTS
    echo "" >> $RESULTS
}


function search_test {
    echo "Searching for the string $1 in file $2 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS

    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        START=`date +%s%N`
        for ((i=0; i<NUMBER_OF_ITER; i++)); do bin/ipmt search -c $1 $2; done
        END=`date +%s%N`
    else
        START=`gdate +%s%N`
        for ((i=0; i<NUMBER_OF_ITER; i++)); do bin/ipmt search -c $1 $2; done
        END=`gdate +%s%N`
    fi

    TOTAL_TIME=$(((END-START)/1000000))
    TOTAL_TIME=$((TOTAL_TIME/NUMBER_OF_ITER))
    echo $TOTAL_TIME >> $RESULTS
    echo "" >> $RESULTS
}

# Main code below:
cd ../
rm -f $RESULTS
make

index_test big.txt
search_test herself big.idx

