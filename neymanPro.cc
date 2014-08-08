#include "SimpleOpt.h"

#include "loadHistogram.hpp"
#include "logging.hpp"

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

enum {
    OPT_RESPONSEFILE,
    OPT_RESPONSEMATRIXNAME,
    OPT_COMPHEP1000,
    OPT_COMPHEP1100,
    OPT_COMPHEP0100,
    OPT_OUTPUT,
    
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
    
    

}
