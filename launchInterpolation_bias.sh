echo "LMR 1"
cp InterpolateSignal_bias.C PreselectedWithRegressionDeepCSV/limits_bias/LMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/LMR/
root -l -b -q "InterpolateSignal_bias.C(\"gaus_exp\", \"250_330\",1, 0)"
rm InterpolateSignal_bias.C
cd -

echo "LMR 2"
cp InterpolateSignal_bias.C PreselectedWithRegressionDeepCSV/limits_bias/LMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/LMR/
root -l -b -q "InterpolateSignal_bias.C(\"novo\", \"285_625\",2, 0)"
rm InterpolateSignal_bias.C
cd -


echo "MMR 1"
cp InterpolateSignal_bias.C PreselectedWithRegressionDeepCSV/limits_bias/MMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/MMR/
root -l -b -q "InterpolateSignal_bias.C(\"novo\", \"550_1200\",1, 1)"
rm InterpolateSignal_bias.C
cd -



./runLimits_Interpolation_bias.sh gaus_exp 250_330 novo 550_1200 1
./runLimits_Interpolation_bias.sh novo 285_625 novo 550_1200 2

echo "LMR"
cp drawLimits_LMR_interpolated.c PreselectedWithRegressionDeepCSV/limits_bias/LMR/
cp DrawLimitPlot.cc PreselectedWithRegressionDeepCSV/limits_bias/LMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/LMR/
root -l -b -q drawLimits_LMR_interpolated.c 
rm -fr drawLimits_LMR_interpolated.c 
rm -fr DrawLimitPlot.cc
cd -


echo "MMR"
cp drawLimits_MMR_interpolated.c  PreselectedWithRegressionDeepCSV/limits_bias/MMR/
cp DrawLimitPlot.cc PreselectedWithRegressionDeepCSV/limits_bias/MMR/
cd PreselectedWithRegressionDeepCSV/limits_bias/MMR/
root -l -b -q drawLimits_MMR_interpolated.c 
rm -fr drawLimits_MMR_interpolated.c 
rm -fr DrawLimitPlot.cc
cd -


cp drawLimits_All.c PreselectedWithRegressionDeepCSV/limits_bias/
cp DrawLimitPlot_All.cc PreselectedWithRegressionDeepCSV/limits_bias/
cd PreselectedWithRegressionDeepCSV/limits_bias/
root -l -b -q drawLimits_All.c
mv UpperLimit_all.png UpperLimit_all_bias.png
rm -fr drawLimits_All.c
rm -fr DrawLimitPlot.cc
cd -


