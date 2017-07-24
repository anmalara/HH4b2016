#include <TROOT.h>
#include "TSystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TAxis.h"
#include "TLatex.h"
#include "../../TDRStyle.h"
#include "../../CMS_lumi.h"
#include <iostream>
int iPeriod = 4;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV 
int iPos = 11;

void 
CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{            
  bool outOfFrame    = false;
  if( iPosX/10==0 ) 
    {
      outOfFrame = true;
    }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  if( iPosX == 0  ) relPosX = 0.14;
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();

  pad->cd();

  TString lumiText;
  if( iPeriod==1 )
    {
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==2 )
    {
      lumiText += lumi_8TeV;
      lumiText += " (8 TeV)";
    }
  else if( iPeriod==3 ) 
    {
      lumiText = lumi_8TeV; 
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==4 )
    {
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
    }
  else if ( iPeriod==7 )
    { 
      if( outOfFrame ) lumiText += "#scale[0.85]{";
      lumiText += lumi_13TeV; 
      lumiText += " (13 TeV)";
      lumiText += " + ";
      lumiText += lumi_8TeV; 
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
      if( outOfFrame) lumiText += "}";
    }
  else if ( iPeriod==12 )
    {
      lumiText += "8 TeV";
    }
  else if ( iPeriod==0 )
    {
      lumiText += lumi_sqrtS;
    }
   
  std::cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);    

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex.SetTextFont(42);
  latex.SetTextAlign(31); 
  latex.SetTextSize(lumiTextSize*t);    
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11); 
      latex.SetTextSize(cmsTextSize*t);    
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }
  
  pad->cd();

  float posX_=0;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame )
    {
      if( drawLogo )
	{
	  posX_ =   l + 0.045*(1-l-r)*W/H;
	  posY_ = 1-t - 0.045*(1-t-b);
	  float xl_0 = posX_;
	  float yl_0 = posY_ - 0.15;
	  float xl_1 = posX_ + 0.15*H/W;
	  float yl_1 = posY_;
	  TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
	  TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
	  pad_logo->Draw();
	  pad_logo->cd();
	  CMS_logo->Draw("X");
	  pad_logo->Modified();
	  pad->cd();
	}
      else
	{
	  latex.SetTextFont(cmsTextFont);
	  latex.SetTextSize(cmsTextSize*t);
	  latex.SetTextAlign(align_);
	  latex.DrawLatex(posX_, posY_, cmsText);
	  if( writeExtraText ) 
	    {
	      latex.SetTextFont(extraTextFont);
	      latex.SetTextAlign(align_);
	      latex.SetTextSize(extraTextSize*t);
	      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
	    }
	}
    }
  else if( writeExtraText )
    {
      if( iPosX==0) 
	{
	  posX_ =   l +  relPosX*(1-l-r);
	  posY_ =   1-t+lumiTextOffset*t;
	}
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize*t);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);      
    }
  return;
}


std::string itoa(int i) 
{
  char res[4];
  sprintf(res, "%d", i);
  std::string ret(res);
  return ret;
}

void DrawLimitPlot_All(std::vector<double> mass_1,
	std::vector<double> mass_2,
	std::vector<double> mass_3, 
	double ymin, double ymax, 
	std::string background_1,
	std::string background_2,
	std::string background_3){
	    gStyle->SetTitleOffset(1.2,"Y");
		gStyle->SetPadLeftMargin(0.18);
		gStyle->SetPadBottomMargin(0.15);
		gStyle->SetPadTopMargin(0.08);
		gStyle->SetPadRightMargin(0.05);
		gStyle->SetMarkerSize(0.5);
		gStyle->SetHistLineWidth(1);
		gStyle->SetStatFontSize(0.020);
		gStyle->SetTitleSize(0.06, "XYZ");
		gStyle->SetLabelSize(0.05, "XYZ");
		gStyle->SetNdivisions(510, "XYZ");
		gStyle->SetLegendBorderSize(0);
		gStyle->SetPadBorderMode(0);
		gStyle->SetFrameBorderMode(0);
		gStyle->SetPadBottomMargin(0.12);
		gStyle->SetPadLeftMargin(0.12);
		gStyle->SetCanvasColor(kWhite);
		gStyle->SetCanvasDefH(600);
		gStyle->SetCanvasDefW(600);
		gStyle->SetCanvasDefX(0);
		gStyle->SetCanvasDefY(0);

		gStyle->SetPadTopMargin(0.05);
		gStyle->SetPadBottomMargin(0.15);
		gStyle->SetPadLeftMargin(0.15);
		gStyle->SetPadRightMargin(0.05);

		gStyle->SetPadBorderMode(0);
		gStyle->SetPadColor(kWhite);
		gStyle->SetGridColor(0);
		gStyle->SetGridStyle(3);
		gStyle->SetGridWidth(1);

		gStyle->SetFrameBorderMode(0);
		gStyle->SetFrameBorderSize(1);
		gStyle->SetFrameFillColor(0);
		gStyle->SetFrameFillStyle(0);
		gStyle->SetFrameLineColor(1);
		gStyle->SetFrameLineStyle(1);
		gStyle->SetFrameLineWidth(1);

		gStyle->SetTitleColor(1, "XYZ");
		gStyle->SetTitleFont(42, "XYZ");
		gStyle->SetTitleSize(0.05, "XYZ");
		gStyle->SetTitleXOffset(1.15);
		gStyle->SetTitleYOffset(1.3) ;
		gStyle->SetLabelColor(1, "XYZ");
		gStyle->SetLabelFont(42, "XYZ");
		gStyle->SetLabelOffset(0.007, "XYZ");
		gStyle->SetLabelSize(0.045, "XYZ");

		gStyle->SetPadBorderMode(0);
		gStyle->SetFrameBorderMode(0);
		gStyle->SetTitleTextColor(1);
		gStyle->SetTitleFillColor(10);
		gStyle->SetTitleFontSize(0.05);

    writeExtraText = true;       
	extraText  = "Preliminary";  
	lumi_13TeV  = "35.9 fb^{-1}";

	// LMR1 definitions
	const unsigned int nPoints_1=mass_1.size();
	double xsec_1[nPoints_1], xsecNeg1_1[nPoints_1], xsecPos1_1[nPoints_1], xsecNeg2_1[nPoints_1], xsecPos2_1[nPoints_1];
	double obs_1[nPoints_1];
	double expNeg2_1[nPoints_1], expNeg1_1[nPoints_1], expPos1_1[nPoints_1], expPos2_1[nPoints_1];
	double norm_1=1000;
	// LMR2 definitions
	const unsigned int nPoints_2=mass_2.size();
	double xsec_2[nPoints_2], xsecNeg1_2[nPoints_2], xsecPos1_2[nPoints_2], xsecNeg2_2[nPoints_2], xsecPos2_2[nPoints_2];
	double obs_2[nPoints_2];
	double expNeg2_2[nPoints_2], expNeg1_2[nPoints_2], expPos1_2[nPoints_2], expPos2_2[nPoints_2];
	double norm_2=1000;

	const unsigned int nPoints_3=mass_3.size();
	double xsec_3[nPoints_3], xsecNeg1_3[nPoints_3], xsecPos1_3[nPoints_3], xsecNeg2_3[nPoints_3], xsecPos2_3[nPoints_3];
	double obs_3[nPoints_3];
	double expNeg2_3[nPoints_3], expNeg1_3[nPoints_3], expPos1_3[nPoints_3], expPos2_3[nPoints_3];
	double norm_3=2;	

    // LMR1 LOOP
	for (unsigned int i=0; i<nPoints_1; ++i)
	{
		std::string mass_string_1=itoa(mass_1[i]);
		std::string filename_1;
		filename_1="LMR/LMR_"+mass_string_1+"_"+background_1+"/CMS_HH4b_"+mass_string_1+"_13TeV_asymptoticCLs.out";
		std::ifstream file_1(filename_1.c_str(), ios::in);
		std::string line_1;
        bool found_1= false;
        while (!found_1 && !file_1.eof() && !gSystem->AccessPathName(filename_1.c_str())) {
                          getline(file_1, line_1);
                          std::size_t pos = line_1.find("-- Asymptotic --");  
                          if (pos!=std::string::npos){
                          found_1=true;
                          }
        }
		getline(file_1, line_1); obs_1[i]=atof(line_1.substr(line_1.find("<")+1).c_str())*norm_1;
		getline(file_1, line_1); xsecNeg2_1[i]=atof(line_1.substr(line_1.find("<")+1).c_str())*norm_1;
		getline(file_1, line_1); xsecNeg1_1[i]=atof(line_1.substr(line_1.find("<")+1).c_str())*norm_1;
		getline(file_1, line_1); xsec_1[i]=atof(line_1.substr(line_1.find("<")+1).c_str())*norm_1;
		getline(file_1, line_1); xsecPos1_1[i]=atof(line_1.substr(line_1.find("<")+1).c_str())*norm_1;
		getline(file_1, line_1); xsecPos2_1[i]=atof(line_1.substr(line_1.find("<")+1).c_str())*norm_1;
		expNeg2_1[i]=xsec_1[i]-xsecNeg2_1[i];
		expNeg1_1[i]=xsec_1[i]-xsecNeg1_1[i];
		expPos1_1[i]=xsecPos1_1[i]-xsec_1[i];
		expPos2_1[i]=xsecPos2_1[i]-xsec_1[i];
	}

    // LMR2 LOOP
	for (unsigned int i=0; i<nPoints_2; ++i)
	{
		std::string mass_string_2=itoa(mass_2[i]);
		std::string filename_2;
		filename_2="LMR/LMR_"+mass_string_2+"_"+background_2+"/CMS_HH4b_"+mass_string_2+"_13TeV_asymptoticCLs.out";
		std::ifstream file_2(filename_2.c_str(), ios::in);
		std::string line_2;
        bool found_2= false;
        while (!found_2 && !file_2.eof() && !gSystem->AccessPathName(filename_2.c_str())) {
                          getline(file_2, line_2);
                          std::size_t pos = line_2.find("-- Asymptotic --");  
                          if (pos!=std::string::npos){
                          found_2=true;
                          }
        }
		getline(file_2, line_2); obs_2[i]=atof(line_2.substr(line_2.find("<")+1).c_str())*norm_2;
		getline(file_2, line_2); xsecNeg2_2[i]=atof(line_2.substr(line_2.find("<")+1).c_str())*norm_2;
		getline(file_2, line_2); xsecNeg1_2[i]=atof(line_2.substr(line_2.find("<")+1).c_str())*norm_2;
		getline(file_2, line_2); xsec_2[i]=atof(line_2.substr(line_2.find("<")+1).c_str())*norm_2;
		getline(file_2, line_2); xsecPos1_2[i]=atof(line_2.substr(line_2.find("<")+1).c_str())*norm_2;
		getline(file_2, line_2); xsecPos2_2[i]=atof(line_2.substr(line_2.find("<")+1).c_str())*norm_2;
		expNeg2_2[i]=xsec_2[i]-xsecNeg2_2[i];
		expNeg1_2[i]=xsec_2[i]-xsecNeg1_2[i];
		expPos1_2[i]=xsecPos1_2[i]-xsec_2[i];
		expPos2_2[i]=xsecPos2_2[i]-xsec_2[i];
	}	

    // MMR1 LOOP
	for (unsigned int i=0; i<nPoints_3; ++i)
	{
		std::string mass_string_3=itoa(mass_3[i]);
		std::string filename_3;
		filename_3="MMR/MMR_"+mass_string_3+"_"+background_3+"/CMS_HH4b_"+mass_string_3+"_13TeV_asymptoticCLs.out";
		std::ifstream file_3(filename_3.c_str(), ios::in);
		std::string line_3;
        bool found_3= false;
        while (!found_3 && !file_3.eof() && !gSystem->AccessPathName(filename_3.c_str())) {
                          getline(file_3, line_3);
                          std::size_t pos = line_3.find("-- Asymptotic --");  
                          if (pos!=std::string::npos){
                          found_3=true;
                          }
        }
		getline(file_3, line_3); obs_3[i]=atof(line_3.substr(line_3.find("<")+1).c_str())*norm_3;
		getline(file_3, line_3); xsecNeg2_3[i]=atof(line_3.substr(line_3.find("<")+1).c_str())*norm_3;
		getline(file_3, line_3); xsecNeg1_3[i]=atof(line_3.substr(line_3.find("<")+1).c_str())*norm_3;
		getline(file_3, line_3); xsec_3[i]=atof(line_3.substr(line_3.find("<")+1).c_str())*norm_3;
		getline(file_3, line_3); xsecPos1_3[i]=atof(line_3.substr(line_3.find("<")+1).c_str())*norm_3;
		getline(file_3, line_3); xsecPos2_3[i]=atof(line_3.substr(line_3.find("<")+1).c_str())*norm_3;
		expNeg2_3[i]=xsec_3[i]-xsecNeg2_3[i];
		expNeg1_3[i]=xsec_3[i]-xsecNeg1_3[i];
		expPos1_3[i]=xsecPos1_3[i]-xsec_3[i];
		expPos2_3[i]=xsecPos2_3[i]-xsec_3[i];
	}

	// Graviton curve
	std::vector<double> masses_graviton={
	260,  
		300,  
		350,  
		400,  
		450,  
		500,  
		550,  
		600,  
		650,  
		700,  
		750,  
		800,  
		850,  
		900,
		950,  
		1000,
		1050,
		1100,
		1150,
		1200,
		1250,
		1300,
		1350,
		1400,
		1450,
		1500};

std::vector<double> x_graviton{
	13189.5110556213,
		7213.724953,
		3724.152306612245,
		2074.118283562500,
		1220.908845777777,
		756.0527086799999,
		484.2628197024792,
		321.127891575,
		219.297103860355, 
		152.2164810122449,
		108.4982254720000,
		78.4962635203125 ,
		57.88419000415223,
		43.08418917777778,
		32.69710000221605,
		24.96632111100000,
		19.26116379591836,
		15.04592709917355,
		11.87132725293005,
		9.391763225,
		7.510401695231998,
		6.032381149171598,
		4.878695042469133,
		3.980540729387755,
		3.259530744827586,
		2.6789518692	
};

std::vector<double> br_hh={
7.875333606243001e-6,
	0.0002609363907507743,
	0.0008527793007645482 , 
	0.0014748453016228675,
	0.00202120813640487,
	0.0024744221679741125,
	0.002843812476091045,
	0.003144265384982541,
	0.0033897489806557304,
	0.0035917787224510177, 
	0.003759423688146881,
	0.0038997130095539006, 
	0.004018077826392619,
	0.004118727164348574,   
	0.004204942476367313,   
	0.00427930100894505,   
	0.004343843097937954, 
	0.004400197131035117,  
	0.004449673107888205,
	0.0044933330427632395, 
	0.00453204428524029 , 
	0.004566520194709918,   
	0.004597351400961763, 
	0.004625030011213332, 
	0.00464996849485267,   
	0.004672514523079795
	};

	// Radion curve
	std::vector<double> masses_radion={
	260,  
		300,
		400,
		500,
		600,
		700,
		750,
		800,
		900,
	   1000,
       1500};

    std::vector<double> x_radion{
    10.314910442540885,	
    7.20114376233762,	
    3.369927115848731,	
    1.962250361817225,	
    1.235987766884442,	
    0.8043402650432466,	
    0.6557538821852814,	
    0.5434525134607641,	
    0.3735615328391298,	
	0.2618915924170746,	
	0.054417137017588976};


// Multiply graviton x-sec with appropriate Br(G->HH)*Br(H->bb)^2 * (k'/k)^2
for(int i=0; i<masses_graviton.size(); ++i) 
{
	x_graviton[i]=x_graviton[i]*br_hh[i]*(0.57*0.57)*1000. ; 
}

// Multiply graviton x-sec with appropriate Br(G->HH)*Br(H->bb)^2 * (k'/k)^2
for(int i=0; i<masses_radion.size(); ++i) 
{
	x_radion[i]=x_radion[i]*0.23*(0.57*0.57)*1000.; 
}

// Graviton curve from github
std::vector<double> masses_graviton_new={260, 300, 400, 500, 600, 700, 750, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
std::vector<double> x_graviton_new={6.87e+00, 1.36e+02, 2.32e+02, 1.23e+02, 5.73e+01, 2.84e+01, 2.04e+01, 1.50e+01, 8.16e+00, 4.74e+00, 3.00e+00, 1.90e+00, 1.20e+00, 7.63e-01, 4.83e-01};
//Radion curve from github
std::vector<double> masses_radion_new={260, 300, 400, 500, 600, 700, 750, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
std::vector<double> x_radion_new={7.64e+03, 7.13e+03, 2.92e+03, 1.50e+03, 9.08e+02, 5.84e+02, 4.74e+02, 3.93e+02, 2.70e+02, 1.90e+02, 1.38e+02, 1.01e+02, 7.38e+01, 5.39e+01, 3.93e+01};


TGraph *g_graviton=new TGraph(masses_graviton_new.size(), &(masses_graviton_new[0]), &(x_graviton_new[0])); g_graviton->SetLineWidth(2); g_graviton->SetLineColor(kBlue+1); g_graviton->SetFillColor(kWhite);
TGraph *g_radion=new TGraph(masses_radion_new.size(), &(masses_radion_new[0]), &(x_radion_new[0])); g_radion->SetLineWidth(2); g_radion->SetLineColor(kBlue+1); g_radion->SetFillColor(kWhite);
TStyle *tdrStyle=setTDRStyle();
gROOT->SetStyle("Plain");

// LMR1 flag 
TGraphErrors *g_xsec_1=new TGraphErrors(nPoints_1, &(mass_1[0]), xsec_1);
g_xsec_1->SetTitle("; m_{X} (GeV); #sigma(pp#rightarrowX) #times Br(X#rightarrowH(b#bar{b}) H(b#bar{b})) (fb)");
g_xsec_1->SetLineWidth(2);
g_xsec_1->SetLineStyle(2);
TGraphAsymmErrors *g_xsec_1sigma_1=new TGraphAsymmErrors(nPoints_1, &(mass_1[0]), xsec_1, 0, 0, expNeg1_1, expPos1_1);
g_xsec_1sigma_1->SetLineColorAlpha(kGreen+3,0.5);
g_xsec_1sigma_1->SetFillColorAlpha(kGreen+3,0.5);
TGraphAsymmErrors *g_xsec_2sigma_1=new TGraphAsymmErrors(nPoints_1, &(mass_1[0]), xsec_1, 0, 0, expNeg2_1, expPos2_1);
g_xsec_2sigma_1->SetLineColorAlpha(kYellow+3,0.5);
g_xsec_2sigma_1->SetFillColorAlpha(kYellow+3,0.5);
//TGraph *g_obs=new TGraph(nPoints, &(mass[0]), obs);
//g_obs->SetLineWidth(2);
//g_obs->SetLineStyle(1);

// LMR2 flag 
TGraphErrors *g_xsec_2=new TGraphErrors(nPoints_2, &(mass_2[0]), xsec_2);
g_xsec_2->SetTitle("; m_{X} (GeV); #sigma(pp#rightarrowX) #times Br(X#rightarrowH(b#bar{b}) H(b#bar{b})) (fb)");
g_xsec_2->SetLineWidth(2);
g_xsec_2->SetLineStyle(2);
TGraphAsymmErrors *g_xsec_1sigma_2=new TGraphAsymmErrors(nPoints_2, &(mass_2[0]), xsec_2, 0, 0, expNeg1_2, expPos1_2);
g_xsec_1sigma_2->SetLineColorAlpha(kGreen+1, 0.5);
g_xsec_1sigma_2->SetFillColorAlpha(kGreen+1, 0.5);
TGraphAsymmErrors *g_xsec_2sigma_2=new TGraphAsymmErrors(nPoints_2, &(mass_2[0]), xsec_2, 0, 0, expNeg2_2, expPos2_2);
g_xsec_2sigma_2->SetLineColorAlpha(kYellow+1, 0.5);
g_xsec_2sigma_2->SetFillColorAlpha(kYellow+1, 0.5);
//TGraph *g_obs=new TGraph(nPoints, &(mass[0]), obs);
//g_obs->SetLineWidth(2);
//g_obs->SetLineStyle(1);

// MMR flag 
TGraphErrors *g_xsec_3=new TGraphErrors(nPoints_3, &(mass_3[0]), xsec_3);
g_xsec_3->SetTitle("; m_{X} (GeV); #sigma(pp#rightarrowX) #times Br(X#rightarrowH(b#bar{b}) H(b#bar{b})) (fb)");
g_xsec_3->SetLineWidth(2);
g_xsec_3->SetLineStyle(2);
TGraphAsymmErrors *g_xsec_1sigma_3=new TGraphAsymmErrors(nPoints_3, &(mass_3[0]), xsec_3, 0, 0, expNeg1_3, expPos1_3);
g_xsec_1sigma_3->SetLineColorAlpha(kGreen, 0.5);
g_xsec_1sigma_3->SetFillColorAlpha(kGreen, 0.5);
TGraphAsymmErrors *g_xsec_2sigma_3=new TGraphAsymmErrors(nPoints_3, &(mass_3[0]), xsec_3, 0, 0, expNeg2_3, expPos2_3);
g_xsec_2sigma_3->SetLineColorAlpha(kYellow, 0.5);
g_xsec_2sigma_3->SetFillColorAlpha(kYellow, 0.5);
//TGraph *g_obs=new TGraph(nPoints, &(mass[0]), obs);
//g_obs->SetLineWidth(2);
//g_obs->SetLineStyle(1);



TCanvas *c_xsec=new TCanvas("c_xsec", "c_xsec", 1000, 700);
c_xsec->SetLogy();
c_xsec->SetGridx(1); c_xsec->SetGridy(1);
g_xsec_2->SetMaximum(ymax); g_xsec_2->SetMinimum(ymin);
g_xsec_2->Draw("AL");
g_xsec_2sigma_2->Draw("3");
g_xsec_1sigma_2->Draw("3");
g_xsec_2->GetXaxis()->SetLimits(200,1250);
g_xsec_2->Draw("L");
g_xsec_1->DrawClone("L same");
g_xsec_2sigma_1->DrawClone("3 same");
g_xsec_1sigma_1->DrawClone("3 same");
g_xsec_1->DrawClone("L same");
g_xsec_3->DrawClone("L same");
g_xsec_2sigma_3->DrawClone("3same");
g_xsec_1sigma_3->DrawClone("3same");
g_xsec_3->Draw("L same");
g_graviton->Draw("C same");
//g_radion->Draw("C same");
//g_obs_1->SetMarkerStyle(20);	
//g_obs_1->Draw("LP SAME");
c_xsec->Update();

TLegend *leg=new TLegend(0.45, 0.65, 0.90, 0.85);
leg->SetFillStyle(1); leg->SetFillColor(kWhite);
leg->AddEntry(g_xsec_1, "Expected Upper Limit", "L");
leg->AddEntry(g_xsec_1sigma_1, "Expected #pm 1 #sigma (LMR [250,330] GeV)", "F");
leg->AddEntry(g_xsec_2sigma_1, "Expected #pm 2 #sigma (LMR [250,330] GeV)", "F");
leg->AddEntry(g_xsec_1sigma_2, "Expected #pm 1 #sigma (LMR [285,625] GeV)", "F");
leg->AddEntry(g_xsec_2sigma_2, "Expected #pm 2 #sigma (LMR [285,625] GeV)", "F");
leg->AddEntry(g_xsec_1sigma_3, "Expected #pm 1 #sigma (MMR [550,1200] GeV)", "F");
leg->AddEntry(g_xsec_2sigma_3, "Expected #pm 2 #sigma (MMR [550,1200] GeV)", "F");
//leg->AddEntry(g_obs, "Observed Upper Limit", "LP");
leg->AddEntry(g_graviton,"Bulk Graviton k=0.1","L");
//leg->AddEntry(g_radion,"Bulk Radion #lambda= 1 TeV, kl=35","L");	
leg->SetFillColor(kWhite);
leg->SetFillStyle(0);
leg->SetTextSize(0.03);
leg->SetTextFont(42);
leg->SetBorderSize(0);
leg->Draw();
CMS_lumi( c_xsec, iPeriod, iPos );
c_xsec->Update();
c_xsec->SaveAs("UpperLimit_all.png");
c_xsec->SaveAs("UpperLimit_all.root");

TFile *file=new TFile("UpperLimits_xsec_all.root", "RECREATE");
//g_obs_1->Write("g_obs");
g_xsec_1->Write("g_xsec_1");
g_xsec_1sigma_1->Write("g_xsec_1sigma_1");
g_xsec_2sigma_1->Write("g_xsec_2sigma_1");
g_xsec_2->Write("g_xsec_2");
g_xsec_1sigma_2->Write("g_xsec_1sigma_2");
g_xsec_2sigma_2->Write("g_xsec_2sigma_2");
g_xsec_3->Write("g_xsec_3");
g_xsec_1sigma_3->Write("g_xsec_1sigma_3");
g_xsec_2sigma_3->Write("g_xsec_2sigma_3");
file->Close();

}

