#ifndef __CLASS_H__
#define __CLASS_H__

#include "logging.hpp"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TMath.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "TUnfold.h"
#include "TUnfoldSys.h"

#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <cstdio>

struct ScanResult
{
    double taumean;
    double pmean;
    double taumax;
    double pmax;
};

TMatrixD convertHistToMatrix(const TH2D& from)
{
    TMatrixD matrix(from.GetNbinsX(),from.GetNbinsY());
    for (int row=0; row<from.GetNbinsX();++row){
        for (int col=0; col<from.GetNbinsY(); ++col) {
            (matrix)[row][col]=(from).GetBinContent(row+1,col+1);
        }
    }
    return matrix;
}

ScanResult scanTau(TH2* responseMatrix, TH1* input, bool writeCanvas=true)
{
    const int N = 2000;
    const int NBINS = responseMatrix->GetNbinsX();
    
    double* tau = new double[N];
    double* pmean = new double[N];
    double pmean_min=1.0;
    double pmean_min_tau=0.0;
    double* pmax = new double[N];
    double pmax_min=1.0;
    double pmax_min_tau=0.0;

    double** rho_avg = new double*[NBINS];
    for (int i = 0; i < NBINS; ++i)
    {
        rho_avg[i]=new double[N];
    }
    
    TH2D error("errorMatrixTauScan","",NBINS,0,NBINS,NBINS,0,NBINS);
    TUnfoldSys tunfold(responseMatrix,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);
    for (int iscan=0;iscan< N;++iscan) 
    {
        tau[iscan]=TMath::Power(10.0,1.0*(iscan/(1.0*N)*5.0-7.0));
        tunfold.DoUnfold(tau[iscan],input);
        error.Scale(0.0);
        tunfold.GetEmatrix(&error);
        
        TMatrixD cov_matrix = convertHistToMatrix(error);
        TMatrixD inv_cov_matrix=TMatrixD(TMatrixD::kInverted,cov_matrix);
        TMatrixD diag_cov_halfs(NBINS,NBINS);
        for (int i=0; i<NBINS; ++i) {
            for (int j=0; j<NBINS; ++j) {
                if (i==j)
                {
                    diag_cov_halfs[i][j]=1.0/TMath::Sqrt((cov_matrix)[i][j]);
                }
                else
                {
                    diag_cov_halfs[i][j]=0.0;
                }
            }
        }
        //correlations of the unfolded dist
        TMatrixD rho = diag_cov_halfs*(cov_matrix)*diag_cov_halfs;
        
        //calculate the average per bin correlation; will be used in the "subway" plot
        for (int offrow = 1; offrow<NBINS; ++offrow)
        {
            double sum=0.0;
            for (int entry = 0; entry < NBINS-offrow; ++entry)
            {
                sum+=rho[offrow+entry][entry];
            }
            rho_avg[offrow][iscan]=sum/(NBINS-offrow);
        }
       
        double* p = new double[NBINS];
        pmean[iscan]=0.0; //will store the average global correlation over bins
        pmax[iscan]=0.0; //will store the max global correlation over bins
        for (int i=0; i<NBINS; ++i) 
        {
            //calculate the global correlations
            p[i]=sqrt(1.0-1.0/(inv_cov_matrix[i][i]*(cov_matrix)[i][i]));
            pmean[iscan]+=p[i];
            if (p[i]>pmax[iscan]) 
            {
                pmax[iscan]=p[i];
            }
        }
        pmean[iscan]=pmean[iscan]/(1.0*NBINS);
        
        //check if this is the minimum
        if (pmean[iscan]<pmean_min) 
        {
            pmean_min=pmean[iscan];
            pmean_min_tau=tau[iscan];
        }
        //check if this is the minimum
        if (pmax[iscan]<pmax_min) 
        {
            pmax_min=pmax[iscan];
            pmax_min_tau=tau[iscan];
        }
    }
    TCanvas cv_subway("cv_subway","",800,600);
    cv_subway.SetRightMargin(0.27);
    TH2F axes("axes",";#tau;#rho",50,tau[0],tau[N-1],50,-1.1,1.1);
    axes.Draw("AXIS");
    cv_subway.SetLogx(1);
    
    double Red[5]   = {0.00, 0.00, 0.83, 0.90, 0.65};
    double Green[5] = { 0.00, 0.71, 0.90, 0.15, 0.00};
    double Blue[5]   ={  0.71, 1.00, 0.12, 0.00, 0.00};
    double Length[5] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    int start = TColor::CreateGradientColorTable(5,Length,Red,Green,Blue,(NBINS)*2);

    TLegend legend(0.74,0.9,0.99,0.2);
    legend.SetFillColor(kWhite);
    legend.SetBorderSize(0);
    legend.SetTextFont(42);
    
    for (int i=1; i<NBINS; ++i) 
    {
       
        TGraph* graph = new TGraph(N,tau,rho_avg[i]);
        graph->SetLineColor(start+(i-1)*2+1);
        graph->SetLineWidth(2);
        graph->Draw("SameL"); 
        
        char* graphName= new char[50];
        sprintf(graphName,"#LT #rho(i,i+%i) #GT",i);
        legend.AddEntry(graph,graphName,"L");
    }
    legend.AddEntry("","","");
    TGraph* graph_globalrho_avg = new TGraph(N,tau,pmean);
    graph_globalrho_avg->SetLineColor(kBlack);
    graph_globalrho_avg->SetLineStyle(2);
    graph_globalrho_avg->SetLineWidth(3);
    graph_globalrho_avg->Draw("SameL"); 
    legend.AddEntry(graph_globalrho_avg,"avg. global #rho","L");
    
    char* globalrho_mean= new char[50];
    sprintf(globalrho_mean,"#rho|min=%4.3f",pmean_min);
    legend.AddEntry("",globalrho_mean,"");
    char* globalrho_mean_tau= new char[50];
    sprintf(globalrho_mean_tau,"#tau|min=%3.2e",pmean_min_tau);
    legend.AddEntry("",globalrho_mean_tau,"");
    
    
    legend.AddEntry("","","");
    TGraph* graph_globalrho_max = new TGraph(N,tau,pmax);
    graph_globalrho_max->SetLineColor(kBlack);
    graph_globalrho_max->SetLineWidth(3);
    graph_globalrho_max->SetLineStyle(3);
    graph_globalrho_max->Draw("SameL"); 
    legend.AddEntry(graph_globalrho_max,"max. global #rho","L");
    
    char* globalrho_max= new char[50];
    sprintf(globalrho_max,"#rho|min=%4.3f",pmax_min);
    legend.AddEntry("",globalrho_max,"");
    char* globalrho_max_tau= new char[50];
    sprintf(globalrho_max_tau,"#tau|min=%3.2e",pmax_min_tau);
    legend.AddEntry("",globalrho_max_tau,"");
    
    legend.Draw("Same");
    if (writeCanvas)
    {
        cv_subway.Write();
    }
    ScanResult scanResult;
    scanResult.taumean=pmean_min_tau;
    scanResult.pmean=pmean_min;
    scanResult.taumax=pmax_min_tau;
    scanResult.pmax=pmax_min;
    return scanResult;
}


#endif

