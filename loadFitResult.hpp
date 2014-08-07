#ifndef __LOADFITRESULT_H__
#define __LOADFITRESULT_H__

#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <array>
#include <string>
#include <fstream>

typedef std::map<std::string,std::array<float,2>> FitResult;
FitResult loadFitResult(const std::string& fresult)
{
    FitResult result;
    
    std::ifstream ifs;
    ifs.open(fresult);
    if(!ifs.good()) {
        log(CRITICAL, "", "ERROR: could not open fit results file! '%s'\n",fresult.c_str());
        throw std::string("ERROR: could not open fit results file! '"+fresult+"'");
    }
    std::string name;
    float scale, unc;
    log(INFO,"read fit result: %s\n",fresult.c_str());
    while (ifs >> name >> scale >> unc) 
    {
        if (name=="beta_signal")
        {
            name="tchan";
        }
        result[name]={{scale,unc}};
        log(DEBUG," ... %s: %6.5f +- %6.5f\n",name.c_str(),scale,unc);
    }
    ifs.close();
    return result;
}

#endif

