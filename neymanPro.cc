#include "SimpleOpt.h"

#include "loadHistogram.hpp"
#include "scanTau.hpp"
#include "logging.hpp"
#include "asymmetryCalculation.hpp"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TROOT.h"

#include "TUnfold.h"
#include "TUnfoldSys.h"

#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <array>
#include <string>
#include <fstream>

static const unsigned int REBIN_RECO=12;
static const unsigned int REBIN_GEN=6;

struct NeymanResult
{
    double vl;
    double vr;
    double genA;
    double unfoldedA;
    double uncertainty;
    double deviation;
};

TH1D generateWeightedHist(TH1D* comphep1000,TH1D* comphep1100,TH1D* comphep0100,double vl,double vr)
{
    double gammaTop=1.49;
    double w1000=gammaTop;
    double wart=gammaTop;
    double w0100=gammaTop;
    double wtot=gammaTop*(pow(vl,2)+pow(vr,2));
    double m=pow(vl,4)*w1000/wtot;
    double n=pow(vl,2)*pow(vr,2)*wart/wtot;
    double k=pow(vr,4)*w0100/wtot;
    TH1D result(*comphep1000);
    result.Scale(0.0);
    result.Add(comphep1000,m);
    result.Add(comphep1100,n);
    result.Add(comphep0100,k);
    return result;
}
    
double calcAsymmetry(TH1* hist)
{
    double sumUp=0.0;
    double sumDown=0.0;
    for (int i=0;i<hist->GetNbinsX();++i)
    {
        if (hist->GetBinCenter(i+1)>0.0)
        {
            sumUp+=hist->GetBinContent(i+1);
        }
        else
        {
            sumDown+=hist->GetBinContent(i+1);
        }
    }
    return (sumUp-sumDown)/(sumUp+sumDown);
}
    
std::vector<NeymanResult> doNeyman(TH2F* responseMatrix, TH2F* comphep1000Matrix, TH2F* comphep1100Matrix, TH2F* comphep0100Matrix, int N, double fixedTau)
{
    TH1D* comphep1000Gen=comphep1000Matrix->ProjectionX("comphep1000Gen");
    TH1D* comphep1100Gen=comphep1100Matrix->ProjectionX("comphep1100Gen");
    TH1D* comphep0100Gen=comphep0100Matrix->ProjectionX("comphep0100Gen");
    
    TH1D* comphep1000Reco=comphep1000Matrix->ProjectionY("comphep1000Reco");
    TH1D* comphep1100Reco=comphep1100Matrix->ProjectionY("comphep1100Reco");
    TH1D* comphep0100Reco=comphep0100Matrix->ProjectionY("comphep0100Reco");

    TUnfoldSys tunfold(responseMatrix,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);
    tunfold.SetBias(responseMatrix->ProjectionX());
    
    TH1F unfoldedHist("unfolded","unfolded", REBIN_GEN, -1, 1);
    TH2D error("errorMatrixTauScan","",REBIN_GEN,0,REBIN_GEN,REBIN_GEN,0,REBIN_GEN);
    
    
    std::vector<NeymanResult> results;

    for (int i = 0; i<N;++i)
    {
        
        double vl=1.0;
        double vr=1.0*i/(N-1);
        
        TH1D genDist=generateWeightedHist(comphep1000Gen,comphep1100Gen,comphep0100Gen,vl,vr);
        TH1D recoDist=generateWeightedHist(comphep1000Reco,comphep1100Reco,comphep0100Reco,vl,vr);
        for (int i=0;i<REBIN_RECO;++i)
        {
            recoDist.SetBinError(i+1,sqrt(recoDist.GetBinContent(i+1)));
        }
        if (fixedTau>0.0)
        {
            tunfold.DoUnfold(fixedTau,&recoDist,1.0);
        }
        else
        {
            ScanResult scanResult = scanTau(responseMatrix,&recoDist,false);
            tunfold.DoUnfold(scanResult.taumean,&recoDist,1.0);
        }
        unfoldedHist.Scale(0.0);
        error.Scale(0.0);
	    tunfold.GetOutput(&unfoldedHist);
	    tunfold.GetEmatrix(&error);
	    Asymmetry res = estimateAsymmetry(&unfoldedHist,&error);
	    
	    NeymanResult point;
        point.vl=vl;
        point.vr=vr;
        point.genA=calcAsymmetry(&genDist);
        point.unfoldedA=res.mean;
        point.uncertainty=res.uncertainty;
        point.deviation=fabs(point.genA-point.unfoldedA);
        
        results.push_back(point);
    }
    return results;
}


enum {
    OPT_RESPONSEFILE,
    OPT_RESPONSEMATRIXNAME,
    OPT_COMPHEP1000,
    OPT_COMPHEP1100,
    OPT_COMPHEP0100,
    OPT_OUTPUT,
    OTP_FIXEDTAU,
    OTP_SCANTAU
};

CSimpleOpt::SOption options[] ={
    {OPT_RESPONSEFILE,"--responseFile",SO_REQ_SEP},
    {OPT_RESPONSEMATRIXNAME,"--responseMatrixName",SO_REQ_SEP},
    {OPT_COMPHEP1000,"--comphep1000Name",SO_REQ_SEP},
    {OPT_COMPHEP1100,"--comphep1100Name",SO_REQ_SEP},
    {OPT_COMPHEP0100,"--comphep0100Name",SO_REQ_SEP},
    {OPT_OUTPUT,"--output",SO_REQ_SEP},
    SO_END_OF_OPTIONS

};

int main(int argc, char* argv[])
{
    CSimpleOpt parser(argc,argv,options);
    
    std::vector<std::string> responseFiles;
    std::string responseMatrixName = "tm";
    std::string comphep1000Name="";
    std::string comphep1100Name="";
    std::string comphep0100Name="";
    std::string outputFile = "out.root";
    
    double fixedTau=-1.0;
    
    gErrorIgnoreLevel = kPrint | kInfo | kWarning;
    
    setupLogging();
    
    while (parser.Next())
    {
        if (parser.LastError() == SO_SUCCESS)
        {
            if (parser.OptionId() == OPT_RESPONSEFILE)
            {
                responseFiles.push_back(parser.OptionArg());
            }
            else if (parser.OptionId() == OPT_RESPONSEMATRIXNAME)
            {
                responseMatrixName=parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_COMPHEP1000)
            {
                comphep1000Name=parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_COMPHEP1100)
            {
                comphep1100Name=parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_COMPHEP0100)
            {
                comphep0100Name=parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_OUTPUT)
            {
                outputFile  = parser.OptionArg();
            }
            else if (parser.OptionId() ==OTP_FIXEDTAU)
            {
                try
                {
                    fixedTau=stod(std::string(parser.OptionArg()));
                } 
                catch (std::exception e)
                {
                    log(ERROR,"while parsing the given fixed tau: %s\n",e.what());
                }
            }
            else if (parser.OptionId() ==OTP_SCANTAU)
            {
                fixedTau=-1.0; //neg value will trigger a scan
            }   
        }
        else
        {
            log(CRITICAL,"Invalid argument: %s\n", parser.OptionText());
            return 1;
        }
    }
    
    log(INFO,"responseFiles: \n");
    for (unsigned int i = 0; i< responseFiles.size();++i)
    {
        log(INFO," ... %s\n",responseFiles[i].c_str());
    }
    log(INFO,"matrix name: %s\n",(responseMatrixName).c_str());
    
    
    TH2F* responseHist = loadHistogram2D(responseFiles,responseMatrixName,"",1.0,REBIN_GEN,REBIN_RECO);
    log(INFO,"A(response,gen)= %f\n",calcAsymmetry(responseHist->ProjectionX()));
    
    TH2F* comphep1000 = loadHistogram2D(responseFiles,comphep1000Name,"",1.0,REBIN_GEN,REBIN_RECO);
    log(INFO,"A(comphep1000,gen)= %f\n",calcAsymmetry(comphep1000->ProjectionX()));
        
    TH2F* comphep1100 = loadHistogram2D(responseFiles,comphep1100Name,"",1.0,REBIN_GEN,REBIN_RECO);
    log(INFO,"A(comphep1100,gen)= %f\n",calcAsymmetry(comphep1100->ProjectionX()));
    
    TH2F* comphep0100 = loadHistogram2D(responseFiles,comphep0100Name,"",1.0,REBIN_GEN,REBIN_RECO);
    log(INFO,"A(comphep0100,gen)= %f\n",calcAsymmetry(comphep0100->ProjectionX()));
    
    std::vector<NeymanResult> neymanResults = doNeyman(responseHist,comphep1000,comphep1100,comphep0100,11,fixedTau);
    
    
    std::ofstream file(outputFile,std::ios_base::trunc | std::ios_base::out);
    file<<"\"vl\",\"vr\",\"generated\",\"unfolded\",\"uncertainty\",\"bias\""<<std::endl;
    for (const NeymanResult& result: neymanResults)
    {
        file<<result.vl<<", "<<result.vr<<", "<<result.genA<<", "<<result.unfoldedA<<", "<<result.uncertainty<<", "<<result.deviation<<std::endl;
    }
    file.close();    
}
