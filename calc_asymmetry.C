#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <iostream>

#include "utils.hpp"
#include "info.h"

using namespace std;


void calc_asymmetry()
{
	TFile *f1 = new TFile("histos/unfolded.root");
	TFile *f2 = new TFile("histos/pseudoexperiments.root");
	TFile *f3 = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__reco_mu.root");
	TFile *f5 = new TFile("histos/"+sample+"/mu/merged/cos_theta_lj.root");

	TH1F *hunf = (TH1F*)f1->Get("unfolded");
	TH2F *herr = (TH2F*)f1->Get("error");
	TH1F *hgen_presel = (TH1F*)f3->Get("tm__nominal__proj_x");
	TH1F *hrec = (TH1F*)f3->Get("tm__nominal__proj_y");
	
	TH1F *hasy= (TH1F*)f2->Get("asymmetry");
	TH1F *hStatErr = (TH1F*)f2->Get("staterr");
	
	TH1F *hdata = (TH1F*)f5->Get(var_y+"__DATA");
	
	cout << "generated (before selection)" << endl;
	cout << asymmetry(hgen_presel) << endl;
	cout << "reconstruced" << endl;
	cout << asymmetry(hrec) << endl;
	cout << "unfolded" << endl;
	cout << asymmetry(hunf) << endl;
	cout << "stat. error: " << error_unfold(herr,hunf) << endl;
	cout << "unfolded (mean PEs)" << endl;
	cout << hasy->GetMean() << endl;
	cout << "mean PEs stat. error: " << hStatErr->GetMean() << " +- " << hStatErr->GetMeanError() << endl;
//	cout << "DATA" << endl;
//	cout << asymmetry(hdata) << endl;
}

int main()
{
	calc_asymmetry();

	return 0;
}
