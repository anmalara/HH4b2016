std::string ftoa(double i)
{
    char res[4];
    sprintf(res, "%2.1f", i);
    std::string ret(res);
    return ret;
}

void drawBias_MMR(const double signal_strenght=1.0, std::string Filename="", std::string background_MMR_1="", std::string background_MMR_2="") {

    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    gStyle->SetOptTitle(1);
    gStyle->SetFitFormat("2.2g");
    gStyle->SetPadBottomMargin(0.2);
    
    
    const int nDirs = 1;
    
    Color_t cols[nDirs] = {kBlack};//, kRed+1, kOrange-3, kGreen+1, kAzure+7 , kMagenta};
    
    
    TString dirNames[nDirs] = {background_MMR_1};//,"expow2","pow1","lau1","atlas1","vvdijet1"};
    
    
    const int nPoints = 7;
    int mass[nPoints] ={550, 600, 650, 750, 800, 900, 1000};
    TCanvas* c1[nDirs][nPoints];
    TFile * f[nDirs][nPoints];
    TTree* tree_fit_sb[nDirs][nPoints];
    
    TH1D* hists[nDirs][nPoints];
    TH1D* hists_mean[nDirs][nPoints];
    TH1D* hists_sigma[nDirs][nPoints];
    
    TGraphErrors* biasG[nDirs];
    TGraphErrors* biasG2[nDirs];
    TGraphErrors* sigmaG[nDirs];
    TGraphErrors* sigmaG2[nDirs];
    TGraphErrors* g_err[nDirs];
    TGraphErrors* g_err2[nDirs];
    
    TCanvas* c2 = new TCanvas("p2","p2",700,700);
    TH1F *hr = c2->DrawFrame(450,-1,1200,1);
    hr->SetXTitle("m_{X} (GeV)");
    hr->SetYTitle("mean_{fit}");
    hr->SetTitle(Form("Mean_{fit} for #mu = %2.1f",double(signal_strenght)));
    
    TCanvas* c2a = new TCanvas("c2a","c2a",700,700);
    TH1F *hra = c2a->DrawFrame(450,-1,1200,1);
    hra->SetXTitle("m_{X} (GeV)");
    hra->SetYTitle("#mu-#mu_{inj}");
    hra->SetTitle(Form("Residual for #mu = %2.1f",double(signal_strenght)));

    TCanvas* c3 = new TCanvas("p3","p3",700,700);
    TH1F *hr3 = c3->DrawFrame(450,0,1200,1.5);
    hr3->SetXTitle("m_{X} (GeV)");
    hr3->SetYTitle("#sigma_{fit}");
    hr3->SetTitle(Form("#sigma_{fit} for #mu = %2.1f",double(signal_strenght)));

    TCanvas* c3a = new TCanvas("c3a","c3a",700,700);
    TH1F *hr3a = c2a->DrawFrame(450,0,1200,1.5);
    hr3a->SetXTitle("m_{X} (GeV)");
    hr3a->SetYTitle("#sigma");
    hr3a->SetTitle("RMS of residuals");
    
    TLegend* leg = new TLegend(0.75,0.9,0.9,0.75,"pull mean_fit","BNDC");
    leg->SetTextFont(42);
    TLegend* leg2 = new TLegend(0.75,0.9,0.9,0.75,"mean","BNDC");
    leg2->SetTextFont(42);
    TLegend* leg3 = new TLegend(0.75,0.9,0.9,0.75,"sigma_fit","BNDC");
    leg->SetTextFont(42);
    TLegend* leg4 = new TLegend(0.75,0.9,0.9,0.75,"RMS","BNDC");
    leg2->SetTextFont(42);
    
    for (int dd=0; dd!=1; ++dd) {
        biasG[dd] = new TGraphErrors(nPoints);
        biasG2[dd] = new TGraphErrors(nPoints);
        sigmaG[dd] = new TGraphErrors(nPoints);
        sigmaG2[dd] = new TGraphErrors(nPoints);
        g_err[dd] = new TGraphErrors(nPoints);
        g_err[dd]->SetTitle(Form("Mean for #mu = %2.1f",double(signal_strenght)));
        g_err2[dd] = new TGraphErrors(nPoints);
        g_err2[dd]->SetTitle(Form("Residual for #mu = %2.1f",double(signal_strenght)));
        biasG[dd]->SetMarkerColor(cols[dd]);
        biasG[dd]->SetMarkerStyle(20);
        biasG[dd]->SetLineColor(cols[dd]);
        biasG2[dd]->SetMarkerColor(cols[dd]);
        biasG2[dd]->SetMarkerStyle(20);
        biasG2[dd]->SetLineColor(cols[dd]);
        sigmaG[dd]->SetMarkerColor(cols[dd]);
        sigmaG[dd]->SetMarkerStyle(20);
        sigmaG[dd]->SetLineColor(cols[dd]);
        sigmaG2[dd]->SetMarkerColor(cols[dd]);
        sigmaG2[dd]->SetMarkerStyle(20);
        sigmaG2[dd]->SetLineColor(cols[dd]);
        g_err[dd]->SetLineColor(5);
        g_err[dd]->SetFillColor(5);
        g_err2[dd]->SetLineColor(5);
        g_err2[dd]->SetFillColor(5);
        
        
        leg->AddEntry(biasG[dd],dirNames[dd],"p");
        leg2->AddEntry(biasG2[dd],dirNames[dd],"p");
        leg3->AddEntry(sigmaG[dd],dirNames[dd],"p");
        leg4->AddEntry(sigmaG2[dd],dirNames[dd],"p");
        
        Double_t quantile,prob;
        prob = 0.5;
        
        for (int i=0; i!=nPoints; ++i) {
            
            std::cout<<mass[i]<<std::endl;
            c1[dd][i] = new TCanvas(Form("c_%d_%d",dd,i),Form("c_%d_%d",dd,i), 800, 600);
            TString nameF = Form("mlfit%s_%d.root",(Filename).c_str(), mass[i]);
            if (gSystem->AccessPathName(nameF)) continue;
            f[dd][i] = new TFile(nameF);
            if(!f[dd][i]) continue;
            
            tree_fit_sb[dd][i] = static_cast<TTree*>(f[dd][i]->Get("tree_fit_sb"));
            
            TString name = Form("bias_%d_GeV-%s-%s",mass[i],background_MMR_1.c_str(),background_MMR_2.c_str());
            TString name_mean = Form("bias_mean_%d_GeV-%s-%s",mass[i],background_MMR_1.c_str(),background_MMR_2.c_str());
            TString name_sigma = Form("bias_sigma_%d_GeV-%s-%s",mass[i],background_MMR_1.c_str(),background_MMR_2.c_str());
            TString title = Form("bias at %d GeV for ",mass[i])+background_MMR_1+"-"+background_MMR_2+";#frac{#mu_{fit}-"+ftoa(signal_strenght)+"}{#sigma_{fit}};entries/0.25";
            TString title_mean = Form("bias at %d GeV for ",mass[i])+background_MMR_1+"-"+background_MMR_2+"mu_{fit}-"+ftoa(signal_strenght)+";entries/0.25";
            TString title_sigma = Form("bias at %d GeV for ",mass[i])+background_MMR_1+"-"+background_MMR_2+";#sigma_{fit};entries/0.25";
            
            hists[dd][i] = new TH1D(name,title,100,-20,20);
            hists_mean[dd][i] = new TH1D(name_mean,title_mean,100,-10,10);
            hists_sigma[dd][i] = new TH1D(name_sigma,title_sigma,100,-0.,2.);
            hists[dd][i]->SetMarkerColor(kBlack);
            hists_mean[dd][i]->SetMarkerColor(kBlack);
            hists_sigma[dd][i]->SetMarkerColor(kBlack);
            hists[dd][i]->SetLineColor(kBlack);
            hists_mean[dd][i]->SetLineColor(kBlack);
            hists_sigma[dd][i]->SetLineColor(kBlack);
            hists[dd][i]->SetMarkerStyle(20);
            hists_mean[dd][i]->SetMarkerStyle(20);
            hists_sigma[dd][i]->SetMarkerStyle(20);
            hists[dd][i]->GetXaxis()->SetTitleOffset(1.5);
            hists_mean[dd][i]->GetXaxis()->SetTitleOffset(1.5);
            hists_sigma[dd][i]->GetXaxis()->SetTitleOffset(1.5);
            hists[dd][i]->GetYaxis()->SetTitleOffset(1.5);
            hists_mean[dd][i]->GetYaxis()->SetTitleOffset(1.5);
            hists_sigma[dd][i]->GetYaxis()->SetTitleOffset(1.5);
            
            tree_fit_sb[dd][i]->Draw(Form("(mu-%f)*2/(muLoErr+muHiErr)>>"+name,double(signal_strenght)),Form("( fit_status>=0 && (mu-%f)< 5*muHiErr &&(mu-%f)>-5*muLoErr) && abs((mu-%f)*2/(muLoErr+muHiErr)) < 5 ", double(signal_strenght), double(signal_strenght), double(signal_strenght)),"pe");
            hists[dd][i]->Fit("gaus","LM","",-2,2);
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s.png",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s.root",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));

            c1[dd][i]->Clear();

            tree_fit_sb[dd][i]->Draw(Form("(mu-%f)>>"+name_mean,double(signal_strenght)),Form("( fit_status>=0 && (mu-%f)< 5*muHiErr &&(mu-%f)>-5*muLoErr) && abs((mu-%f)*2/(muLoErr+muHiErr)) < 5 ", double(signal_strenght), double(signal_strenght), double(signal_strenght)),"pe");
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s_mean.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s_mean.png",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s_mean.root",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->Clear();


            tree_fit_sb[dd][i]->Draw(Form("(muLoErr+muHiErr)/2>>"+name_sigma,double(signal_strenght)),Form("( fit_status>=0 && (mu-%f)< 5*muHiErr &&(mu-%f)>-5*muLoErr) && abs((mu-%f)*2/(muLoErr+muHiErr)) < 5 ", double(signal_strenght), double(signal_strenght), double(signal_strenght)),"pe");
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s_sigma.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s_sigma.png",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->SaveAs(Form("%s_%s_mass_%d_strenght_%s_sigma.root",background_MMR_1.c_str(),background_MMR_2.c_str(),mass[i],(ftoa(signal_strenght)).c_str()));
            c1[dd][i]->Clear();

            biasG[dd]->SetPoint(i,mass[i],hists[dd][i]->GetFunction("gaus")->GetParameter(1));
            biasG[dd]->SetPointError(i,0,hists[dd][i]->GetFunction("gaus")->GetParameter(2)/sqrt(hists[dd][i]->GetEntries()));
            sigmaG[dd]->SetPoint(i,mass[i],hists[dd][i]->GetFunction("gaus")->GetParameter(2));
            sigmaG[dd]->SetPointError(i,0,hists[dd][i]->GetFunction("gaus")->GetParError(2));
            
            biasG2[dd]->SetPoint(i,mass[i],hists_mean[dd][i]->GetMean());
            biasG2[dd]->SetPointError(i,0,hists_mean[dd][i]->GetMeanError());
            sigmaG2[dd]->SetPoint(i,mass[i],hists_mean[dd][i]->GetStdDev());
            sigmaG2[dd]->SetPointError(i,0,hists_mean[dd][i]->GetStdDevError());

            g_err[dd]->SetPoint(i,mass[i],0);
            g_err[dd]->SetPointError(i,0, hists_sigma[dd][i]->GetMean());
            g_err2[dd]->SetPoint(i,mass[i],0);
            g_err2[dd]->SetPointError(i,0, hists_mean[dd][i]->GetStdDev());

            std::ofstream myfile;
            myfile.open(Form("latex_bias_%s_%s_%s.txt",(ftoa(signal_strenght)).c_str(), background_MMR_1.c_str(),background_MMR_2.c_str()), ios::app);
            myfile << signal_strenght << " " << mass[i] << " " << hists_mean[dd][i]->GetMean() << " " << hists_mean[dd][i]->GetMeanError() << " " << hists_mean[dd][i]->GetStdDev() << endl;
            myfile.close();
            
            
        }
        TLine *line=new TLine(500, 0, 1200, 0);
        line->SetLineWidth(2);
        line->SetLineStyle(2);
        TLine *line_range_1=new TLine(500, -0.1, 1200, -0.1);
        line_range_1->SetLineWidth(2);
        line_range_1->SetLineStyle(2);
        TLine *line_range_2=new TLine(500, 0.1, 1200, 0.1);
        line_range_2->SetLineWidth(2);
        line_range_2->SetLineStyle(2);

        c2->cd();
        biasG[dd]->Draw("pe");
        //g_err[dd]->Draw("3");
        biasG[dd]->Draw("pe same");
        line->Draw();
        line_range_1->Draw();
        line_range_2->Draw();
        c2a->cd();
        biasG2[dd]->Draw("pe");
        g_err2[dd]->Draw("3 same");
        biasG2[dd]->Draw("pe same");
        line->Draw();

        TLine *line2=new TLine(500, 1, 1200, 1);
        line2->SetLineWidth(2);
        line2->SetLineStyle(2);

        c3->cd();
        sigmaG[dd]->Draw("pe same");
        line2->Draw();
        c3a->cd();
        sigmaG2[dd]->Draw("pe same");
        line2->Draw();

        
    }
    c2->cd();
    leg->Draw("same");
    c2->SaveAs(Form("%s_%s_strenght_%s.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c2->SaveAs(Form("%s_%s_strenght_%s.png",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c2->SaveAs(Form("%s_%s_strenght_%s.root",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c2a->cd();
    leg2->Draw("same");
    //c2a->SaveAs(Form("%s_%s_a_strenght_%s.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    //c2a->SaveAs(Form("%s_%s_a_strenght_%s.png",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    //c2a->SaveAs(Form("%s_%s_a_strenght_%s.root",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c3->cd();
    leg3->Draw("same");
    c3->SaveAs(Form("%s_%s_strenght_%s_sigma.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c3->SaveAs(Form("%s_%s_strenght_%s_sigma.png",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c3->SaveAs(Form("%s_%s_strenght_%s_sigma.root",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    c3a->cd();
    leg4->Draw("same");
    //c3a->SaveAs(Form("%s_%s_a_strenght_%s_sigma.pdf",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    //c3a->SaveAs(Form("%s_%s_a_strenght_%s_sigma.png",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    //c3a->SaveAs(Form("%s_%s_a_strenght_%s_sigma.root",background_MMR_1.c_str(),background_MMR_2.c_str(),(ftoa(signal_strenght)).c_str()));
    
}

