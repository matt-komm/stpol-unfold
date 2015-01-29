#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TF1.h"

#include "SimpleOpt.h"
#include "logging.hpp"
#include "asymmetryCalculation.hpp"
#include "TCanvas.h"
#include <fstream>
#include <iostream>

enum {
    OPT_NOMINAL,
    OPT_UP,
    OPT_DOWN,
    OPT_SYS,
    OPT_ERROR,
    OPT_OUTPUT,
};

CSimpleOpt::SOption options[] ={
    {OPT_NOMINAL, "--nominal", SO_REQ_SEP},
    {OPT_SYS, "--sys", SO_REQ_SEP},
    {OPT_UP,"--up",SO_REQ_SEP},
    {OPT_DOWN,"--down",SO_REQ_SEP},
    {OPT_ERROR,"--fromError",SO_NONE},
    {OPT_OUTPUT,"--output",SO_REQ_SEP},
    SO_END_OF_OPTIONS

};

Asymmetry loadFromFile(std::string file,TH1D* input=nullptr,TH1D* unfolded=nullptr, TH2D* covHist = nullptr)
{
    TFile f(file.c_str());
    TH1D* data = (TH1D*)f.Get("substractedData");

    if (input!=nullptr)
    {
        input->Add(data);
    }
    TH1D* hist = (TH1D*)f.Get("unfolded");
    
    double scale = hist->Integral()*2.0/hist->GetNbinsX();
    hist->Scale(1.0/scale);
    TH2D* cov = (TH2D*)f.Get("error");  
    cov->Scale(1.0/(scale*scale));
    
    if (unfolded!=nullptr)
    {
        unfolded->Add(hist);
    }
    if (covHist!=nullptr)
    {
        covHist->Add(cov);
    }
    Asymmetry asy = estimateAsymmetry(unfolded,cov);
    f.Close();
    return asy;
}

void smooth(TH1* hist, double asymmetry, int N=2, float alpha=0.3)
{
    TH1* new_hist=(TH1*)hist->Clone();
    for (int iter = 0; iter<N; ++iter)
    {
        for (int i = 0; i < hist->GetNbinsX(); ++i)
        {
            double n1=hist->GetBinContent(i)-0.5-asymmetry*(hist->GetBinCenter(i));
            double n2=hist->GetBinContent(i+1)-0.5-asymmetry*(hist->GetBinCenter(i+1));
            double n3=hist->GetBinContent(i+2)-0.5-asymmetry*(hist->GetBinCenter(i+2));
            
            if (i==0)
            {
                n1=n1*0;
                n2=n2*(1.0-0.5*alpha);
                n3=n3*0.5*alpha;
            }
            else if (i==hist->GetNbinsX()-1)
            {
                n1=n1*0.5*alpha;
                n2=n2*(1.0-0.5*alpha);
                n3=n3*0;
            }
            else
            {
                n1=n1*0.5*alpha;
                n2=n2*(1-alpha);
                n3=n3*0.5*alpha;
            }
            
            new_hist->SetBinContent(i+1,n1+n2+n3+0.5+asymmetry*(hist->GetBinCenter(i+1)));
        }
        for (int i = 0; i < hist->GetNbinsX(); ++i)
        {
            hist->SetBinContent(i+1,new_hist->GetBinContent(i+1));
        }
    }
}


int main(int argc, char* argv[])
{
    gErrorIgnoreLevel = kPrint | kInfo | kWarning;
    std::string nominalFile = "";
    std::string sysName = "";
    std::string upFile = "";
    std::string downFile = "";
    std::string output = "";
    
    bool fromError=false;
    try
    {
        CSimpleOpt parser(argc,argv,options);
        
        
        while (parser.Next())
        {
            if (parser.LastError() == SO_SUCCESS)
            {
                if (parser.OptionId() == OPT_NOMINAL) 
                {
                    nominalFile=parser.OptionArg();
                }
                else if (parser.OptionId() == OPT_UP) 
                {
                    upFile=parser.OptionArg();
                }
                else if (parser.OptionId() == OPT_DOWN) 
                {
                    downFile=parser.OptionArg();
                }
                else if (parser.OptionId() == OPT_ERROR) 
                {
                    fromError=true;
                }
                else if (parser.OptionId() == OPT_SYS) 
                {
                    sysName=parser.OptionArg();
                }
                else if (parser.OptionId() == OPT_OUTPUT) 
                {
                    output=parser.OptionArg();
                }
            }
            else
            {
                log(CRITICAL,"Invalid argument: %s\n", parser.OptionText());
                return 1;
            }
        }
        if (nominalFile.size()==0)
        {
            throw std::string("nominal file name is empty");
        }
        if (sysName.size()==0)
        {
            throw std::string("sysname not defined");
        }
        if (output.size()==0)
        {
            throw std::string("output file name is empty");
        }
        if (!fromError)
        {
            
            if (upFile.size()==0)
            {
                throw std::string("up file is empty");
            }
            if (downFile.size()==0)
            {
                throw std::string("down file is empty");
            }
            
        }
        
    }
    catch (std::string s)
    {
        std::cout<<"Exception: "<<s.c_str()<<std::endl;
        return -1;
    }

    
    TH1D* nominalHist=new TH1D("nominal","",12,-1,1);
    TH1D* upHist=new TH1D("up","",12,-1,1);
    TH1D* downHist=new TH1D("down","",12,-1,1);
    
    TH1D* nominalHistUnfolded=new TH1D("nominalUnfolded","",6,-1,1);
    TH1D* upHistUnfolded=new TH1D("upUnfolded","",6,-1,1);
    TH1D* downHistUnfolded=new TH1D("downUnfolded","",6,-1,1);
    
    TH2D* nominalCov=new TH2D("nominalCov","",6,1,6,6,1,6);
    TH2D* upCov=new TH2D("upCov","",6,1,6,6,1,6);
    TH2D* downCov=new TH2D("downCov","",6,1,6,6,1,6);
    
    
    Asymmetry nominalA = loadFromFile(nominalFile,nominalHist,nominalHistUnfolded,nominalCov);
    double mean = nominalA.mean;
    double up = -1;
    double down = -1;
    if (fromError)
    {
        up=mean+nominalA.uncertainty;
        down=mean-nominalA.uncertainty;
    }
    else
    {
        Asymmetry upA = loadFromFile(upFile,upHist,upHistUnfolded,upCov);
        Asymmetry downA = loadFromFile(downFile,downHist,downHistUnfolded,downCov);
        
        if (sysName=="generator")
        {
            downHist->Scale(-1.0);
            downHist->Add(nominalHist,2.0);
            
            downHistUnfolded->Scale(-1.0);
            downHistUnfolded->Add(nominalHistUnfolded,2.0);
            
            downA=estimateAsymmetry(downHistUnfolded,downCov);
        }
        
        up=upA.mean;
        down=downA.mean;
        gStyle->SetOptStat(0);
        gStyle->SetLabelSize(20,"XYZ");
        gStyle->SetLabelFont(43,"XYZ");
        gStyle->SetTitleSize(28,"XYZ");
        gStyle->SetTitleFont(43,"XYZ");
        gStyle->SetTitleOffset(1.1,"X");
        gStyle->SetTitleOffset(1.4,"Y");
        gStyle->SetTickLength(0.015/(1-0.135-0.02),"X");
        gStyle->SetTickLength(0.015/(1-0.5),"Y");
        
        TLegend legend(0.17,0.96,0.5,0.8);
        legend.SetTextFont(43);
        legend.SetTextSize(20);
        legend.SetTextAlign(12);
        legend.SetBorderSize(0);
        legend.SetFillColor(0);
        
        legend.AddEntry(nominalHist,"nominal","PE");
        legend.AddEntry(upHist,(sysName+" up").c_str(),"L");
        legend.AddEntry(downHist,(sysName+" down").c_str(),"L");
        
        TCanvas cv1("cv1","",600,600);
        cv1.Divide(1,2,0,0);
        cv1.GetPad(1)->SetPad(0.0, 0.0, 1.0, 1.0);
        cv1.GetPad(1)->SetFillStyle(0);
        cv1.GetPad(1)->SetLeftMargin(0.135);
        cv1.GetPad(1)->SetBottomMargin(0.57);
        cv1.GetPad(1)->SetTopMargin(0.02);
        cv1.GetPad(1)->SetRightMargin(0.02);
        cv1.GetPad(1)->SetTickx(1);
        cv1.GetPad(1)->SetTicky(1);
        
        cv1.GetPad(2)->SetPad(0.0, 0.0, 1.0, 1.0);
        cv1.GetPad(2)->SetFillStyle(0);
        cv1.GetPad(2)->SetLeftMargin(0.135);
        cv1.GetPad(2)->SetBottomMargin(0.12);
        cv1.GetPad(2)->SetTopMargin(0.43);
        cv1.GetPad(2)->SetRightMargin(0.02);
        cv1.GetPad(2)->SetTickx(1);
        cv1.GetPad(2)->SetTicky(1);
        
        cv1.cd(1);
        
        TH2D axis1("axis1",";reco cos#theta_{l}*;events / 0.17",50,-1,1,50,0,1.1*std::max({nominalHist->GetMaximum(),upHist->GetMaximum(),downHist->GetMaximum()}));
        axis1.GetXaxis()->SetTitleSize(0);
        axis1.GetXaxis()->SetLabelSize(0);
        axis1.Draw("AXIS");
        
        nominalHist->SetLineColor(kBlack);
        nominalHist->SetLineWidth(2);
        nominalHist->SetMarkerStyle(21);
        nominalHist->Draw("SameLP");
        upHist->SetLineColor(kOrange+10);
        upHist->SetLineWidth(2);
        upHist->Draw("SameLHISTP");
        downHist->SetLineColor(kAzure-4);
        downHist->SetLineWidth(2);
        downHist->Draw("SameLHISTP");
        legend.Draw("Same");
        
        
        cv1.cd(2);
        
        
        TH1* nominalHistRes=(TH1*)nominalHist->Clone("nominalHistRes");
        for (int i = 0; i < nominalHist->GetNbinsX(); ++i)
        {
            nominalHistRes->SetBinError(i+1,nominalHistRes->GetBinError(i+1)/nominalHistRes->GetBinContent(i+1));
            nominalHistRes->SetBinContent(i+1,1.0);
        }
        
        double range1 = std::fabs(nominalHistRes->GetBinError(2));
        TH2D axis1res("axis1res",";reco cos#theta_{l}*;sys / nominal",50,-1,1,50,1.0-1.15*range1,1.0+1.15*range1);
        axis1res.Draw("AXIS");
        
        nominalHistRes->Draw("SameL");
        TH1* upHistRes=(TH1*)upHist->Clone("upHistRes");
        upHistRes->Divide(nominalHist);
        upHistRes->Draw("SameLHIST");
        TH1* downHistRes=(TH1*)downHist->Clone("downHistRes");
        downHistRes->Divide(nominalHist);
        downHistRes->Draw("SameLHIST");
        cv1.Print((output+"_input.pdf").c_str());
        
        TCanvas cv2("cv2","",600,600);
        cv2.Divide(1,2,0,0);
        cv2.GetPad(1)->SetPad(0.0, 0.0, 1.0, 1.0);
        cv2.GetPad(1)->SetFillStyle(0);
        cv2.GetPad(1)->SetLeftMargin(0.135);
        cv2.GetPad(1)->SetBottomMargin(0.57);
        cv2.GetPad(1)->SetTopMargin(0.02);
        cv2.GetPad(1)->SetRightMargin(0.02);
        cv2.GetPad(1)->SetTickx(1);
        cv2.GetPad(1)->SetTicky(1);
        
        cv2.GetPad(2)->SetPad(0.0, 0.0, 1.0, 1.0);
        cv2.GetPad(2)->SetFillStyle(0);
        cv2.GetPad(2)->SetLeftMargin(0.135);
        cv2.GetPad(2)->SetBottomMargin(0.12);
        cv2.GetPad(2)->SetTopMargin(0.43);
        cv2.GetPad(2)->SetRightMargin(0.02);
        cv2.GetPad(2)->SetTickx(1);
        cv2.GetPad(2)->SetTicky(1);
        cv2.cd(1);
        
        TH2D axis2("axis2",";unfolded cos#theta_{l}*;events / 0.33",50,-1,1,50,0,1.1*std::max({nominalHistUnfolded->GetMaximum(),upHistUnfolded->GetMaximum(),downHistUnfolded->GetMaximum()}));
        axis2.GetXaxis()->SetTitleSize(0);
        axis2.GetXaxis()->SetLabelSize(0);
        axis2.Draw("AXIS");
        
        nominalHistUnfolded->SetLineColor(kBlack);
        nominalHistUnfolded->SetLineWidth(2);
        nominalHistUnfolded->SetMarkerStyle(21);
        nominalHistUnfolded->Draw("SameLP");
        
        upHistUnfolded->SetLineColor(kOrange+10);
        upHistUnfolded->SetLineWidth(2);
        upHistUnfolded->Draw("SameLHIST");
        
        TF1 upFit("upFit","[0]+x*[1]",-1,1);
        upFit.SetParameter(0,0.5);
        upFit.SetParameter(1,upA.mean);
        
        TH1* upSmooth = (TH1*)upHistUnfolded->Clone();
        smooth(upSmooth,upA.mean);
        Asymmetry upSmoothA=estimateAsymmetry(upSmooth,upCov);
        upSmooth->SetLineStyle(3);
        upSmooth->Draw("SameLHIST");
        
        downHistUnfolded->SetLineColor(kAzure-4);
        downHistUnfolded->SetLineWidth(2);
        downHistUnfolded->Draw("SameLHIST");
        
        TF1 downFit("downFit","[0]+x*[1]",-1,1);
        downFit.SetParameter(0,0.5);
        downFit.SetParameter(1,downA.mean);
        
        TH1* downSmooth = (TH1*)downHistUnfolded->Clone();
        smooth(downSmooth,downA.mean);
        Asymmetry downSmoothA=estimateAsymmetry(downSmooth,downCov);
        downSmooth->SetLineStyle(3);
        downSmooth->Draw("SameLHIST");
        
        legend.Draw("Same");
        
        cv2.cd(2);
        TH1* nominalHistUnfoldedRes = (TH1*)nominalHistUnfolded->Clone("downHistUnfoldedRes");
        for (int i = 0; i < nominalHistUnfolded->GetNbinsX(); ++i)
        {
            nominalHistUnfoldedRes->SetBinError(i+1,nominalHistUnfoldedRes->GetBinError(i+1)/nominalHistUnfoldedRes->GetBinContent(i+1));
            nominalHistUnfoldedRes->SetBinContent(i+1,1.0);
        }
        //nominalHistUnfoldedRes->Divide(nominalHistUnfolded);
        TH1* upHistUnfoldedRes = (TH1*)upHistUnfolded->Clone("upHistUnfoldedRes");
        upHistUnfoldedRes->Divide(nominalHistUnfolded);
        TH1* downHistUnfoldedRes = (TH1*)downHistUnfolded->Clone("downHistUnfoldedRes");
        downHistUnfoldedRes->Divide(nominalHistUnfolded);
        
        double range2 = std::fabs(nominalHistUnfoldedRes->GetBinError(2));
        TH2D axis2res("axis2res",";unfolded cos#theta_{l}*;sys / nominal",50,-1,1,50,1.0-1.15*range2,1.0+1.15*range2);
        axis2res.Draw("AXIS");
        
        nominalHistUnfoldedRes->Draw("SameL");
        
        TF1 upFitRes("upFitRes","[0]+x*[1]",-1,1);
        upFitRes.SetLineColor(kOrange+10);
        upFitRes.SetLineStyle(2);
        upFitRes.SetLineWidth(2);
        upFitRes.SetParameter(0,1.0);
        upFitRes.SetParameter(1,upA.mean-nominalA.mean);
        upFitRes.Draw("SameL");
        
        
        upHistUnfoldedRes->Draw("SameLHIST");
        
        TH1* upSmoothRes = (TH1*)upSmooth->Clone();
        upSmoothRes->Divide(nominalHistUnfolded);
        upSmoothRes->Draw("SameLHIST");
        
        
        TF1 downFitRes("downFitRes","[0]+x*[1]",-1,1);
        downFitRes.SetLineColor(kAzure-4);
        downFitRes.SetLineStyle(2);
        downFitRes.SetLineWidth(2);
        downFitRes.SetParameter(0,1.0);
        downFitRes.SetParameter(1,downA.mean-nominalA.mean);
        downFitRes.Draw("SameL");
        
        TH1* downSmoothRes = (TH1*)downSmooth->Clone();
        downSmoothRes->Divide(nominalHistUnfolded);
        downSmoothRes->Draw("SameLHIST");
        
        
        downHistUnfoldedRes->Draw("SameLHIST");

        cv2.Print((output+"_unfolded.pdf").c_str());
        
        up=upSmoothA.mean;
        down=downSmoothA.mean;
        
    }
    
    
    
    std::ofstream ouputFile(output.c_str());
    ouputFile<<"\"syst\",\"mean\",\"up\",\"down\",\"dup\",\"ddown\",\"d\""<<std::endl;
    ouputFile<<("\""+sysName+"\",").c_str()<<mean<<","<<up<<","<<down<<","<<(up-mean)
             <<","<<(down-mean)<<","<<std::max(fabs(mean-up),fabs(mean-down))<<std::endl;
    ouputFile.close();
    //std::cout<<nominalA.mean<<" +- "<<nominalA.uncertainty<<std::endl;
    
    
    return 0;
}
