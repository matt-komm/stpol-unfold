#include "SimpleOpt.h"
#include "utils.hpp"
#include "info.hpp"
#include "unfold.hpp"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "TUnfold.h"
#include "TUnfoldSys.h"


#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <string>
#include <fstream>

static const std::string DATA="DATA";
static const std::string SIGNAL="tchan";
static const std::set<std::string> BACKGROUND={"ttjets","wzjets","qcd"};
static const std::string HISTPREFIX="2j1t_cos_theta_lj__";
static const unsigned int REBIN_RECO=12;
static const unsigned int REBIN_GEN=6;

typedef std::map<std::string,std::array<float,2>> FitResult;

bool verbose = false;


TH1F* loadHistogram1D(
    const std::vector<std::string>& fileList, 
    const std::string& name, 
    const std::string& sys, 
    double scale,
    int rebin
    )
{
    TH1F* result = nullptr;
    for (const std::string& file: fileList)
    {
        TFile input(file.c_str(),"r");
        
        std::string histName;
        std::string histNameAlternative=name;
        if (sys=="nominal")
        {
            histName=name;
        }
        else
        {
            histName=name+"__"+sys;
        }
        TH1F* hist = static_cast<TH1F*>(input.Get(histName.c_str()));
        if (!hist)
        {
           printf("WARNING: using fallback '%s' for '%s'\n",histNameAlternative.c_str(),histName.c_str());
           hist = static_cast<TH1F*>(input.Get(histNameAlternative.c_str()));
           if (!hist)
           {
               printf("ERROR: fallback '%s' not found in file '%s'\n",histNameAlternative.c_str(),file.c_str());
               continue;
           }
        }
        hist->Scale(scale);
        hist->Rebin(hist->GetNbinsX()/rebin);
        if (!result)
        {
            result = new TH1F(*hist);
            result->SetDirectory(0);
        }
        else
        {
            result->Add(hist);
        }
    }
    if (!result)
    {
        throw std::string("Error while finding histogram '"+name+"' in given files");
    }
    return result;
}

TH2F* loadHistogram2D(
    const std::vector<std::string>& fileList, 
    const std::string& name, 
    const std::string& sys, 
    double scale,
    int rebinX,
    int rebinY
    )
{
    TH2F* result = nullptr;
    for (const std::string& file: fileList)
    {
        TFile input(file.c_str(),"r");
        
        std::string histName=name+"__"+sys;
        std::string histNameAlternative=name+"__nominal";
        
        
        TH2F* hist = static_cast<TH2F*>(input.Get(histName.c_str()));
        if (!hist)
        {
           printf("WARNING: using fallback '%s' for '%s'\n",histNameAlternative.c_str(),histName.c_str());
           hist = static_cast<TH2F*>(input.Get(histNameAlternative.c_str()));
           if (!hist)
           {
               printf("ERROR: fallback '%s' not found in file '%s'\n",histNameAlternative.c_str(),file.c_str());
               continue;
           }
        }
        hist->Scale(scale);
        
        hist->Rebin2D(hist->GetNbinsX()/rebinX,hist->GetNbinsY()/rebinY);
        if (!result)
        {
            result = new TH2F(*hist);
            result->SetDirectory(0);
        }
        else
        {
            result->Add(hist);
        }
    }
    if (!result)
    {
        throw std::string("Error while finding histogram '"+name+"' in given files");
    }
    return result;
}

FitResult loadFitResult(const std::string& fresult)
{
    FitResult result;
    
	//cout << "reading fit results: " << fresult << endl;
    
    std::ifstream ifs;
    ifs.open(fresult);
    if(!ifs.good()) {
        printf("ERROR: could not open fit results file! '%s'\n",fresult.c_str());
        throw std::string("ERROR: could not open fit results file! '"+fresult+"'");
    }
    std::string name;
    float scale, unc;
    printf("read fit result:\n");
    while (ifs >> name >> scale >> unc) 
    {
        if (name=="beta_signal")
        {
            name="tchan";
        }
        result[name]={{scale,unc}};
        printf(" ... %s: %6.5f +- %6.5f\n",name.c_str(),scale,unc);
    }
    printf("\n");
    ifs.close();
    return result;
}

void doUnfolding(const std::vector<std::string>& histFiles,
       const std::string& signalHistName,
       const std::set<std::string>& backgroundHistNames,
       const std::string& dataHistName,
       const std::vector<std::string>& responseFiles,
       const std::string& responseMatrixName,
       const std::string& fitResult,
       const std::string& fitCovariance,
       const std::string& outputFileName,
       const std::string& sys="nominal",
       bool noStatUnc=true,
       bool noMCStatUnc=true,
       bool noFitUnc=true,
       bool mcOnly=false)
{
	TH1::SetDefaultSumw2(true);


    FitResult fitFactors = loadFitResult(fitResult);
    
    
    TH1F* dataHist = nullptr;
    if (mcOnly)
    {
        dataHist = loadHistogram1D(histFiles,HISTPREFIX+signalHistName,"nominal",fitFactors[signalHistName][0],REBIN_RECO);
        for (const std::string& histName: backgroundHistNames)
        {
            //nice mem leak
            dataHist->Add(loadHistogram1D(histFiles,HISTPREFIX+histName,"nominal",fitFactors[histName][0],REBIN_RECO));
        }
        for (int i=0; i<dataHist->GetNbinsX()+2; ++i)
        {
            //set statistical uncertainty to Poisson
            dataHist->SetBinError(i,sqrt(dataHist->GetBinContent(i)));
        }
    }
    else
    {
        dataHist = loadHistogram1D(histFiles,HISTPREFIX+dataHistName,"nominal",1.0,REBIN_RECO);
    }
    
    printf("data hist: %i bins, N=%f\n",dataHist->GetNbinsX(),dataHist->Integral());
    
    TH2F* responseHist = loadHistogram2D(responseFiles,responseMatrixName,sys,fitFactors[signalHistName][0],REBIN_GEN,REBIN_RECO);
    printf("response hist: %ix%i, N=%f\n",responseHist->GetNbinsX(),responseHist->GetNbinsY(),responseHist->Integral());
    
    
    TUnfoldSys tunfold(responseHist,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);
    tunfold.SetBias(responseHist->ProjectionX());
    tunfold.SetInput(dataHist);
    
    //TODO: decorrelate bgs first!
    for (FitResult::const_iterator it = fitFactors.begin(); it != fitFactors.end(); ++it)
    {
        if (backgroundHistNames.find(it->first)!=backgroundHistNames.end())
        {
            TH1F* bgHist = loadHistogram1D(histFiles,HISTPREFIX+it->first,sys,1.0,REBIN_RECO);
            tunfold.SubtractBackground(bgHist,it->first.c_str(),fitFactors[it->first][0],fitFactors[it->first][1]);
        }
    }
    
    TFile outputFile(outputFileName.c_str(),"RECREATE");
    
    tunfold.DoUnfold(tau,dataHist,scaleBias);
    TH1F *unfoldedHist = new TH1F("unfolded","unfolded", REBIN_GEN, -1, 1);
	tunfold.GetOutput(unfoldedHist);
	unfoldedHist->Write();
	
	
	TH2D *hrhoij = new TH2D("correlation","correlation",REBIN_GEN,1,REBIN_GEN,REBIN_GEN,1,REBIN_GEN);
	tunfold.GetRhoIJ(hrhoij);
	hrhoij->Write();
	TH1D *hrhoi = new TH1D("1dcorr","1dcorr",bin_x,1,bin_x);
	tunfold.GetRhoI(hrhoi);
	hrhoi->Write();
	responseHist->ProjectionX("gen")->Write();

	TH2D* errorMatrix = new TH2D("error","error",REBIN_GEN,1,REBIN_GEN,REBIN_GEN,1,REBIN_GEN);
	if (!noStatUnc)
	{
	    //add error through input vector (aka statistical uncertainty)
	    tunfold.GetEmatrixInput(errorMatrix, 0, false);
	}
	if (!noMCStatUnc)
	{
	    //add error through limited MC of response matrix
	    tunfold.GetEmatrixSysUncorr(errorMatrix ,0, false);  
	    for (FitResult::const_iterator it = fitFactors.begin(); it != fitFactors.end(); ++it)
        {
            if (backgroundHistNames.find(it->first)!=backgroundHistNames.end())
            {
	            //add error through limited MC of subtracted background histograms
	            tunfold.GetEmatrixSysBackgroundUncorr(errorMatrix,it->first.c_str() ,0, false);
            }
        } 
	}
    if (!noFitUnc)
    {
        for (FitResult::const_iterator it = fitFactors.begin(); it != fitFactors.end(); ++it)
        {
            if (backgroundHistNames.find(it->first)!=backgroundHistNames.end())
            {
                //add error through fit of background normalization
	            tunfold.GetEmatrixSysBackgroundScale(errorMatrix,it->first.c_str(), 0, false); 
	        }
        }
	}
	errorMatrix->Write();
	
	outputFile.Close();
}




enum {
    OPT_SYS, 
    OPT_HISTFILE,
    OPT_FITRESULTPREFIX, 
    OPT_FITRESULT, 
    OPT_FITCOVARIANCE, 
    OPT_RESPONSEFILE,
    OPT_RESPONSEMATRIXNAME,
    OPT_OUTPUT,
    OPT_NOSTAT,
    OPT_NOMC,
    OPT_NOFITERROR,
    OPT_MCONLY,
    OPT_VERBOSE
};

CSimpleOpt::SOption options[] ={
    {OPT_SYS, "--sys", SO_REQ_SEP},
    {OPT_HISTFILE,"--histFile",SO_REQ_SEP},
    {OPT_FITRESULTPREFIX,"--fitResultPrefix",SO_REQ_SEP},
    {OPT_FITRESULT,"--fitResult",SO_REQ_SEP},
    {OPT_FITCOVARIANCE,"--fitCovariance",SO_REQ_SEP},
    {OPT_RESPONSEFILE,"--responseFile",SO_REQ_SEP},
    {OPT_RESPONSEMATRIXNAME,"--responseMatrixName",SO_REQ_SEP},
    {OPT_OUTPUT,"--output",SO_REQ_SEP},
    {OPT_NOSTAT,"--no-stat",SO_NONE},
    {OPT_NOMC,"--no-mcstat",SO_NONE},
    {OPT_NOFITERROR,"--no-fiterror",SO_NONE},
    {OPT_MCONLY,"--mc-only",SO_NONE},
    {OPT_VERBOSE,"-v",SO_NONE},
    SO_END_OF_OPTIONS

};

int main(int argc, char* argv[])
{
    CSimpleOpt parser(argc,argv,options);
    
    std::string systematic = "nominal";
    std::string fitResultPrefix="";
    std::string fitResultFile="mu.txt";
    std::string covarianceFile="mu_cov.root";
    std::vector<std::string> histFiles;
    std::vector<std::string> responseFiles;
    std::string responseMatrixName = "tm";
    std::string outputFile = "out.root";
    
    bool noStat = false;
    bool noMCStat = false;
    bool noFitError = false;
    bool mcOnly = false;
    

    
    while (parser.Next())
    {
        if (parser.LastError() == SO_SUCCESS)
        {
            if (parser.OptionId() == OPT_SYS) 
            {
                systematic=parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_HISTFILE) 
            {
                histFiles.push_back(parser.OptionArg());
            }
            else if (parser.OptionId() == OPT_FITRESULTPREFIX) 
            {
                fitResultPrefix = parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_FITRESULT) 
            {
                fitResultFile = parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_FITCOVARIANCE) 
            {
                covarianceFile = parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_RESPONSEFILE)
            {
                responseFiles.push_back(parser.OptionArg());
            }
            else if (parser.OptionId() == OPT_RESPONSEMATRIXNAME)
            {
                responseMatrixName=parser.OptionArg();
            }
            else if (parser.OptionId() == OPT_OUTPUT)
            {
                outputFile  = parser.OptionArg();
            }
            else if (parser.OptionId() ==OPT_NOSTAT)
            {
                noStat=true;
            }
            else if (parser.OptionId() ==OPT_NOMC)
            {
                noMCStat=true;
            }
            else if (parser.OptionId() ==OPT_NOFITERROR)
            {
                noFitError = true;
            }
            else if (parser.OptionId() ==OPT_MCONLY)
            {
                mcOnly=true;
            }
            else if (parser.OptionId() ==OPT_VERBOSE)
            {
                verbose=true;
            }
        }
        else
        {
            printf("Invalid argument: %s\n", parser.OptionText());
            return 1;
        }
    }
    
    if (verbose)
    {
        printf("histfiles: \n");
        for (const std::string& histFile: histFiles)
        {
            printf(" ... %s\n",histFile.c_str());
        }
        printf("\n");
        
        printf("responseFiles: \n");
        for (unsigned int i = 0; i< responseFiles.size();++i)
        {
            printf(" ... %s\n",responseFiles[i].c_str());
        }
        printf("\n");
        
        printf("matrix name: %s\n",(responseMatrixName+"__"+systematic).c_str());
        printf("\n");
        
        printf("systematic:\n ... %s\n",systematic.c_str());
        printf("\n");
        
        printf("fit:\n");
        printf(" ... scale: %s\n",(fitResultPrefix+"/"+systematic+"/"+fitResultFile).c_str());
        printf(" ... covariance: %s\n",(fitResultPrefix+"/"+systematic+"/"+covarianceFile).c_str());
        printf("\n");

    }
    
    
    
    doUnfolding(
        histFiles,
        SIGNAL,
        BACKGROUND,
        DATA,
        responseFiles,
        responseMatrixName,
        fitResultPrefix+"/"+systematic+"/"+fitResultFile,
        fitResultPrefix+"/"+systematic+"/"+covarianceFile,
        outputFile,
        systematic,
        noStat, //stat unc
        noMCStat, //mcstat unc
        noFitError, //fitunc
        mcOnly //only mc
    );
    
    
    return 0;
}
