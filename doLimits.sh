#!/bin/bash

function_LMR=$1
range_LMR=$2
function_MMR=$3 
range_MMR=$4

function show_help { 
    echo "./doLimits.sh [-c to clean up] [-d debug combine]" 
}

if [ $5 -eq 1 ]; 
	then
	massesLMR=() #260 270 300 350)
	else massesLMR=() #270 300 350 400 450 500 550 600 650)
fi
cleanUp=0
#rebin=10
debug=0

while getopts "h?cr:d" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    d) 
        debug=1
        ;;
    c)  cleanUp=1
        ;;
    esac
done

for m in ${massesLMR[@]}
do
    if [ $cleanUp -eq 0 ]; then
	echo  'LMR' $function_LMR $range_LMR
        source doBuildInputsLMR.sh $m $function_LMR $range_LMR $7
        source ./doDatacardsLMR.sh $m $function_LMR $range_LMR
    fi
    
    if [ $cleanUp -eq 1 ]; then
        ./cleanUp.sh $m
    fi
done



if [ $5 -eq 1 ];
        then
	massesMMR=() #550 600 650 750 800 900 1000 1200)
        else massesMMR=()
fi

cleanUp=0
debug=0

while getopts "h?cr:d" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    d)
        debug=1
        ;;
    c)  cleanUp=1
        ;;
    esac
done

for m in ${massesMMR[@]}
do
    if [ $cleanUp -eq 0 ]; then
        echo  'MMR' $function_MMR $range_MMR
        source doBuildInputs.sh $m $function_MMR $range_MMR $7
        source doDatacards.sh $m $function_MMR $range_MMR
    fi

    if [ $cleanUp -eq 1 ]; then
        ./cleanUp.sh $m
    fi
done
