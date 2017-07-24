#!/bin/bash

function_LMR=$1
range_LMR=$2
function_MMR=$3 
range_MMR=$4
range_num_LMR=$5
range_num_MMR=$6
location=$7

function show_help { 
    echo "./doLimits.sh [-c to clean up] [-d debug combine]" 
}

if [ $range_num_LMR -eq 1 ]; 
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
	echo  'LMR' $m $function_LMR $range_LMR $location $range_num_LMR 
        source doBuildInputsLMR_bias.sh $m $function_LMR $range_LMR $location $range_num_LMR
        source doDatacardsLMR_bias.sh $m $function_LMR $range_LMR $range_num_LMR
    fi
    
    if [ $cleanUp -eq 1 ]; then
        ./cleanUp.sh $m
    fi
done



if [ $range_num_MMR -eq 1 ];
        then
	massesMMR=(1200)  #550 600 650 750 800 900 1000 1200)
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
        echo  'MMR' $m $function_MMR $range_MMR $location $range_num_MMR 
        source doBuildInputs_bias.sh $m $function_MMR $range_MMR $location $range_num_MMR
        source doDatacards_bias.sh $m $function_MMR $range_MMR $range_num_MMR
    fi

    if [ $cleanUp -eq 1 ]; then
        ./cleanUp.sh $m
    fi
done
