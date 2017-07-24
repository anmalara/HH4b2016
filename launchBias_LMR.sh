#!/bin/bash
  
  DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  echo $DIR
  cd $DIR
  
  function_LMR_1=$1
  range_LMR_1=$2
  background_LMR_1="_${function_LMR_1}_${range_LMR_1}"
  function_LMR_2=$3
  range_LMR_2=$4
  background_LMR_2="_${function_LMR_2}_${range_LMR_2}"
  if [ $5 -eq 1 ];
          then
          masses_LMR=(260 270 300)  
          strenght_LMR=(0.0 0.6 1.0) 
          else masses_LMR=(270 300 350 400 450 500 550 600)
          strenght_LMR=(0.0 0.1 0.2 0.3 0.6)
  fi
  
  source_dir_LMR="PreselectedWithRegressionDeepCSV/LMRSelection_chi2/fit/"
  dest_dir_LMR="PreselectedWithRegressionDeepCSV/Bias/LMR/"
  seed=334477
  n_toys=300

  for signal_strenght in ${strenght_LMR[@]}
  do
          cd $DIR
          echo $signal_strenght
          name="closure_strenght_"$signal_strenght
          dir_name="Signal_Strenght_"${signal_strenght}"_fixed"${background_LMR_1}${background_LMR_2}
          if [ -d $dest_dir_LMR$dir_name ]; then
                  echo $dest_dir_LMR$dir_name
          else
                  mkdir $dest_dir_LMR$dir_name
          fi
          cd $DIR
          cp drawBias_LMR.C $dest_dir_LMR$dir_name
          cd $dest_dir_LMR$dir_name
          for i in ${masses_LMR[@]}
          do
                  echo $i
                  mkdir out
                  echo ${source_dir_LMR}LMR_${i}${background_LMR_1}
                  echo ${source_dir_LMR}LMR_${i}${background_LMR_2}
                  cp ../../../../${source_dir_LMR}LMR_${i}${background_LMR_1}/w_*.root .
                  cp ../../../../${source_dir_LMR}LMR_${i}${background_LMR_1}/datacard*txt .
                  cp ../../../../${source_dir_LMR}LMR_${i}${background_LMR_2}/w_*.root .
                  cp ../../../../${source_dir_LMR}LMR_${i}${background_LMR_2}/datacard*txt .
                  combine datacard_${i}${background_LMR_1}.txt -M GenerateOnly -m $i -t ${n_toys} --saveToys -s ${seed} --expectSignal=$signal_strenght -n ${name} --toysNoSystematic
                  combine datacard_${i}${background_LMR_2}.txt -M MaxLikelihoodFit -m $i --expectSignal=$signal_strenght --rMin=-10 --rMax=10 -t ${n_toys} --toysFile=higgsCombine${name}.GenerateOnly.mH$i.${seed}.root -s ${seed} -n ${name}_${i} --robustFit=1  #--saveNormalizations --plot --out out 
          done
          root -l -b -q drawBias_LMR.C\($signal_strenght,\"$name\",\"$background_LMR_1\",\"$background_LMR_2\"\)
  done

cd $DIR
