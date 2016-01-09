#!/bin/bash

# Halts the script when a nonzero value is returned from a command.
set -e

RESULTS=experiments/results.txt
NUMBER_OF_ITER=10

function index {
    echo "Indexing and compressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt index " $1
}


function search {
    echo "Searching for the string $1 in file $2 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt search -c " $1 $2
}

function compress {
    echo "Compressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt compress " $1
}

function decompress {
    echo "Decompressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt decompress " $1
}

function aux {
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        START=`date +%s%N`
        for ((i=0; i<NUMBER_OF_ITER; i++)); do $3 $1 $2; done
        END=`date +%s%N`
    else
        START=`gdate +%s%N`
        for ((i=0; i<NUMBER_OF_ITER; i++)); do $3 $1 $2; done
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

index big.txt
search herself big.idx

