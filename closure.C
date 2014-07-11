#include <iostream>
#include <TFile.h>
#include <TH1F.h>

#include "info.h"
#include "utils.hpp"

using namespace std;

void closure()
{
  // Background subtraction
  vector<TString>  names;
  vector<Float_t>  scales;
  vector<Float_t>  uncs;
  vector<Float_t>  preds;
  vector<TH1F*>  bkghistos;
  
	TFile *f = new TFile("histos/"+sample+"/mu/merged/cos_theta_lj.root");

	//TFile *f = new TFile("histos/"+sample+"/data.root");


  read_fitres("nominal",names,scales,uncs);
  Int_t nbkgs = names.size()-1;

  TH1F *hdata = (TH1F*)f->Get(var_y+"__DATA");
  TH1F *hsignal = (TH1F*)f->Get(var_y+"__tchan");
  hsignal->Scale(scales[0]);

  Float_t sum_nonrot = 0;

  // Read in background histograms
  for(int i = 0; i < nbkgs ; i++) {
    TString name = names.at(i+1);
    TH1F *histo = (TH1F*)f->Get(var_y+"__"+name);

    // Scale histos
    histo->Scale(scales[i+1]);
    preds.push_back(histo->Integral());

    hdata->Add(histo,-1);
    cout << name << " | " << histo->Integral() << endl;

    sum_nonrot += histo->Integral();
    bkghistos.push_back((TH1F*)histo);
  }

	TFile *fo = new TFile("closure.root","RECREATE");
  fo->cd();
  hdata->Write();
  hsignal->Write();
  fo->Close();

}

int main()
{
  closure();
}
