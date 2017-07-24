#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd /scratch/malara/WorkingArea/release/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit 
#cmsenv
cd $DIR

function_LMR=$1
range_LMR=$2
background_LMR="_${function_LMR}_${range_LMR}"

function_MMR=$3
range_MMR=$4
background_MMR="_${function_MMR}_${range_MMR}"

if [ $5 -eq 1 ];
        then
        masses_LMR=(260 265 270 275 280 285 290 295 300 310 320)
        else masses_LMR=(290 295 300 310 320 330 340 350 360 370 380 390 400 410 420 430 440 450 460 470 480 490 500 510 520 530 540 550 560 570 580 590 600 610 620)
fi

if [ $5 -eq 1 ];
        then
        masses_MMR=(550 570 600 620 650 670 700 720 750 770 800 820 840 860 880 900 920 940 960 980 1000) 
        else masses_MMR=() 
fi

echo 'MMR'
for i in ${masses_MMR[@]}

do
        cd $DIR
        folder="PreselectedWithRegressionDeepCSV/limits_bias/MMR/MMR_${i}${background_MMR}"
	if [ -d $folder ]
	then
	cd $folder
	else
        echo "mass point $i"
        continue
	fi
	mkdir out
        echo "Asymptotic"
        combine -M Asymptotic datacard_${i}${background_MMR}.txt -t -1  |& tee  CMS_HH4b_$i\_13TeV_asymptoticCLs.out
        echo "MaxLikelihoodFit"
        combine -M MaxLikelihoodFit datacard_${i}${background_MMR}.txt -t -1 --minimizerTolerance=0.001 --rMin=0 --rMax=3 |& tee CMS_HH4b_$i\_13TeV_MaxLikelihood.out
        echo "Plot"
        combine -M MaxLikelihoodFit datacard_${i}${background_MMR}.txt -t -1 --minimizerTolerance=0.001 --rMin=0 --rMax=3 --saveNormalizations --plot --out out 
done

cd $DIR

echo 'LMR'

for i in ${masses_LMR[@]}

do
        cd $DIR
        folder="PreselectedWithRegressionDeepCSV/limits_bias/LMR/LMR_${i}${background_LMR}"
        if [ -d $folder ]
        then
        cd $folder
	else
        echo "mass point $i" 
	continue
        fi
        mkdir out
        echo "Asymptotic"
        combine -M Asymptotic datacard_${i}${background_LMR}.txt -t -1  |& tee  CMS_HH4b_$i\_13TeV_asymptoticCLs.out
        echo "MaxLikelihoodFit"
        combine -M MaxLikelihoodFit datacard_${i}${background_LMR}.txt -t -1 --minimizerTolerance=0.001 --rMin=0 --rMax=3  |& tee  CMS_HH4b_$i\_13TeV_MaxLikelihood.out
        echo "Plot"
        combine -M MaxLikelihoodFit datacard_${i}${background_LMR}.txt -t -1 --minimizerTolerance=0.001 --rMin=0 --rMax=3 --saveNormalizations --plot --out out
done

cd $DIR

