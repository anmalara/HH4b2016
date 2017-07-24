/* ************************************
 *  *                Library             *
 *   **************************************/

#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "TArrow.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TFractionFitter.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLorentzVector.h"
#include "TMinuit.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TPaveText.h"
#include "TPaveText.h"
#include "TProfile.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TView.h"
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooCBShape.h"
#include "RooChebychev.h"
#include "RooConstVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooGaussian.h"
#include "RooIntegralMorph.h"
#include "RooNLLVar.h"
#include "RooPolynomial.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "../../../PDFs/ExpGaussExp.h"

using namespace RooFit ;

/*************************************
 **      Function Declaration        *
 *************************************/
Double_t straight_line(Double_t *x, Double_t *par);
Double_t pol_line(Double_t *x, Double_t *par);
void PrintArray(Double_t *v, Int_t dim, Int_t precision);
std::string itoa(int i);
void interpolation_normalization(bool, std::string, std::string, int);
bool flag_crystal = 1; //1 for CrystalBall 0 for ExpGaussExp
const int NumOfSyst=5;

void InterpolateSignal_bias(std::string function, std::string name_range, int range, bool flag_MMR) {
    gSystem->Load("../../../PDFs/ExpGaussExp_cxx.so");
    interpolation_normalization(flag_MMR, function, name_range, range);

    // I m p o r t   p d f   s h a p e s
    // ------------------------------------------------------
    
    // Observable
    RooRealVar *X;
    if(flag_MMR){
        X= new RooRealVar("x","x",400,1400);
    }
    else{
        if (range==1){X= new RooRealVar("x","x",230,400);}
        else{X= new RooRealVar("x","x",250,700);}
    }
    
    std::string dir, background=function+"_"+name_range;
    double step = 10;
    std::vector<double> masses;
    if (flag_MMR) {
        dir = "MMR";
        std::vector<double> masses_temp;
        if (range==1){masses_temp= {550, 600, 650, 750, 800, 900, 1000, 1200};}
        else{masses_temp= {550, 600, 650, 750, 800, 900, 1000, 1200};}
        for (unsigned int i = 0 ; i< masses_temp.size(); i++) {
            masses.push_back(masses_temp[i]);
        }
    }
    else{
        dir = "LMR";
        std::vector<double> masses_temp;
        if (range==1){masses_temp= {260, 270, 300, 350};}
        else{masses_temp= {270, 300, 350, 400, 450, 500, 550, 600, 650};}
        for (unsigned int i = 0 ; i< masses_temp.size(); i++) {
            masses.push_back(masses_temp[i]);
        }
        
    }
    const unsigned int nMCpoints=masses.size();
    
    TFile *f[nMCpoints];   
    RooWorkspace* xf[nMCpoints];    
    RooAbsPdf* PDF_mass[nMCpoints];
    
    for (unsigned int i = 0; i<nMCpoints; i++ ) {
        TString name = Form("%s_%d_%s/w_signal_%d.root", dir.c_str(), int(masses[i]), background.c_str(), int(masses[i]));
        if (!gSystem->AccessPathName(name)) {
            f[i] = new TFile(name);
            xf[i] = (RooWorkspace*)f[i]->Get("HbbHbb");
            xf[i]->Print();
            PDF_mass[i] = xf[i]->pdf("signal_fixed");
            PDF_mass[i]->Print("t");
        } else {
            std::cout<<"File is not found: "<<name<<std::endl;
            return;
        }
    }

    RooWorkspace* w_test= new RooWorkspace("w_test");
    for (unsigned int i = 0; i!=nMCpoints; ++i ) {
        RooWorkspace w("w");
        TString name = Form("mass_%d",int(masses[i]));
        cout<<"test_1" << endl;
        PDF_mass[i]->Print();
        cout<<"test_2" << endl;
        w.import(*PDF_mass[i],RooFit::RenameConflictNodes(name),RooFit::RenameAllVariablesExcept(name,"x"),RooFit::RenameVariable("signal_fixed","signal_fixed_"+name), RooFit::RenameVariable("signalCore_fixed","signalCore_fixed_"+name), RooFit::RenameVariable("signalComb_fixed","signalComb_fixed_"+name) );
        cout<<"test_3" << endl;
        w.Print();
        cout<<"test_4" << endl;
        PDF_mass[i]->Print();
        cout<<"test_5" << endl;
        PDF_mass[i] = w.pdf("signal_fixed_"+name);
        cout<<"test_6" << endl;
        PDF_mass[i]->Print();
        cout<<"test_7" << endl;
        w_test= new RooWorkspace("w_test");
        w_test->import(*PDF_mass[i]);
        w_test->Print();
        PDF_mass[i] = w_test->pdf("signal_fixed_"+name);
        w_test->Print();
        cout<<"test_8" << endl;
        if (i!=0)PDF_mass[i-1]->Print();
    }
    cout<<"test_9" << endl;
    w_test->Print();

 
    for (unsigned int i = 0; i!=nMCpoints; ++i ) {
        cout<<"test_10" << endl;
        PDF_mass[i]->Print("t");
    }

    // C r e a t e   i n t e r p o l a t i n g   p d f
    // -----------------------------------------------
    
    // Create interpolation variable
    RooRealVar alpha("alpha","alpha",0,1.0) ;
    
    // Specify sampling density on observable and interpolation variable
    X->setBins(10000,"cache") ;
    alpha.setBins(2200,"cache") ;
    
    TCanvas* c[nMCpoints];
    TCanvas* c_tot;
    RooPlot* frame1[nMCpoints];
    RooPlot* frame_tot;
    frame_tot = X->frame() ;
    
    for (unsigned int iPoint = 0; iPoint<nMCpoints-1; iPoint++) {
        
        RooIntegralMorph lmorph("lmorph","lmorph",*PDF_mass[iPoint+1],*PDF_mass[iPoint],*X,alpha) ;
        if (masses[iPoint]<300){
            step=5;
        }
        else{
            step=10;
        }
        
        frame1[iPoint] = X->frame() ;
        PDF_mass[iPoint]->plotOn(frame1[iPoint]) ;
        PDF_mass[iPoint+1]->plotOn(frame1[iPoint]) ;
        PDF_mass[iPoint]->plotOn(frame_tot,LineColor(kBlue)) ;
        PDF_mass[iPoint+1]->plotOn(frame_tot, LineColor(kBlue)) ;
        cout<< " Interpolate from " << masses[iPoint] << " to " << masses[iPoint+1] << endl;


        int nPoints = int((masses[iPoint+1]-masses[iPoint])/step);
        //here bias is the range in which we define the variable in the workspace
        double bias=1.0;
        for (int i=0; i<=nPoints; i++) {
            if (std::find(masses.begin(), masses.end(), int(masses[iPoint]+i*step)) != masses.end()){continue; }            
            alpha.setVal(double(i)/double(nPoints)) ;
            cout<< " Interpolate mass : " << int(masses[iPoint]+i*step) << endl;
            lmorph.plotOn(frame1[iPoint],LineColor(kRed)) ;
            lmorph.plotOn(frame_tot,LineColor(kRed), LineWidth(1)) ;
            
            TH1D* hist;
            if(flag_MMR){
                hist= (TH1D*)lmorph.createHistogram("m_X (GeV)",*X,Binning(1000,400,1400));
            }
            else{
                hist= (TH1D*)lmorph.createHistogram("m_X (GeV)",*X,Binning(1000,0,1000));
            }
            
            TCanvas* c_temp = new TCanvas(Form("linearmorph_%d",iPoint),Form("linearmorph_%d",iPoint),700,700) ;
            hist->Draw();
            c_temp->SaveAs(Form("%s_%d_%s/c_mX_SR_KinFit_%d.png", dir.c_str(), int(masses[iPoint]+i*step), background.c_str(), int(masses[iPoint]+i*step)));
            delete c_temp;
            
            RooRealVar *x, *sg_p0, *sg_p1, *sg_p2, *sg_p3,*sg_p4;
            double m=masses[iPoint]+i*step;

            if (flag_MMR) {
                double rangeHi = 1.1584*m -5.84;
                double rangeLo = 0.72*m + 98;
                double sg_p1_up; 
                if (rangeLo<250) rangeLo=250;
                if (m<600){sg_p1_up=10;} else{sg_p1_up=25;} 
                sg_p0=new RooRealVar("sg_p0", "sg_p0", 0.90208*m+47.792, 1.09712*m-32.712);
                sg_p1=new RooRealVar("sg_p1", "sg_p1", 5.,sg_p1_up);
                sg_p2=new RooRealVar("sg_p2", "sg_p2", 0.,5.);
                sg_p3=new RooRealVar("sg_p3", "sg_p3", 0.,7.);
                x=new RooRealVar("x", "m_{X} (GeV)", rangeLo-100., rangeHi+100.);
                ExpGaussExp signal("signal", "Signal Prediction", *x, *sg_p0, *sg_p1, *sg_p2, *sg_p3);
                RooDataHist signalHistogram("signalHistogram", "Signal Histogram", RooArgList(*x), hist);
                signal.fitTo(signalHistogram, RooFit::Range(rangeLo, rangeHi), RooFit::Save());
                
                RooRealVar signal_p0("signal_p0", "signal_p0", sg_p0->getVal());
                RooRealVar signal_p1("signal_p1", "signal_p1", sg_p1->getVal());
                RooRealVar signal_p2("signal_p2", "signal_p2", sg_p2->getVal());
                RooRealVar signal_p3("signal_p3", "signal_p3", sg_p3->getVal());
                ExpGaussExp signal_fixed("signal_fixed", "Signal Prediction Fixed", *x, signal_p0, signal_p1, signal_p2, signal_p3);
                RooRealVar signal_p0_1("signal_p0_1", "signal_p0_1", sg_p0->getVal());
                RooRealVar signal_p1_1("signal_p1_1", "signal_p1_1", sg_p1->getVal());
                RooRealVar signal_p2_1("signal_p2_1", "signal_p2_1", sg_p2->getVal());
                RooRealVar signal_p3_1("signal_p3_1", "signal_p3_1", sg_p3->getVal());
                ExpGaussExp signal_bkg("signal_bkg", "Signal-as-background Prediction Fixed", *x, signal_p0_1, signal_p1_1, signal_p2_1, signal_p3_1);
                RooRealVar *signal_bkg_norm= new RooRealVar("signal_bkg_norm","signal_bkg_norm", 0., -abs(bias), abs(bias),"");

                RooWorkspace *w=new RooWorkspace("HbbHbb");                
                w->import(signal_fixed);
                w->import(signal_bkg);
                w->import(*signal_bkg_norm); 
                w->SaveAs(Form("%s_%d_%s/w_signal_%d.root", dir.c_str(), int(masses[iPoint]+i*step), background.c_str(), int(masses[iPoint]+i*step)));
            }
            else{
                double rangeHi = 1.1*m + 50;
                double rangeLo = m/3. + 160;
                if (rangeLo<250) rangeLo=250;
                sg_p0=new RooRealVar("sg_p0", "sg_p0", 0.94*m, 1.1*m);
                sg_p1=new RooRealVar("sg_p1", "sg_p1", 2., 0.1*m-20.);
                double mu_min = 1.6*m-440.; if (mu_min<0) mu_min=10.;
                sg_p2=new RooRealVar("sg_p2", "sg_p2", mu_min, 0.95*m+100.);
                double sigma_max;
                if (m<300) {sigma_max=300.;}else{ sigma_max = 150.;}
                sg_p3=new RooRealVar("sg_p3", "sg_p3", 10.,sigma_max);
                sg_p4=new RooRealVar("sg_p4", "sg_p4", 0., 1.);

                x=new RooRealVar("x", "m_{X} (GeV)", rangeLo, rangeHi);
                RooGaussian signalCore("signalCore", "Signal Prediction", *x, *sg_p0, *sg_p1);
                RooGaussian signalComb("signalComb", "Combinatoric", *x, *sg_p2, *sg_p3);
                RooAddPdf signal("signal", "signal", RooArgList(signalCore, signalComb), *sg_p4);
                RooDataHist signalHistogram("signalHistogram", "Signal Histogram", RooArgList(*x), hist);
                signal.fitTo(signalHistogram, RooFit::Range(rangeLo-50, rangeHi), RooFit::Save());

                RooRealVar signal_p0("signal_p0", "signal_p0", sg_p0->getVal());
                RooRealVar signal_p1("signal_p1", "signal_p1", sg_p1->getVal());
                RooRealVar signal_p2("signal_p2", "signal_p2", sg_p2->getVal());
                RooRealVar signal_p3("signal_p3", "signal_p3", sg_p3->getVal());
                RooRealVar signal_p4("signal_p4", "signal_p4", sg_p4->getVal());
                RooGaussian signalCore_fixed("signalCore_fixed", "Signal Prediction", *x, signal_p0, signal_p1);
                RooGaussian signalComb_fixed("signalComb_fixed", "Combinatoric", *x, signal_p2, signal_p3);
                RooAddPdf signal_fixed("signal_fixed", "signal", RooArgList(signalCore_fixed, signalComb_fixed), signal_p4);
                RooRealVar signal_p0_1("signal_p0_1", "signal_p0_1", sg_p0->getVal());
                RooRealVar signal_p1_1("signal_p1_1", "signal_p1_1", sg_p1->getVal());
                RooRealVar signal_p2_1("signal_p2_1", "signal_p2_1", sg_p2->getVal());
                RooRealVar signal_p3_1("signal_p3_1", "signal_p3_1", sg_p3->getVal());
                RooRealVar signal_p4_1("signal_p4_1", "signal_p4_1", sg_p4->getVal());
                RooGaussian signalCore_fixed_1("signalCore_fixed_1", "Signal Prediction_1", *x, signal_p0_1, signal_p1_1);
                RooGaussian signalComb_fixed_1("signalComb_fixed_1", "Combinatoric_1", *x, signal_p2_1, signal_p3_1);
                RooAddPdf signal_bkg("signal_bkg", "signal_1", RooArgList(signalCore_fixed_1, signalComb_fixed_1), signal_p4_1);
                
                RooRealVar *signal_bkg_norm= new RooRealVar("signal_bkg_norm","signal_bkg_norm", 0., -abs(bias), abs(bias),"");                
                RooWorkspace *w=new RooWorkspace("HbbHbb");             
                w->import(signal_fixed);
                w->import(signal_bkg);   
                w->import(*signal_bkg_norm); 
                w->SaveAs(Form("%s_%d_%s/w_signal_%d.root", dir.c_str(), int(masses[iPoint]+i*step), background.c_str(), int(masses[iPoint]+i*step)));
            }
        }
        c[iPoint] = new TCanvas(Form("linearmorph_%d",iPoint),Form("linearmorph_%d",iPoint),700,700) ;
        frame1[iPoint]->Draw();
        c[iPoint]->SaveAs(Form("fig/linearmorph_%d_%d_%d.png",int(masses[iPoint]), int(masses[iPoint+1]),range));
        c[iPoint]->SaveAs(Form("fig/linearmorph_%d_%d_%d.pdf",int(masses[iPoint]), int(masses[iPoint+1]),range));
        c[iPoint]->SaveAs(Form("fig/linearmorph_%d_%d_%d.root",int(masses[iPoint]), int(masses[iPoint+1]),range));
        
    }
    c_tot = new TCanvas("linearmorph_tot","linearmorph_tot",700,700) ;
    frame_tot->Draw();
    c_tot->SaveAs(Form("fig/linearmorph_tot_%d.png",range));
    c_tot->SaveAs(Form("fig/linearmorph_tot_%d.pdf",range));
    c_tot->SaveAs(Form("fig/linearmorph_tot_%d.root",range));
    return ;
}
/*********************************
 **           Functions          *
 *********************************/

//================================================================================

void interpolation_normalization(bool flag_MMR, std::string function, std::string name_range, int range){
    
    std::vector<double> mass_0;
    std::vector<double> mass;
    std::string dir, background=function+"_"+name_range;
    if (flag_MMR) {
        dir = "MMR";
        std::vector<double> mass_0_temp,mass_temp;
        if (range==1){mass_0_temp= {550, 600, 650, 750, 800, 900, 1000, 1200};}
        else{mass_0_temp= {550, 600, 650, 750, 800, 900, 1000, 1200};}
        if (range==1){mass_temp= {550, 570, 600, 620, 650, 670, 700, 720, 750, 770, 800, 820, 840, 860, 880, 900, 920, 940, 960, 980, 1000, 1050, 1100, 1150, 1200};}
        else{mass_temp= {550, 570, 600, 620, 650, 670, 700, 720, 750, 770, 800, 820, 840, 860, 880, 900, 920, 940, 960, 980, 1000, 1050, 1100, 1150, 1200};}
        for (unsigned int i = 0 ; i< mass_0_temp.size(); i++) {
            mass_0.push_back(mass_0_temp[i]);
        }
        
        for (unsigned int i = 0 ; i< mass_temp.size(); i++){
            mass.push_back(mass_temp[i]);
        }
    }
    else{
        dir = "LMR";
        std::vector<double> mass_0_temp,mass_temp;
        if (range==1){mass_0_temp= {260, 270, 300, 350};}
        else{mass_0_temp= {270, 300, 350, 400, 450, 500, 550, 600, 650};}
        if (range==1){mass_temp= {260, 265, 270, 275, 280, 285, 290, 295, 300, 310, 320, 350};}
        else{mass_temp= {270, 275, 280, 285, 290, 295, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 650};}
        for (unsigned int i = 0 ; i< mass_0_temp.size(); i++) {
            mass_0.push_back(mass_0_temp[i]);
        }
        
        for (unsigned int i = 0 ; i< mass_temp.size(); i++){
            mass.push_back(mass_temp[i]);

        }
        
    }
    const unsigned int nPoints=mass.size();
    const unsigned int nPoints_0=mass_0.size();
    double lin[nPoints],interpol[nPoints];
    double signal_0[nPoints_0];
    double interpol_syst_0[NumOfSyst][nPoints];
    double syst_0[NumOfSyst][nPoints_0];
    double syst[NumOfSyst][nPoints_0];
    double sp_syst_0[nPoints_0];
    std::string name_syst[NumOfSyst];
    double interpol_bias_0[nPoints];
    double signal_0_bias[nPoints_0];  
    double sp_bias_0[nPoints_0];
    TSpline3* sp_bias;

    int j=0;
    for (int syst_num=0; syst_num<NumOfSyst; syst_num++) {
        switch (syst_num) {
            case 0: name_syst[syst_num]="bTag";break;
            case 1: name_syst[syst_num]="JER";break;
            case 2: name_syst[syst_num]="JEC";break;
            case 3: name_syst[syst_num]="PDF";break;
            case 4: name_syst[syst_num]="trigger";break;
        }
    }
    
    for (unsigned i = 0; i<nPoints; i++) {
        if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){
            continue;
        }
        if (gSystem->AccessPathName(Form("%s_%d_%s", dir.c_str(), int(mass[i]), background.c_str()))) {
            gSystem->Exec(Form("mkdir %s_%d_%s", dir.c_str(), int(mass[i]), background.c_str()));
        }
        int lMarker=0;
        for (auto m : mass_0){
            if (m > mass[i]){
                break;
            }
            lMarker++;
        }
        gSystem->Exec(Form("cp -r %s_%d_%s/datacard_%d_%s.txt %s_%d_%s/datacard_%d_%s.txt", dir.c_str(), int(mass_0[lMarker]), background.c_str(), int(mass_0[lMarker]), background.c_str(), dir.c_str(), int(mass[i]), background.c_str(), int(mass[i]), background.c_str()));
        gSystem->Exec(Form("cp -r %s_%d_%s/signal%d_sig.log %s_%d_%s/signal%d_sig.log", dir.c_str(), int(mass_0[lMarker]), background.c_str(), int(mass_0[lMarker]), dir.c_str(), int(mass[i]), background.c_str(), int(mass[i])));
        gSystem->Exec(Form("cp -r %s_%d_%s/w_background_%s.root %s_%d_%s/w_background_%s.root", dir.c_str(), int(mass_0[lMarker]), background.c_str(), background.c_str(), dir.c_str(), int(mass[i]), background.c_str(), background.c_str()));
        if(mass_0[lMarker] == 1200){gSystem->Exec(Form("sed -i 's/.signal_%d./_signal_%d./g' %s_%d_%s/datacard_%d_%s.txt", int(mass_0[lMarker]), int(mass[i]), dir.c_str(), int(mass[i]), background.c_str(), int(mass[i]), background.c_str())); }
        else{gSystem->Exec(Form("sed -i 's/%d./%d./g' %s_%d_%s/datacard_%d_%s.txt", int(mass_0[lMarker]), int(mass[i]), dir.c_str(), int(mass[i]), background.c_str(), int(mass[i]), background.c_str()));}
        
    }

    for (unsigned i = 0; i<nPoints_0; i++) {
        
        std::string mass_string= itoa(mass_0[i]);
        std::cout<< mass_string << std::endl;
        std::string filename=dir+"_"+mass_string+"_"+background+"/datacard_"+mass_string+"_"+background+".txt";
        
        std::ifstream file(filename.c_str(), ios::in);
        bool found= false;
        std::string line;
        while (!found && !file.eof()) {
            getline(file, line);
            std::size_t pos = line.find("rate");
            if (pos!=std::string::npos){
                found=true;
                signal_0[i] = atof(line.substr(pos+22,line.find_last_of(" ")).c_str());
                cout<<"This is the rate in the datacards: "<<signal_0[i]<<endl;                
            }
        }
        
        file.close();

        std::string filename_bias=dir+"_"+mass_string+"_"+background+"/datacard_"+mass_string+"_"+background+".txt";
        std::ifstream file_bias(filename_bias.c_str(), ios::in);
        bool found_bias= false; 
        std::string line_bias;
        while (!found_bias && !file_bias.eof()) {
            getline(file_bias, line_bias);
            std::size_t pos = line_bias.find("shapeBkg_signal_bkg_HbbHbb__norm");
            if (pos!=std::string::npos){
                found_bias=true;
                signal_0_bias[i] = atof(line_bias.substr(pos+43,line_bias.find_last_of(" ")).c_str());
                cout<<"This is the bias in the datacards: "<<signal_0_bias[i]<<endl;
            }
        }
        file_bias.close();

        for (int syst_num = 0; syst_num<NumOfSyst; syst_num++) {
            
            std::string filename=dir+"_"+mass_string+"_"+background+"/datacard_"+mass_string+"_"+background+".txt";
            std::ifstream file_syst(filename.c_str(), ios::in);
            found= false;
            while (!found && !file_syst.eof()) {
                getline(file_syst, line);
                std::size_t pos = line.find(Form("%s", name_syst[syst_num].c_str()));;
                if (pos!=std::string::npos){
                    found=true;
                    syst_0[syst_num][i] = atof(line.substr(pos+16,line.find_last_of("-")).c_str());
                    cout<<"This is the systematic in the datacards: "<<syst_0[syst_num][i]<<endl;
                }
            }
            file_syst.close();
        }
        if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){j++;}        
    }

    TCanvas *c1 = new TCanvas("canvas","canvas",700,700);
    TPad *p_1=new TPad("p_1", "p_1", 0, 0.3, 1, 1);
    p_1->SetFillStyle(4000);
    p_1->SetFrameFillColor(0);
    p_1->SetBottomMargin(0.035);
    TPad* p_2 = new TPad("p_2", "p_2",0,0,1,0.3);
    p_2->SetBottomMargin(0.2);
    p_2->SetTopMargin(0.02);
    p_2->SetFillColor(0);
    p_2->SetBorderMode(0);
    p_2->SetBorderSize(2);
    p_2->SetFrameBorderMode(0);
    p_2->SetFrameBorderMode(0);
    p_1->Draw();
    p_2->Draw();
    p_1->cd();
    
    TH1F* hr_1;
    TH1F* hr_2;
    if (flag_MMR) {
        if (range==1){hr_1 = c1->DrawFrame(400,0,1300,10);}
        else{hr_1 = c1->DrawFrame(400,0,1300,10);}
        hr_1->SetTitle("Linear interpolation of yield in MMR");
    } else {
        if (range==1){hr_1 = c1->DrawFrame(250,0,370,600);}
        else{hr_1 = c1->DrawFrame(250,0,650,2000);}
        hr_1->SetTitle("Linear interpolation of yield in LMR");
    }
    hr_1->SetXTitle("m_{X} (GeV)");
    hr_1->GetYaxis()->SetTitleOffset(1.2);
    hr_1->SetYTitle("Normalization");
    c1->GetFrame()->SetBorderSize(12);
    p_2->cd();
    if (flag_MMR) {
        if (range==1){hr_2 = c1->DrawFrame(400,-0.07,1300,0.07);}
        else{hr_2 = c1->DrawFrame(400,-0.07,1300,0.07);}
    } else {
        if (range==1){hr_2 = c1->DrawFrame(250,-0.04,370,0.04);}
        else{hr_2 = c1->DrawFrame(250,-0.04,650,0.04);}
    }
    hr_2->SetXTitle("m_{X} (GeV)");
    hr_2->GetXaxis()->SetTitleOffset(0.6);
    hr_2->GetXaxis()->SetTitleSize(0.08);
    hr_2->GetYaxis()->SetTitleOffset(0.5);
    hr_2->GetYaxis()->SetTitleSize(0.08);
    hr_2->SetLabelSize(0.06,"xy");
    hr_2->SetYTitle("Relative errors");    
    c1->GetFrame()->SetBorderSize(12);
    p_1->cd();
        
    TGraphErrors* g_signal_0 = new TGraphErrors(nPoints_0, &(mass_0[0]), signal_0);
    g_signal_0->SetMarkerColor(kRed);
    g_signal_0->SetMarkerStyle(21);
    g_signal_0->SetLineColor(0);
    g_signal_0->Draw("P");
    
    j=0;
    for (unsigned i = 0; i<nPoints; i++) {
        if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){
            interpol[i]=signal_0[j];
            j++;
        }
        else{int lMarker=0;
            for (auto pVal : mass_0){  if (pVal > mass[i]){break;}lMarker++;}
            interpol[i] =( (signal_0[lMarker]-signal_0[lMarker-1])*(mass[i]-mass_0[lMarker-1])/(mass_0[lMarker]-mass_0[lMarker-1])+signal_0[lMarker-1]);
        }
    }

    TGraphErrors* g_signal_2 = new TGraphErrors(nPoints, &(mass[0]), interpol);
    g_signal_2->SetMarkerColor(kBlue);
    g_signal_2->SetMarkerStyle(20);
    g_signal_2->SetLineColor(0);
    g_signal_2->Draw("P");
    g_signal_0->Draw("P");

    TF1 *fit_pol = new TF1("fit_pol",pol_line,100,1000,4);
    fit_pol->SetParameter(0, 1500.);
    fit_pol->SetParameter(1, -13.);
    fit_pol->SetParameter(2, 0.07 );
    fit_pol->SetParameter(3, -1.e-05);
    fit_pol->SetParName(0, "x_0");
    fit_pol->SetParName(1, "x_1");
    fit_pol->SetParName(2, "x_2");
    fit_pol->SetParName(3, "x_3");
    if (flag_MMR) {
        g_signal_0->Fit(fit_pol,"","", 550,1200);
    }
    else{
        if (range==1){
            g_signal_0->Fit(fit_pol,"","",260,400);}
        else{g_signal_0->Fit(fit_pol,"","",260,650);}
    }
 
    TGraphErrors* g_pull = new TGraphErrors(nPoints);
    g_pull->SetMarkerStyle(7);
    g_pull->SetMarkerSize(0.4);
    
    for (unsigned i = 0; i<nPoints; i++) {
        bool trovato = false;
        for (double m : mass_0) {
            if (m == mass[i]) {
                trovato=true;
            }
        }
        if (!trovato) {
            double temp = (interpol[i]-fit_pol->Eval(mass[i]))/fit_pol->Eval(mass[i]);
            g_pull->SetPoint(i, mass[i],temp);
        }
    }     
        
    TLegend *leg=new TLegend(0.2, 0.65, 0.65, 0.85);
    leg->SetFillStyle(1); leg->SetFillColor(kWhite);
    leg->AddEntry(g_signal_0, "Norm for MC masses", "lep");
    leg->AddEntry(g_signal_2, "Interpolation", "lep");
    leg->SetFillColor(kWhite);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);
    leg->SetTextFont(42);
    leg->SetBorderSize(0);
    leg->Draw();

    p_2->cd();
    TLine* line;
    g_pull->Draw("P"); 
    if (flag_MMR) {
        line= new TLine(350, 0, 1200, 0);
    }
    else{
        line= new TLine(250, 0, 550, 0);
    }
    line->SetLineStyle(2);
    line->SetLineWidth(1);
    line->Draw();
         
    if (flag_MMR) {
        c1->SaveAs("interpolation_yield_MMR.pdf");
        c1->SaveAs("interpolation_yield_MMR.png");
        
    } else {
        c1->SaveAs(Form("interpolation_yield_LMR_%d.pdf",range));
        c1->SaveAs(Form("interpolation_yield_LMR_%d.png",range));   
    }
    
    for (unsigned i = 0; i<nPoints; i++) {
        
        std::string mass_string= itoa(mass[i]);
        std::string filename=dir+"_"+mass_string+"_"+background+"/datacard_"+mass_string+"_"+background+".txt";
        
        std::ifstream file(filename.c_str(), ios::in);
        
        bool found= false;
        std::string line;
        double background_temp=0.;                
        

        while (!found && !file.eof()) {
            getline(file, line);
            std::size_t pos = line.find("rate");
            if (pos!=std::string::npos){
                found=true;
                background_temp = atof((line.substr(line.find_last_of(" "))).c_str());
            }
            
        }
                
        if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){
            continue;
        }
        
        int lMarker=0;
        for (auto m : mass_0){
            if (m > mass[i]){
                break;
            }
            lMarker++;
        }
        gSystem->Exec(Form("sed -i 's/.*rate.*/rate            %f   %f   %f /g' %s_%d_%s/datacard_%d_%s.txt", interpol[i], background_temp,interpol[i], dir.c_str(), int(mass[i]), background.c_str(), int(mass[i]), background.c_str()));
    }
    cout<<"end"<<endl;


    //Interpolate bias
    TCanvas *c3 = new TCanvas("canvas","canvas",700,700);
    TPad *p_3=new TPad("p_3", "p_3", 0, 0, 1, 1);
    p_3->SetFillStyle(4000);
    p_3->SetFrameFillColor(0);
    p_3->SetBottomMargin(0.035);  
    p_3->Draw();
    
    TH1F* hr_3;
    if (flag_MMR) {
        if (range==1){hr_3 = c3->DrawFrame(400,-0.5,1400,0.5);}
        else{hr_3 = c3->DrawFrame(400,0.5,1400,0.5);}
        hr_3->SetTitle("Interpolation of bias in MMR");
    } else {
        if (range==1){hr_3 = c3->DrawFrame(250,-0.5,370,0.5);}
        else{hr_3 = c3->DrawFrame(250,-0.5,650,0.5);}
        hr_3->SetTitle("Interpolation of bias in LMR");
    }
    hr_3->SetXTitle("m_{X} (GeV)");
    hr_3->GetYaxis()->SetTitleOffset(1.2);
    hr_3->SetYTitle("Bias");
    c3->GetFrame()->SetBorderSize(12);
    
    TGraphErrors* g_signal_0_bias = new TGraphErrors(nPoints_0, &(mass_0[0]), signal_0_bias);
    g_signal_0_bias->SetMarkerColor(kRed);
    g_signal_0_bias->SetMarkerStyle(21);
    g_signal_0_bias->SetLineColor(0);
    
    //Code for Spline
    for (unsigned i = 0; i<nPoints_0; i++) {
            sp_bias_0[i]= signal_0_bias[i];
            cout<<"This is the spline mc input (bias): "<<sp_bias_0[i]<<endl;
    }
    //sp_bias = new TSpline3(Form("Spline_bias_%d_%d",flag_MMR,range), &(mass_0[0]), sp_bias_0, nPoints_0, "b2e2", 0, 0);
    sp_bias = new TSpline3(Form("Spline_bias_%d_%d",flag_MMR,range), &(mass_0[0]), sp_bias_0, nPoints_0, "b2e2", 0, 0);
    sp_bias->SetLineColor(kGreen+2);  
     
    int k=0;
    for (unsigned i = 0; i<nPoints; i++) {
            if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){
                interpol_bias_0[i]=signal_0_bias[k];
                cout<<"This is the mc (bias): "<<interpol_bias_0[i]<<endl;
                k++;
            }
            else{
                interpol_bias_0[i]=sp_bias->Eval(mass[i]);
                cout<<"This is the interpolation (bias): "<<interpol_bias_0[i]<<endl;
            }

    }

    TGraphErrors* g_signal_2_bias = new TGraphErrors(nPoints, &(mass[0]), interpol_bias_0);
    g_signal_2_bias->SetMarkerColor(kBlue);
    g_signal_2_bias->SetMarkerStyle(20);
    g_signal_2_bias->SetLineColor(0);
    g_signal_2_bias->Draw("P");
    g_signal_0_bias->Draw("P");
    sp_bias->Draw("lsame");    
    
    TLegend *leg_bias=new TLegend(0.2, 0.65, 0.65, 0.85);
    leg_bias->SetFillStyle(1); leg_bias->SetFillColor(kWhite);
    leg_bias->AddEntry(g_signal_0_bias, "Bias for MC masses", "lep");
    leg_bias->AddEntry(g_signal_2_bias, "Interpolation", "lep");
    leg_bias->AddEntry(sp_bias, "Spline Interpolation", "l");    
    leg_bias->SetFillColor(kWhite);
    leg_bias->SetFillStyle(0);
    leg_bias->SetTextSize(0.03);
    leg_bias->SetTextFont(42);
    leg_bias->SetBorderSize(0);
    leg_bias->Draw();
         
    if (flag_MMR) {
        c3->SaveAs("interpolation_bias_MMR.pdf");
        c3->SaveAs("interpolation_bias_MMR.png");
        
    } else {
        c3->SaveAs(Form("interpolation_bias_LMR_%d.pdf",range));
        c3->SaveAs(Form("interpolation_bias_LMR_%d.png",range));   
    }

    //Change the bias value in datacards
    for (unsigned i = 0; i<nPoints; i++) {
        if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){ continue;}
        std::string mass_string= itoa(mass[i]);
        std::string filename_bias=dir+"_"+mass_string+"_"+background+"/datacard_"+mass_string+"_"+background+".txt";    
        std::ifstream file_bias(filename_bias.c_str(), ios::in);
        gSystem->Exec(Form("sed -i 's/shapeBkg_signal_bkg_HbbHbb__norm.*param.*/shapeBkg_signal_bkg_HbbHbb__norm param  0.0 %f /g' %s_%d_%s/datacard_%d_%s.txt", interpol_bias_0[i], dir.c_str(), int(mass[i]), background.c_str(), int(mass[i]), background.c_str()));
        file_bias.close();
    }
    cout<<"end"<<endl;

    TCanvas *c2[NumOfSyst];
    TPad *p_syst1[NumOfSyst];
    TPad *p_syst2[NumOfSyst];
    TH1F* h_syst_1[NumOfSyst];
    TH1F* h_syst_2[NumOfSyst];
    TGraphErrors* g_syst_0[NumOfSyst];
    TGraphErrors* g_syst_1[NumOfSyst];  
    TF1 *fit_pol_syst[NumOfSyst];
    TGraphErrors* g_pull_syst[NumOfSyst];
    TLegend* leg_syst[NumOfSyst];
    TSpline3* sp_syst[NumOfSyst];

    for (int syst_num=0; syst_num<NumOfSyst; syst_num++) {
        c2[syst_num]= new TCanvas(Form("canvas_syst_%d",syst_num),Form("canvas_syst_%d",syst_num),700,700);
        c2[syst_num]->SetGrid();
        c2[syst_num]->GetFrame()->SetBorderSize(12);
        p_syst1[syst_num] = new TPad(Form("p_syst1_%d",syst_num), Form("p_syst1_%d",syst_num), 0, 0.3, 1, 1);
        p_syst2[syst_num] = new TPad(Form("p_syst2_%d",syst_num), Form("p_syst2_%d",syst_num), 0,0,1,0.3);
        p_syst1[syst_num]->SetFillStyle(4000);
        p_syst1[syst_num]->SetFrameFillColor(0);
        p_syst1[syst_num]->SetBottomMargin(0.035);
        p_syst2[syst_num]->SetBottomMargin(0.2);
        p_syst2[syst_num]->SetTopMargin(0.02);
        p_syst2[syst_num]->SetFillColor(0);
        p_syst2[syst_num]->SetBorderMode(0);
        p_syst2[syst_num]->SetBorderSize(2);
        p_syst2[syst_num]->SetFrameBorderMode(0);
        p_syst2[syst_num]->SetFrameBorderMode(0);
        c2[syst_num]->cd();
        p_syst1[syst_num]->Draw();
        p_syst2[syst_num]->Draw();
        p_syst1[syst_num]->cd();
        if (flag_MMR) {
            if (range==1){
                h_syst_1[syst_num] = c2[syst_num]->DrawFrame(500,1,1250,1.10);
            }
            else{
                h_syst_1[syst_num] = c2[syst_num]->DrawFrame(500,1,1250,1.10);
            }
            h_syst_1[syst_num]->SetTitle(Form("Interpolation of %s in MMR",name_syst[syst_num].c_str()));
        } else {
            if (range==1){
                switch (syst_num) {
                    case 0: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,1.0,370,1.30); break;
                    case 1: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,0.97,370,1.07); break;
                    case 2: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,0.95,370,1.05); break;
                    case 3: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,1.0,370,1.05); break;
                    case 4: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,1.05,370,1.15); break;
                }
            }
            else{
                switch (syst_num) {
                    case 0: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,1.0,650,1.30); break;
                    case 1: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,0.97,650,1.07); break;
                    case 2: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,0.95,650,1.05); break;
                    case 3: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,1,650,1.05); break;
                    case 4: h_syst_1[syst_num] = c2[syst_num]->DrawFrame(250,1.05,650,1.15); break;
                }

            }
            h_syst_1[syst_num]->SetTitle(Form("Interpolation of %s in LMR",name_syst[syst_num].c_str()));
        }
        h_syst_1[syst_num]->SetXTitle("m_{X} (GeV)");
        h_syst_1[syst_num]->GetYaxis()->SetTitleOffset(1.2);
        h_syst_1[syst_num]->SetYTitle(Form("%s",name_syst[syst_num].c_str()));
        p_syst2[syst_num]->cd();
        if (flag_MMR) {
            if (range==1){h_syst_2[syst_num] = c2[syst_num]->DrawFrame(500,-0.07,1250,0.07);}
            else{h_syst_2[syst_num] = c2[syst_num]->DrawFrame(500,-0.07,1100,0.07);}
        } else {
            if (range==1){
                switch (syst_num) {
                    case 1: h_syst_2[syst_num] = c2[syst_num]->DrawFrame(250,-0.0004,370,0.0004); break;
                    case 3: h_syst_2[syst_num] = c2[syst_num]->DrawFrame(250,-0.0004,370,0.0004); break;
                    default:
                        h_syst_2[syst_num] = c2[syst_num]->DrawFrame(250,-0.004,370,0.004); break;
                }
            }
            else{
                h_syst_2[syst_num] = c2[syst_num]->DrawFrame(250,-0.002,650,0.002);
            }
        }
        h_syst_2[syst_num]->SetXTitle("m_{X} (GeV)");
        h_syst_2[syst_num]->GetXaxis()->SetTitleOffset(0.6);
        h_syst_2[syst_num]->GetXaxis()->SetTitleSize(0.08);
        h_syst_2[syst_num]->GetYaxis()->SetTitleOffset(0.5);
        h_syst_2[syst_num]->GetYaxis()->SetTitleSize(0.08);
        h_syst_2[syst_num]->SetLabelSize(0.06,"xy");
        h_syst_2[syst_num]->SetYTitle("Relative errors (Pol. fit)");
        
        g_syst_0[syst_num] = new TGraphErrors(nPoints_0, &(mass_0[0]), syst_0[syst_num]);
        g_syst_0[syst_num]->SetMarkerColor(kRed);
        g_syst_0[syst_num]->SetMarkerStyle(21);
        g_syst_0[syst_num]->SetLineColor(0);
        c2[syst_num]->cd();
        p_syst1[syst_num]->cd();
        g_syst_0[syst_num]->Draw("P");


        //Code for Spline  
        for (unsigned i = 0; i<nPoints_0; i++) {
            sp_syst_0[i]= syst_0[syst_num][i];
            cout<<"This is the spline mc input: "<<sp_syst_0[i]<<endl;
        }
        sp_syst[syst_num] = new TSpline3(Form("Spline_syst_%d",syst_num), &(mass_0[0]), sp_syst_0, nPoints_0, "b2e2", 0, 0);
        sp_syst[syst_num]->SetLineColor(kGreen+2);   
        j=0;
        for (unsigned i = 0; i<nPoints; i++) {
            if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){
                interpol_syst_0[syst_num][i]=syst_0[syst_num][j];
                cout<<"This is the mc: "<<interpol_syst_0[syst_num][i]<<endl;
                j++;
            }
            else{
                interpol_syst_0[syst_num][i]=sp_syst[syst_num]->Eval(mass[i]);
                cout<<"This is the interpolation: "<<interpol_syst_0[syst_num][i]<<endl;
            }

        }
         

        g_syst_1[syst_num] = new TGraphErrors(nPoints, &(mass[0]), interpol_syst_0[syst_num]);
        g_syst_1[syst_num]->SetMarkerColor(kBlue);
        g_syst_1[syst_num]->SetMarkerStyle(20);
        g_syst_1[syst_num]->SetLineColor(0);
        g_syst_1[syst_num]->Draw("P");
        g_syst_0[syst_num]->Draw("P");
        
        fit_pol_syst[syst_num] = new TF1(Form("fit_pol_syst_%d", syst_num),Form("pol%d",nPoints_0-1),100,1200);
        fit_pol_syst[syst_num]->SetParameter(0, 1.);
        fit_pol_syst[syst_num]->SetParameter(1, -13.);
        fit_pol_syst[syst_num]->SetParameter(2, 0.07 );
        fit_pol_syst[syst_num]->SetParameter(3, -1.e-05);
        fit_pol_syst[syst_num]->SetParName(0, "x_0");
        fit_pol_syst[syst_num]->SetParName(1, "x_1");
        fit_pol_syst[syst_num]->SetParName(2, "x_2");
        fit_pol_syst[syst_num]->SetParName(3, "x_3");
        sp_syst[syst_num]->Draw("lsame");
        if (flag_MMR) {g_syst_0[syst_num]->Fit(fit_pol_syst[syst_num],"","", 550,1250);}
        else{
            if (range==1){g_syst_0[syst_num]->Fit(fit_pol_syst[syst_num],"","",260,350);}
            else{g_syst_0[syst_num]->Fit(fit_pol_syst[syst_num],"","",260,650);}
        }       

        //Pull for interpolated errors
        g_pull_syst[syst_num] = new TGraphErrors(nPoints);
        g_pull_syst[syst_num]->SetMarkerStyle(7);
        g_pull_syst[syst_num]->SetMarkerSize(1.4);
        
        for (unsigned i = 0; i<nPoints; i++) {
            bool trovato = false;
            for (double m : mass_0) {if (m == mass[i]) {trovato=true;}}
            if (!trovato) {
                double temp = (interpol_syst_0[syst_num][i]-fit_pol_syst[syst_num]->Eval(mass[i]))/fit_pol_syst[syst_num]->Eval(mass[i]);
                g_pull_syst[syst_num]->SetPoint(i, mass[i],temp);
            }
        }
        
        leg_syst[syst_num] =new TLegend(0.5, 0.65, 0.82, 0.85);
        leg_syst[syst_num]->SetFillStyle(1); leg->SetFillColor(kWhite);
        leg_syst[syst_num]->AddEntry(g_syst_0[syst_num], Form("%s for MC points", name_syst[syst_num].c_str()), "lep");
        leg_syst[syst_num]->AddEntry(g_syst_1[syst_num], Form("%s for interpolated points", name_syst[syst_num].c_str()), "lep");
        leg_syst[syst_num]->AddEntry(sp_syst[syst_num], "Spline Interpolation", "l");
        leg_syst[syst_num]->AddEntry(fit_pol_syst[syst_num], "Polynomial Fit", "l");               
        leg_syst[syst_num]->SetFillColor(kWhite);
        leg_syst[syst_num]->SetFillStyle(0);
        leg_syst[syst_num]->SetTextSize(0.03);
        leg_syst[syst_num]->SetTextFont(42);
        leg_syst[syst_num]->SetBorderSize(0);
        leg_syst[syst_num]->Draw();
        p_syst2[syst_num]->cd();
        g_pull_syst[syst_num]->Draw("P");
        if (flag_MMR) {
            if (range==1){
                line= new TLine(350, 0, 1250, 0);
            }
            else{
                line= new TLine(350, 0, 125-0, 0);
            }
        }
        else{
            if (range==1){
                line= new TLine(250, 0, 370, 0);
            }
            else{
                line= new TLine(250, 0, 650, 0);
            }
            
        }
        line->SetLineStyle(2);
        line->SetLineWidth(1);
        line->Draw();
        
        if (flag_MMR) {
            c2[syst_num]->SaveAs(Form("%s_interpolation_MMR_%d.pdf", name_syst[syst_num].c_str(), range));
            c2[syst_num]->SaveAs(Form("%s_interpolation_MMR_%d.png", name_syst[syst_num].c_str(), range));
        } else {
            c2[syst_num]->SaveAs(Form("%s_interpolation_LMR_%d.pdf", name_syst[syst_num].c_str(), range));
            c2[syst_num]->SaveAs(Form("%s_interpolation_LMR_%d.png", name_syst[syst_num].c_str(), range));
        }
        
        for (unsigned i = 0; i<nPoints; i++) {
            if (std::find(mass_0.begin(), mass_0.end(), mass[i]) != mass_0.end()){ continue;}
            std::string mass_string= itoa(mass[i]);
            std::string filename=dir+"_"+mass_string+"_"+background+"/datacard_"+mass_string+"_"+background+".txt";
            std::ifstream file_syst(filename.c_str(), ios::in);
            gSystem->Exec(Form("sed -i 's/%s.*lnN.*/%s      lnN     %f    -    -  /g' %s_%d_%s/datacard_%d_%s.txt", name_syst[syst_num].c_str(), name_syst[syst_num].c_str(), interpol_syst_0[syst_num][i], dir.c_str(), int(mass[i]), background.c_str(), int(mass[i]), background.c_str()));
            file_syst.close();
        }
        
    }
}


//================================================================================

Double_t straight_line(Double_t *x, Double_t *par)
{
    Double_t m,q; // Exponential slope
    
    q = par[0];
    m = par[1];
    
    return q + m*x[0];
}

//================================================================================

Double_t pol_line(Double_t *x, Double_t *par)
{
    Double_t x_0,x_1,x_2,x_3; // Exponential slope
    
    x_0 = par[0];
    x_1 = par[1];
    x_2 = par[2];
    x_3 = par[3];
    
    return x_0 + x_1*x[0]+ x_2*x[0]*x[0]+ x_3*x[0]*x[0]*x[0];
}

//================================================================================

void PrintArray(Double_t *v, Int_t dim, Int_t precision){
    int i;
    for (i = 0; i < dim; i++){
        switch (precision) {
            case 0: printf("v[%d]: %.0f\n", i, v[i]); break;
            case 1: printf("v[%d]: %.1f\n", i, v[i]); break;
            case 2: printf("v[%d]: %.2f\n", i, v[i]); break;
            case 3: printf("v[%d]: %.3f\n", i, v[i]); break;
            case 4: printf("v[%d]: %.4f\n", i, v[i]); break;
            case 5: printf("v[%d]: %.5f\n", i, v[i]); break;
            case 6: printf("v[%d]: %.6f\n", i, v[i]); break;
            case 7: printf("v[%d]: %.7f\n", i, v[i]); break;
            default : printf("v[%d]: %.8f\n", i, v[i]); break;
        }
    }
    cout << endl;
}

//================================================================================

std::string itoa(int i)
{
    char res[4];
    sprintf(res, "%d", i);
    std::string ret(res);
    return ret;
}

//================================================================================
