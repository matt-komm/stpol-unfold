#ifndef __LOAD_HISTOGRAM_H__
#define __LOAD_HISTOGRAM_H__

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "logging.hpp"

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <array>
#include <string>
#include <fstream>

TH1F* loadHistogram1D(
    const std::vector<std::string>& fileList, 
    const std::string& histName, 
    const std::string& sys, 
    double scale,
    int rebin
    )
{
    TH1F* result = nullptr;
    for (const std::string& file: fileList)
    {
        TFile input(file.c_str(),"r");
        
        TH1F* hist=nullptr;
        if ((sys=="" or sys=="nominal") and input.FindKey(histName.c_str()))
        {
            hist = static_cast<TH1F*>(input.Get(histName.c_str()));
        }
        else if (input.FindKey((histName+"__"+sys).c_str()))
        {
            hist = static_cast<TH1F*>(input.Get((histName+"__"+sys).c_str()));
        }
        else if (input.FindKey(histName.c_str()))
        {
            log(WARNING,"using fallback '%s' for '%s'\n",histName.c_str(),(histName+"__"+sys).c_str());
            hist = static_cast<TH1F*>(input.Get(histName.c_str()));
        }
        else
        {
            log(ERROR,"neither '%s' nor '%s' found in file '%s'\n",histName.c_str(),(histName+"__"+sys).c_str(),file.c_str());
            continue;
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
        log(CRITICAL,"Error while finding histogram '"+histName+"' with sys '"+sys+"' in given files");
        throw std::string("Error while finding histogram '"+histName+"' with sys '"+sys+"' in given files");
    }
    return result;
}

TH2F* loadHistogram2D(
    const std::vector<std::string>& fileList, 
    const std::string& histName, 
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
        
        TH2F* hist=nullptr;
        if ((sys=="") and input.FindKey(histName.c_str()))
        {
            hist = static_cast<TH2F*>(input.Get(histName.c_str()));
        }
        else if (input.FindKey((histName+"__"+sys).c_str()))
        {
            hist = static_cast<TH2F*>(input.Get((histName+"__"+sys).c_str()));
        }
        else if (input.FindKey((histName+"__nominal").c_str()))
        {
            log(WARNING,"using fallback '%s' for '%s'\n",(histName+"__nominal").c_str(),(histName+"__"+sys).c_str());
            hist = static_cast<TH2F*>(input.Get((histName+"__nominal").c_str()));
        }
        else
        {
            log(ERROR,"neither '%s', '%s' nor '%s' found in file '%s'\n",histName.c_str(),(histName+"__nominal").c_str(),(histName+"__"+sys).c_str(),file.c_str());
            continue;
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
        log(CRITICAL,"Error while finding histogram '"+histName+"' with sys '"+sys+"' in given files");
        throw std::string("Error while finding histogram '"+histName+"' with sys '"+sys+"' in given files");
    }
    return result;
}

#endif

