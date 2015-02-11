#ifndef __ASYMMETRYCALC_H__
#define __ASYMMETRYCALC_H__

#include "logging.hpp"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "TH1.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TROOT.h"
#include "TMath.h"
#include "TRandom.h"
#include "TDecompChol.h"

#include <functional>
#include <vector>

TMatrixD convert(TH2* cov)
{
    const int N = cov->GetNbinsX();
    TMatrixD covMatrix(N,N);
    
    for (int i=0; i<N;++i)
    {
        for (int j=0; j<N;++j)
        {
            covMatrix[i][j]=cov->GetBinContent(i+1,j+1);
        }
    }
    return covMatrix;
}

TMatrixD decompose(const TMatrixD& cov)
{
    TDecompChol decomp(cov);
    const TMatrixD& a = decomp.GetU();
    return a;
}

std::vector<double> randGaus(const TH1* mean, const TMatrixD& a)
{
    const int N = mean->GetNbinsX();
    std::vector<double> z(N);
    for (unsigned int i=0; i<N;++i)
    {
        z[i]=gRandom->Gaus(0.0,1.0);
    }
    std::vector<double> x(N);
    for (unsigned int i=0; i<N;++i)
    {
        x[i]=mean->GetBinContent(i+1);
        for (unsigned int j=0; j<N;++j)
        {
            x[i]+=z[j]*a[i][j];
        }
    }
    return x;
    
}

struct Asymmetry
{
    double mean;
    double uncertainty;
};

double chi2A(const TH1* hist, const TMatrixD* invcov, const double *xx)
{
    const int N = hist->GetNbinsX();
    double chi2 = 0.0;
    
    for (int i = 0; i<N;++i)
    {
        double measured1=hist->GetBinContent(i+1);
        double expected1=2.0/N*0.5*(1.0+2.0*xx[0]*hist->GetBinCenter(i+1));
        //log(INFO,"cos=%3.2f, A=%3.2f m=%3.2f e=%3.2f\n",hist->GetBinCenter(i+1),xx[0],measured1,expected1);
        
        for (int j=0; j<N;++j)
        {
            double measured2=hist->GetBinContent(j+1);
            double expected2=2.0/N*0.5*(1.0+2.0*xx[0]*hist->GetBinCenter(j+1));
            double invcovariance = (*invcov)[i][j];
            
            chi2+=(expected1-measured1)*(expected2-measured2)*invcovariance;
        }
        
        //chi2+=pow(measured1-expected1,2);
    }
    return chi2;
    
}



Asymmetry estimateAsymmetry(
    const TH1* hist, const TH2* cov, 
    const char * minName = "Minuit2",
    const char *algoName = "" )
{
    
    TH1* normHist=(TH1*)hist->Clone("normHist");
    TH2* normCov=(TH2*)cov->Clone("normCov");
    
    
    
    normHist->Scale(1.0/hist->Integral());
    normCov->Scale(1.0/hist->Integral()/hist->Integral());
    
    TMatrixD covMatrix = convert(normCov);
    TMatrixD decompA = decompose(covMatrix);
    
    
    TH1D asymHist("asy","",4000,-1,1);
    for (unsigned int itoy=0; itoy<1000; ++itoy)
    {
        double sumUp=0.0;
        double sumDown=0.0;
        for (unsigned int ibin=0; ibin<hist->GetNbinsX();++ibin)
        {
            std::vector<double> x = randGaus(normHist,decompA);
            if (hist->GetBinCenter(ibin+1)<0)
            {
                sumDown+=x[ibin];
            }
            else
            {
                sumUp+=x[ibin];
            }
        }
        asymHist.Fill((sumUp-sumDown)/(sumUp+sumDown));
    }
    
    

    TMatrixD invCovMatrix = TMatrixD(TMatrixD::kInverted,covMatrix);
    
    
    
    ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer(minName, algoName);

    // set tolerance , etc...
    min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2 
    min->SetMaxIterations(10000);  // for GSL 
    min->SetTolerance(0.001);
    //min->SetPrintLevel(-1000000);
    //const double xx[1] = {0.5};
    std::function<double(const TH1*, const TMatrixD*, const double*)> unboundFct = chi2A;
    std::function<double(const double*)> boundFct = std::bind(unboundFct,normHist, &invCovMatrix, std::placeholders::_1);
    
    //boundFct(xx);
    ROOT::Math::Functor fct(boundFct,1); 


    min->SetFunction(fct);

    min->SetVariable(0,"A",0.2, 0.01);
    min->Minimize(); 

    const double *xs = min->X();
    const double *error = min->Errors();
    log(INFO,"min: %f\n",xs[0]);
    log(INFO,"err: %f\n",error[0]);
    
    
    Asymmetry res;
    //res.mean=asymHist.GetMean();
    //res.uncertainty=asymHist.GetRMS();
    res.mean=xs[0];
    res.uncertainty=error[0];
    return res;
}

#endif

