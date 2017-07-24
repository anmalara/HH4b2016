
./runLimits_bias.sh gaus_exp 250_330 novo 550_1200 1
./runLimits_bias.sh novo 285_625 novo 550_1200 2

echo "LMR"
cp drawLimits_LMR.c PreselectedWithRegressionDeepCSV/limits_bias/LMR/
cp DrawLimitPlot.cc PreselectedWithRegressionDeepCSV/limits_bias/LMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/LMR/
root -l -b -q drawLimits_LMR.c
rm -fr drawLimits_LMR.c 
rm -fr DrawLimitPlot.cc
cd -

echo "MMR"
cp drawLimits_MMR.c PreselectedWithRegressionDeepCSV/limits_bias/MMR/
cp DrawLimitPlot.cc PreselectedWithRegressionDeepCSV/limits_bias/MMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/MMR/
root -l -b -q drawLimits_MMR.c
rm -fr drawLimits_MMR.c 
rm -fr DrawLimitPlot.cc 
cd -

