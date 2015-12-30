#!/bin/bash

RESULTS=experiments/results.txt

function index_test {
    echo "Indexing file $1 10 times took on average, in seconds:" >> $RESULTS
    START=`date +%s`
    for i in {1..10}; do bin/ipmt index $1; done
    END=`date +%s`
    TOTAL_TIME=$((END-START))
    echo $TOTAL_TIME >> $RESULTS
    echo "" >> $RESULTS
}


function search_test {
    echo "Searching for string $1 in file $2 10 times took on average, in seconds:" >> $RESULTS
    START=`date +%s`
    for i in {1..10}; do bin/ipmt search $2; done
    END=`date +%s`
    TOTAL_TIME=$((END-START))
    echo $TOTAL_TIME >> $RESULTS
    echo "" >> $RESULTS
}

# Main code below:
cd ../
rm -f $RESULTS
make

index_test big.txt
search_test str big.idx

