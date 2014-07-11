#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TUnfold.h"
#include "TUnfoldSys.h"
#include <TVector.h>
#include <TMatrix.h>
#include <TMath.h>
#include "TMinuit.h"
#include <iostream>

#include "utils.hpp"
#include "info.h"
#include "unfold.hpp"

using namespace std;

// Number of pseudo experiments
//#define NPSEUDO 50000
#define NPSEUDO 1000


void unfold(TH1F *hrec, TH1F *hgen, TH2F *hgenrec, TFile *f)
{
	// only show errors
	gErrorIgnoreLevel = kError;

	cout << "using TUnfold " << TUnfold_VERSION << endl;
	
	// dummy canvas
	TCanvas *c1 = new TCanvas("canvas","canvas");
	c1->Clear();
	
	TRandom3 random(0);

	TH1::SetDefaultSumw2(true);

	TFile *fo = new TFile("histos/pseudoexperiments.root","RECREATE");
	
	//bool subtractData = true;
	bool subtractData = false;

  Double_t sum_tau = 0;
  Double_t sum_corr = 0;

	// Background subtraction
	vector<TString> names;
	vector<Float_t> scales;
	vector<Float_t> uncs;
	vector<TH1F*> bkghistos;
	TH1F *hsignal;

	vector<TH1F*> eigenhistos;
	vector<Float_t> eigenerrors;

	Int_t nbkgs = 0;
	Float_t sum_nonrot = 0;


  if(hrec == NULL) throw;

	if(subtractData) {
		// Order of fit results must be the same as in covariance matrix:
		// first entry beta_signal, rest alphabetic
		read_fitres("nominal",names,scales,uncs);
				
		nbkgs = names.size()-1;

		hsignal = (TH1F*)f->Get(var_y+"__tchan");
    if(hsignal == NULL) throw;
    hsignal->Rebin(4);
		hsignal->Scale(scales[0]);

		// Read in background histograms
		for(int i = 0; i < nbkgs ; i++) {
			TString name = names.at(i+1);
			TH1F *histo = (TH1F*)f->Get(var_y+"__"+name);
			
      histo->Rebin(4);
			// Scale histos
			//histo->Scale(scales[i+1]);
			
			sum_nonrot += histo->Integral();
			bkghistos.push_back((TH1F*)histo);

		}
		cout << "background events: " << sum_nonrot << endl;

  // FIXME currently disabled
/*
		// Read in covariance matrix
		TFile *fcov = new TFile("cov.root");
		TH2F *hcov = (TH2F*)fcov->Get("covariance");
		// Decorrelate background templates
    decorrelate(hcov, bkghistos, eigenhistos, eigenerrors);
*/
	}

	cout << "Unfolding: " + varname << endl;

	// Prepare unfolding
	TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);

	// subtract backgrounds
	if(subtractData) {
		for(int i = 0; i < nbkgs; i++)
		{
			//unfold.SubtractBackground(eigenhistos[i],names[i+1],1.0, eigenerrors[i]);
			unfold.SubtractBackground(bkghistos[i],names[i+1], scales[i+1], uncs[i+1]/scales[i+1]); // FIXME Test subtracting nominal histos
		}
	}


	fo->cd();
	// pseudo experiment histos
	TH1F *hPull[bin_x];
	TH1F *hBin[bin_x];
	TH1F hStatErr("staterr","staterr",1000,0.0,1.0);
	TH1F hasy("asymmetry","asymmetry",100,0.0,1.0);
	TH1F hasy_bias("asymmetry_bias","asymmetry_bias",100,-1.0,1.0);
	TH1F hasy_pull("asymmetry_pull","asymmetry_pull",100,-3.0,3.0);

	TString pull_name = "pull_";
	TString bin_name = "reldiff_";
	for(Int_t i=1; i <= bin_x; i++) {
		TString pname = pull_name;
		pname += i;
		TString bname = bin_name;
		bname += i;
		hPull[i-1] = new TH1F(pname,pname,60,-3.0,3.0);
		hBin[i-1] = new TH1F(bname,bname,100,-1.0,1.0);
	}
	
	// do PEs
  cout << "Dicing " << NPSEUDO << " pseudo events..." << endl;
  TH1F *hgen_produced = (TH1F*)hgen->Clone("hgen_produced");
  Float_t genasy = asymmetry(hgen_produced);
  cout << "Generated asymmetry: " << genasy << endl;
  TH1F *hpseudo = new TH1F("pseudo","pseudo", bin_y, var_min, var_max);
  for(Int_t p=1; p<=NPSEUDO; p++) {

    if(p%500 == 0) cout << p << endl;

    hpseudo->Reset();
    if(subtractData) {
      // Dice from backgrounds
      for(int i = 0; i < nbkgs ; i++) {
        //TH1F *heigen = (TH1F*)eigenhistos[i];
        TH1F *heigen = (TH1F*)bkghistos[i]; // FIXME Test dicing nominal
        TH1F *hclone = (TH1F*)heigen->Clone();

        //Float_t bla = random.Gaus(heigen->Integral(),eigenerrors[i]*heigen->Integral());
        Float_t bla = random.Gaus(heigen->Integral(),uncs[i+1]/scales[i+1]*heigen->Integral()); // FIXME test dicing nominal

        int n = random.Poisson(bla);

        for(int ibin = 1; ibin <= bin_y; ibin++) {
          Float_t val = hclone->GetBinContent(ibin);
          Float_t err = hclone->GetBinError(ibin);
          hclone->SetBinContent(ibin, random.Gaus(val, err));
        }

        for(int j = 0; j < n; j++) {
          hpseudo->Fill(hclone->GetRandom());
        }
        delete hclone;
      }
      // Dice from signal
      int n = random.Poisson(hsignal->Integral());
      TH1F *hclone = (TH1F*)hsignal->Clone();
      for(int ibin = 1; ibin <= bin_y; ibin++) {
        Float_t val = hclone->GetBinContent(ibin);
        Float_t err = hclone->GetBinError(ibin);
        hclone->SetBinContent(ibin, random.Gaus(val, err));
      }
      for(int j = 0; j < n; j++) {
        hpseudo->Fill(hclone->GetRandom());
      }
      delete hclone;
    } else { // signal only
      int n = random.Poisson(hrec->Integral());
      TH1F *hclone = (TH1F*)hrec->Clone();
      // Additional smearing
      for(int ibin = 1; ibin <= bin_y; ibin++) {
      Float_t val = hclone->GetBinContent(ibin);
      Float_t err = hclone->GetBinError(ibin);
      hclone->SetBinContent(ibin, random.Gaus(val, err));
      }

      for(int j = 0; j < n; j++) {
        hpseudo->Fill(hclone->GetRandom());
      }
      delete hclone;
    }

    unfold.SetInput(hpseudo);

    minimizeRhoAverage(&unfold, hpseudo, 1000, -6, -1); // mu
    //minimizeRhoAverage(&unfold, hrec, 1000, -6, 0); // ele
    Float_t tau = unfold.GetTau();
    sum_tau += tau;

    Double_t corr = unfold.DoUnfold(tau,hpseudo,scaleBias);
    sum_corr += corr;
    
    TH1F *hupseudo = new TH1F("upseudo","pseudo unfolded",bin_x,var_min,var_max);
    unfold.GetOutput(hupseudo);

    // Ematrix not containing all errors,
    // check http://root.cern.ch/root/html/TUnfoldSys.html
    TH2F *hperr = new TH2F("perror","perror",bin_x,1,bin_x,bin_x,1,bin_x);
    unfold.GetEmatrix(hperr);
    // Add migration matrix stat. error
    unfold.GetEmatrixSysUncorr(hperr, 0, false); 

    // Calculate asymmetry
    Float_t asy = asymmetry(hupseudo);
    hasy.Fill(asy);
    Float_t asy_diff = genasy - asy;
    hasy_bias.Fill(asy_diff);
    Float_t perror = error_unfold(hperr,hupseudo);
    hStatErr.Fill(perror);
    hasy_pull.Fill(asy_diff/perror);
    // pull, rel. diff.
    for(Int_t k=1; k<=bin_x; k++) {
      Float_t diff = (hgen_produced->GetBinContent(k) - hupseudo->GetBinContent(k));

      hPull[k-1]->Fill(diff/hupseudo->GetBinError(k));
      hBin[k-1]->Fill(diff/hgen_produced->GetBinContent(k));
    }
    delete hperr;
    delete hupseudo;
  }
  // end pseudo

	// write pseudo exp results
  hasy.Write();
  hasy_bias.Write();
  hasy_pull.Write();
  hStatErr.Write();

  // write pull, bin histos
  for(Int_t i=0; i < bin_x; i++) {
    hBin[i]->Write();
    hPull[i]->Write();
  }

	fo->Close();

  cout << "mean correlation: " << sum_corr/NPSEUDO << " mean tau: " << sum_tau/NPSEUDO << endl;
}

int main()
{	
	// load histograms
  /*
  // mu histograms
  TFile *fmu = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_mu.root");
  TFile *fele = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_ele.root");
  TFile *ftau = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_tau.root");
	TFile *f2 = new TFile("histos/"+sample+"/mu/merged/cos_theta_lj.root");
  */

  // ele histograms
  TFile *fmu = new TFile("histos/"+sample+"/ele/tmatrix_nocharge__gen_mu.root");
  TFile *fele = new TFile("histos/"+sample+"/ele/tmatrix_nocharge__gen_ele.root");
  TFile *ftau = new TFile("histos/"+sample+"/ele/tmatrix_nocharge__gen_tau.root");
	TFile *f2 = new TFile("histos/"+sample+"/ele/merged/cos_theta_lj.root");
 
	TH2F *hgenrecmu = (TH2F*)fmu->Get("tm__nominal");
	TH2F *hgenrecele = (TH2F*)fele->Get("tm__nominal");
	TH2F *hgenrectau = (TH2F*)ftau->Get("tm__nominal");

  hgenrecmu->Rebin2D(4,4);
  hgenrecele->Rebin2D(4,4);
  hgenrectau->Rebin2D(4,4);

  TH2F *hgenrec = (TH2F*)hgenrecmu->Clone();
  hgenrec->Add(hgenrecele);
  hgenrec->Add(hgenrectau);
	TH1F *hgen = (TH1F*)hgenrec->ProjectionX();

	TH1F *hrec = (TH1F*)f2->Get(var_y+"__tchan"); //FIXME
  hrec->Rebin(4);
	//TH1F *hrec = (TH1F*)hgenrec->ProjectionY();

	// reconstructed, subtracted, matrix, efficiency, bias
	unfold(hrec,hgen,hgenrec,f2);
}
