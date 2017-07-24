location=$1

cd PreselectedWithRegressionDeepCSV_JECm1
echo "JEC +"
source processPreSelection_Radion.sh ${location}
echo "JEC LMR"
cd LMRSelection_chi2 
source processLMRSelection.sh
echo "JEC MMR"
cd ../MMRSelection_chi2
source processMMRSelection.sh
cd ../../
cd PreselectedWithRegressionDeepCSV_JECp1
echo "JEC -"
source processPreSelection_Radion.sh ${location}
cd LMRSelection_chi2
echo "JEC LMR"
source processLMRSelection.sh
cd ../MMRSelection_chi2
echo "JEC MMR"
source processMMRSelection.sh
cd ../../
echo "JER -"
cd PreselectedWithRegressionDeepCSV_JERm1
source processPreSelection_Radion.sh ${location}
echo "JER LMR"
cd LMRSelection_chi2
source processLMRSelection.sh
echo "JER MMR"
cd ../MMRSelection_chi2
source processMMRSelection.sh
cd ../../
echo "JER +"
cd PreselectedWithRegressionDeepCSV_JERp1
source processPreSelection_Radion.sh ${location}
cd LMRSelection_chi2
echo "JER LMR"
source processLMRSelection.sh
cd ../MMRSelection_chi2
echo "JER MMR"
source processMMRSelection.sh
cd ../../

echo "bTag -"
cd PreselectedWithRegressionDeepCSV_bTagm1
source processPreSelection_Radion.sh ${location}
echo "bTag LMR"
cd LMRSelection_chi2
source processLMRSelection.sh
echo "bTag MMR"
cd ../MMRSelection_chi2
source processMMRSelection.sh
cd ../../
echo "bTag +"
cd PreselectedWithRegressionDeepCSV_bTagp1
source processPreSelection_Radion.sh ${location}
cd LMRSelection_chi2
echo "bTag LMR"
source processLMRSelection.sh
cd ../MMRSelection_chi2
echo "bTag MMR"
source processMMRSelection.sh
cd ../../

