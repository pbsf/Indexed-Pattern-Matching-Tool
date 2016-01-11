#!/bin/bash

# Halts the script when a nonzero value is returned from a command.
set -e

RESULTS=experiments/results.txt
COMP_TAX_CSV=experiments/comp_tax.raw
DECOMP_TIME_CSV=experiments/time_decomp.raw
TIME_CSV=experiments/time_comp.raw

NUMBER_OF_ITER=1

function index {
    echo -n "Indexing and compressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt index " $1
}

function search {
    echo -n "Searching for the string $1 in file $2 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt search -c " $1 $2
}

function compress {
    echo -n "LZ78: Compressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    INI_SIZE=`du -k "$1" | cut -f1`
    aux "bin/ipmt compress " $1 "" complz78 $INI_SIZE
    COMP_FILE_NAME="${1%.*}.comp"
    SIZE=`du -k "$COMP_FILE_NAME" | cut -f1`
    echo "Compressed file has size, in kb: $SIZE" >> $RESULTS
    COMPRESSION_TAX=($INI_SIZE-$SIZE)/$INI_SIZE
    echo -n "Compression tax: " >> $RESULTS
    COMPRESSION_TAX=$(bc -l <<< "scale=2; $COMPRESSION_TAX")
    echo "$COMPRESSION_TAX" >> $RESULTS
    echo -n "$INI_SIZE $COMPRESSION_TAX " >> $COMP_TAX_CSV
    echo -n "$SIZE " >> $DECOMP_TIME_CSV

    decompress $COMP_FILE_NAME
    compress_gzip $1
    rm $COMP_FILE_NAME
}

function compress_gzip {
    echo -n "GZIP: Compressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    rm -f temp.gz
    INI_SIZE=`du -k "$1" | cut -f1`
    aux $1 "" "tar -pczf temp.gz " compgzip
    SIZE=`du -k "temp.gz" | cut -f1`
    echo "Compressed file has size, in kb: $SIZE" >> $RESULTS
    COMPRESSION_TAX=($INI_SIZE-$SIZE)/$INI_SIZE
    echo -n "Compression tax: " >> $RESULTS
    COMPRESSION_TAX=$(bc -l <<< "scale=2; $COMPRESSION_TAX")
    echo "$COMPRESSION_TAX" >> $RESULTS
    echo "$COMPRESSION_TAX" >> $COMP_TAX_CSV
    echo "" >> $RESULTS
    mkdir -p temp
    aux $1 "" "tar -xzf temp.gz " decompgzip
    rm -rf temp
}

function decompress {
    echo -n "Decompressing file $1 $NUMBER_OF_ITER times took on average, in miliseconds:" >> $RESULTS
    aux "bin/ipmt decompress " $1 "" decomplz78
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
    if [ "$4" == "compgzip" ]; then
        echo $TOTAL_TIME >> $TIME_CSV
    elif [ "$4" == "complz78" ]; then
        echo -n "$5 $TOTAL_TIME " >> $TIME_CSV
    elif [ "$4" == "decomplz78" ]; then
        echo -n "$5 $TOTAL_TIME " >> $DECOMP_TIME_CSV
    elif [ "$4" == "decompgzip" ]; then
        echo "$5 $TOTAL_TIME " >> $DECOMP_TIME_CSV
    fi
}

function start_exp {
    rm -rf $RESULTS
    rm -rf $TIME_CSV
    rm -rf $COMP_TAX_CSV
    rm -rf $DECOMP_TIME_CSV
    echo "Size LZ78 Gzip" >> $COMP_TAX_CSV
    echo "Size LZ78 Gzip" >> $TIME_CSV
    echo "Size LZ78 Gzip" >> $DECOMP_TIME_CSV
}

function clean {
    rm -rf arquivos/*.comp
    rm -rf arquivos/*.decomp
    rm -rf temp.gz
}

# Main code below:
cd ../
start_exp
make

# Perform experiments here
compress arquivos/100k.txt
compress arquivos/200k.txt
compress arquivos/300k.txt
compress arquivos/700k.txt
compress arquivos/2mb.txt
compress arquivos/3mb.txt
compress arquivos/5mb.txt
compress arquivos/10mb.txt
compress arquivos/50mb.txt
compress arquivos/100mb.txt
compress arquivos/200mb.txt
compress arquivos/300mb.txt
compress arquivos/500mb.txt
#compress arquivos/1gb.txt

# Clean-up
clean
