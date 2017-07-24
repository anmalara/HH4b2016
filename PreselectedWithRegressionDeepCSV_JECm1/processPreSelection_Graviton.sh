#!/bin/sh
location=$1
if test $location -eq 0
        then export dir="/eos/uscms/store/user/lpchbb/HeppyNtuples/V25"
elif test $location -eq 1
        then export dir="/scratch/malara/WorkingArea/IO_file/output_file/DeepCSV_final/MC/Original"
else
        folder="./"
fi

mkdir PDFs
cp ../PDFs/deepCSV_BH_Moriond17.csv PDFs/

#root -l -b -q "../HbbHbb_PreSelection.cc++(\"$dir\", \"GluGluToBulkGravitonToHHTo4B_M-260_narrow_13TeV-madgraph\",\"JECm1\",\"JER\",\"Trig\",\"bTag\",\"../gravall-v25.weights.xml\")"
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-270_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-300_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-350_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-400_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-450_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-500_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-550_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-600_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-650_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-750_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-800_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
#root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "GluGluToBulkGravitonToHHTo4B_M-900_narrow_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "BulkGravTohhTohbbhbb_narrow_M-1000_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'
root -l -b -q '../HbbHbb_PreSelection.cc++("$dir", "BulkGravTohhTohbbhbb_narrow_M-1200_13TeV-madgraph","JECm1","JER","Trig","bTag","gravall-v25.weights.xml")'

rm -fr PDFs/
