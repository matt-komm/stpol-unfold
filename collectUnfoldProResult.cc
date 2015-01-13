#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TStyle.h"

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

Asymmetry loadFromFile(std::string file,TH1D* input=nullptr,TH1D* unfolded=nullptr)
{
    TFile f(file.c_str());
    TH1D* data = (TH1D*)f.Get("substractedData");

    if (input!=nullptr)
    {
        input->Add(data);
    }
    TH1D* hist = (TH1D*)f.Get("unfolded");
    if (unfolded!=nullptr)
    {
        unfolded->Add(hist);
    }
    TH2D* cov = (TH2D*)f.Get("error");  
    
    Asymmetry asy = estimateAsymmetry(hist,cov);
    f.Close();
    return asy;
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
    
    Asymmetry nominalA = loadFromFile(nominalFile,nominalHist,nominalHistUnfolded);
    double mean = nominalA.mean;
    double up = -1;
    double down = -1;
    if (fromError)
    {
        up=mean+nominalA.uncertainty*0.5;
        down=mean-nominalA.uncertainty*0.5;
    }
    else
    {
        Asymmetry upA = loadFromFile(upFile,upHist,upHistUnfolded);
        Asymmetry downA = loadFromFile(downFile,downHist,downHistUnfolded);
        up=upA.mean;
        down=downA.mean;
        gStyle->SetOptStat(0);
        gStyle->SetLabelSize(0.045,"XYZ");
        TCanvas cv("cv","",400,600);
        cv.Divide(1,2);
        cv.cd(1);
        TH2D axis1("axis1","",50,-1,1,50,0,1.1*std::max({nominalHist->GetMaximum(),upHist->GetMaximum(),downHist->GetMaximum()}));
        axis1.Draw("AXIS");
        nominalHist->SetMarkerStyle(21);
        nominalHist->Draw("SameLP");
        upHist->SetLineColor(kRed+1);
        upHist->SetLineWidth(2);
        upHist->Draw("SameLHISTP");
        downHist->SetLineColor(kGreen+1);
        downHist->SetLineWidth(2);
        downHist->Draw("SameLHISTP");
        
        cv.cd(2);
        TH2D axis2("axis2","",50,-1,1,50,0,1.1*std::max({nominalHistUnfolded->GetMaximum(),upHistUnfolded->GetMaximum(),downHistUnfolded->GetMaximum()}));
        axis2.Draw("AXIS");
        nominalHistUnfolded->SetMarkerStyle(21);
        nominalHistUnfolded->Draw("SameLP");
        upHistUnfolded->SetLineColor(kRed+1);
        upHistUnfolded->SetLineWidth(2);
        upHistUnfolded->Draw("SameLHISTP");
        downHistUnfolded->SetLineColor(kGreen+1);
        downHistUnfolded->SetLineWidth(2);
        downHistUnfolded->Draw("SameLHISTP");
        
        cv.Print((output+".pdf").c_str());
        
    }
    
    
    
    std::ofstream ouputFile(output.c_str());
    ouputFile<<"\"syst\",\"up\",\"down\",\"dup\",\"ddown\",\"d\""<<std::endl;
    ouputFile<<("\""+sysName+"\",").c_str()<<mean<<","<<up<<","<<down<<","<<fabs(mean-up)
             <<","<<fabs(mean-down)<<","<<std::max(fabs(mean-down),fabs(mean-up))<<std::endl;
    ouputFile.close();
    //std::cout<<nominalA.mean<<" +- "<<nominalA.uncertainty<<std::endl;
    
    
    return 0;
}
