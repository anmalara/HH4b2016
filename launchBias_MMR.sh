#!/bin/bash
  
  DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  echo $DIR
  cd $DIR
  
  function_MMR_1=$1
  range_MMR_1=$2
  background_MMR_1="_${function_MMR_1}_${range_MMR_1}"
  function_MMR_2=$3
  range_MMR_2=$4
  background_MMR_2="_${function_MMR_2}_${range_MMR_2}"
  if [ $5 -eq 1 ];
          then
          masses_MMR=(1200) #550 600 650 750 800 900 1000 1200)
          strenght_MMR=(0.0 0.1 0.2 0.3)
          else masses_MMR=()
          strenght_MMR=()
  fi
  
  source_dir_MMR="PreselectedWithRegressionDeepCSV/MMRSelection_chi2/fit/"
  dest_dir_MMR="PreselectedWithRegressionDeepCSV/Bias/MMR/"
  seed=334477
  n_toys=300

  for signal_strenght in ${strenght_MMR[@]}
  do
          cd $DIR
          echo $signal_strenght
          name="closure_strenght_"$signal_strenght
          dir_name="Signal_Strenght_"${signal_strenght}"_fixed"${background_MMR_1}${background_MMR_2}
          if [ -d $dest_dir_MMR$dir_name ]; then
                  echo $dest_dir_MMR$dir_name
          else
                  mkdir $dest_dir_MMR$dir_name
          fi
          cd $DIR
          cp drawBias_MMR.C $dest_dir_MMR$dir_name
          cd $dest_dir_MMR$dir_name
          for i in ${masses_MMR[@]}
          do
                  echo $i
                  mkdir out
                  echo ${source_dir_MMR}/MMR_${i}${background_MMR_1}
                  echo ${source_dir_MMR}/MMR_${i}${background_MMR_2}
                  #cp ../../../../${source_dir_MMR}/MMR_${i}${background_MMR_1}/w_*.root .
                  #cp ../../../../${source_dir_MMR}/MMR_${i}${background_MMR_1}/datacard*txt .
                  #cp ../../../../${source_dir_MMR}/MMR_${i}${background_MMR_2}/w_*.root .
                  #cp ../../../../${source_dir_MMR}/MMR_${i}${background_MMR_2}/datacard*txt .
                  #combine datacard_${i}${background_MMR_1}.txt -M GenerateOnly -m $i -t ${n_toys} --saveToys -s ${seed} --expectSignal=$signal_strenght -n ${name} --toysNoSystematic
                  #combine datacard_${i}${background_MMR_2}.txt -M MaxLikelihoodFit -m $i --expectSignal=$signal_strenght --rMin=-10 --rMax=10 -t ${n_toys} --toysFile=higgsCombine${name}.GenerateOnly.mH$i.${seed}.root -s ${seed} -n ${name}_${i} --robustFit=1  #--saveNormalizations --plot --out out 
          done
          root -l -b -q drawBias_MMR.C\($signal_strenght,\"$name\",\"$background_MMR_1\",\"$background_MMR_2\"\)
  done

cd $DIR
