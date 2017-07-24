mass=$1
cards=""
function=$2
range=$3
background="_${function}_${range}"
dirName="PreselectedWithRegressionDeepCSV/MMRSelection_chi2/fit/MMR_${mass}${background}"
dcardName="datacard_${mass}${background}.txt"
bgLogName="data_bkg.log"
Type="Split"
datacardtype="f_${function}"
data_file="w_background_${function}_${range}.root"
pdfLogName="pdf.log"
sigLogName="signal"${mass}"_sig.log"
trgLogName="PDFs/trig.log"


sig_norm=`grep 'norm =' ${dirName}/${sigLogName}| awk '{print $3/50}'`
jec_norm=`grep 'JEC     lnN' ${dirName}/${sigLogName} | awk '{print $3}'`
jer_norm=`grep 'JER     lnN' ${dirName}/${sigLogName} | awk '{print $3}'`
btag_norm=`grep 'btag    lnN' ${dirName}/${sigLogName} | awk '{print $3}'`
pdf_norm=`grep 'PDF lnN' ${dirName}/${pdfLogName} | awk '{print $3}'`
bkg_norm=`grep ' Background number of '${function}_${range}' = ' ${dirName}/${bgLogName} | awk '{print $6}'`
trigger_norm=`grep 'trig'${mass}' ' ${trgLogName} | awk '{print $2}'`

echo sig_norm ${sig_norm}
echo JEC ${jec_norm}
echo JER ${jer_norm}
echo trigger ${trigger_norm}
echo bTag ${btag_norm}
echo PDF ${pdf_norm}
echo bkg_norm ${bkg_norm}

#let's build a datacard!
cat > ${dirName}/${dcardName} <<EOF
imax 1 number of channels
jmax * number of backgrounds
kmax * number of systematic uncertainty sources
----------
shapes signal     HbbHbb w_signal_${mass}.root      HbbHbb:signal_fixed
shapes background HbbHbb $data_file      HbbHbb:$datacardtype
shapes data_obs   HbbHbb $data_file      HbbHbb:data_obs$background
----------
## Observation
bin                     HbbHbb
observation             -1
----------
bin                   HbbHbb          HbbHbb
process               signal      background
process               0           1
rate                  ${sig_norm} ${bkg_norm}
lumi_13TeV  lnN   1.026       -     
bTag      lnN     ${btag_norm}    -
JER       lnN     ${jer_norm}   - 
JEC       lnN     ${jec_norm}   -
trigger   lnN     ${trigger_norm}   -
PDF       lnN     ${pdf_norm}   -
EOF

#now add the systematics to the card
#grep 'sg_' ${dirName}/${sigLogName} | grep ' param ' >> ${dirName}/${dcardName}
grep 'par_'${function}'_' ${dirName}/${bgLogName} | grep ' param ' >> ${dirName}/${dcardName}

cards+="${dirName}/${dcardName} "
echo $cards


