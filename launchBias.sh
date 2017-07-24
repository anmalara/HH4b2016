  source launchBias_LMR.sh gaus_exp 250_330 gaus_exp 250_330 1 &> /dev/null &
  source launchBias_LMR.sh crystal 250_330 gaus_exp 250_330 1 &> /dev/null &
  source launchBias_LMR.sh crystal 250_330 crystal 250_330 1 &> /dev/null &
  source launchBias_LMR.sh gaus_exp 250_330 crystal 250_330 1 &> /dev/null &

  source launchBias_LMR.sh novo 285_625 novo 285_625 2 &> /dev/null &
  source launchBias_LMR.sh crystal_1 285_625 novo 285_625 2 &> /dev/null &
  source launchBias_LMR.sh crystal_1 285_625 crystal_1 285_625 2 &> /dev/null &
  source launchBias_LMR.sh novo 285_625 crystal_1 285_625 2 &> /dev/null &


source launchBias_MMR.sh novo 550_1200 novo 550_1200 1 &> /dev/null &
source launchBias_MMR.sh crystal_1 550_1200 novo 550_1200 1 &> /dev/null &
source launchBias_MMR.sh crystal_1 550_1200 crystal_1 550_1200 1 &> /dev/null &
source launchBias_MMR.sh novo 550_1200 crystal_1 550_1200 1 &> /dev/null &


