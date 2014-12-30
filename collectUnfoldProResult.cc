#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"

#include "SimpleOpt.h"
#include "logging.hpp"
#include "asymmetryCalculation.hpp"

#include <string>
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

Asymmetry loadFromFile(std::string file)
{
    TFile f(file.c_str());
    TH1D* hist = (TH1D*)f.Get("unfolded");
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

    
    
    Asymmetry nominalA = loadFromFile(nominalFile);
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
        Asymmetry upA = loadFromFile(upFile);
        Asymmetry downA = loadFromFile(downFile);
        up=upA.mean;
        down=downA.mean;
    }
    std::ofstream ouputFile(output.c_str());
    ouputFile<<"\"syst\",\"up\",\"down\",\"dup\",\"ddown\",\"d\""<<std::endl;
    ouputFile<<("\""+sysName+"\",").c_str()<<mean<<","<<up<<","<<down<<","<<fabs(mean-up)
             <<","<<fabs(mean-down)<<","<<std::max(fabs(mean-down),fabs(mean-up))<<std::endl;
    ouputFile.close();
    //std::cout<<nominalA.mean<<" +- "<<nominalA.uncertainty<<std::endl;
    
    
    return 0;
}
