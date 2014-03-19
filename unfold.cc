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
#include "binnings.h"
#include "unfold.hpp"

using namespace std;

void unfold(TH1F *hrec, TH1F *hgen, TH2F *hgenrec, TFile *f)
{
	// only show errors
	// gErrorIgnoreLevel = kError;

	cout << "using TUnfold " << TUnfold_VERSION << endl;
	
	// dummy canvas
	TCanvas *c1 = new TCanvas("canvas","canvas");
	c1->Clear();
	
	TRandom3 random(0);

	TH1::SetDefaultSumw2(true);

	TFile *fo = new TFile("histos/unfolded.root","RECREATE");
	
	bool subtractData = true;
	//bool subtractData = false;

	// Background subtraction
	vector<TString> names;
	vector<Float_t> scales;
	vector<Float_t> uncs;
	vector<Float_t> preds;
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
		hsignal->Scale(scales[0]);

		// Read in background histograms
		for(int i = 0; i < nbkgs ; i++) {
			TString name = names.at(i+1);
			//if(name == "wzjets") name = "wzjets__wjets_FSIM_nominal"; // FIXME wzjets scale / matching
			TH1F *histo = (TH1F*)f->Get(var_y+"__"+name);
			
			// Scale histos
			histo->Scale(scales[i+1]);
			preds.push_back(histo->Integral());
			
			sum_nonrot += histo->Integral();
			bkghistos.push_back((TH1F*)histo);

		}
		cout << "background events: " << sum_nonrot << endl;

		// Read in covariance matrix
		TFile *fcov = new TFile("cov.root");
		TH2F *hcov = (TH2F*)fcov->Get("covariance");
		// Decorrelate background templates
    decorrelate(hcov, bkghistos, eigenhistos, eigenerrors);
	}

	// Current samples are normalized to one
	Float_t expected = 0;

	// Number of expected events
	if(subtractData)
		expected = (hrec->Integral() - sum_nonrot);
	else
		expected = hrec->Integral();


  // Calculate selection efficiency
	Float_t overflow = hgenrec->Integral(1,bin_x,0,0);
	Float_t total = hgenrec->Integral(1,bin_x,1,bin_y)+overflow;
	Float_t sel_eff = hgenrec->Integral(1,bin_x,1,bin_y)/total;

  //cout << "overflow: " << overflow << endl;
	cout << "data events: " << hrec->Integral() << endl;
	cout << "expected signal events: " << expected << endl;
	cout << "matrix integral " << hgenrec->Integral() << endl;
	cout << "Unfolding: " + varname << endl;
		
	// Prepare unfolding
	TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);
	//TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeNone); // FIXME For tests

	Float_t tau = 4.891e-05; // FIXME muon

	// set input distribution
	unfold.SetInput(hrec);
	
	// set different bias dist
  //TH1F *hgen_produced = (TH1F*)hgen->Clone("hgen_produced");
	//unfold.SetBias(hgen_produced);

	// subtract backgrounds
	if(subtractData) {
		for(int i = 0; i < nbkgs; i++)
		{
			unfold.SubtractBackground(eigenhistos[i],names[i+1],1.0, eigenerrors[i]);
			//unfold.SubtractBackground(bkghistos[i],names[i+1],1.0, uncs[i+1]); // FIXME Test subtracting nominal histos
		}
	}


	Float_t corr;
	corr = unfold.DoUnfold(tau,hrec, scaleBias);

	cout << "global correlation: " << corr << endl;

	fo->cd();

	TH1F *hurec = new TH1F("unfolded","unfolded",bin_x,var_min,var_max);
	unfold.GetOutput(hurec);

	cout << "selection eff: " << sel_eff << endl;
	cout << "reconstructed: " << expected << " unfolded: " << hurec->Integral() << endl;

	// rho, error matrix
	TH2D *hrhoij = new TH2D("correlation","correlation",bin_x,1,bin_x,bin_x,1,bin_x);
	unfold.GetRhoIJ(hrhoij);
	TH1D *hrhoi = new TH1D("1dcorr","1dcorr",bin_x,1,bin_x);
	unfold.GetRhoI(hrhoi);
	TH2D *hematrix = new TH2D("error","error",bin_x,1,bin_x,bin_x,1,bin_x);
	unfold.GetEmatrix(hematrix);
	// Add migration matrix stat. error
	unfold.GetEmatrixSysUncorr(hematrix, 0, false);  // FIXME remove for mig mag statistics

	// write results
	hurec->Write();
	hrhoij->Write();
	hrhoi->Write();
	hematrix->Write();
	
	fo->Close();
}

int main()
{	
	// load histograms
  TFile *f = new TFile("histos/"+sample+"/mu/tmatrix_nocharge.root");
	TFile *f2 = new TFile("histos/"+sample+"/mu/merged/cos_theta_lj.root");
  
  // ele histograms
  //TFile *f = new TFile("histos/"+sample+"/mu/tmatrix_nocharge.root");
	//TFile *f2 = new TFile("histos/"+sample+"/ele/merged/cos_theta_lj.root");
	
	TH2F *hgenrec = (TH2F*)f->Get("tm__pdgid_13__nominal");
	TH1F *hgen = (TH1F*)hgenrec->ProjectionX();

	//TH2F *hgenrec = (TH2F*)f->Get("tm__pdgid_11__nominal");
	//TH1F *hgen = (TH1F*)f->Get("tm__pdgid_11__nominal__proj_x");

	// DATA
	TH1F *hrec = (TH1F*)f2->Get(var_y+"__DATA");

	// reconstructed, subtracted, matrix, efficiency, bias
	unfold(hrec,hgen,hgenrec,f2);
}
