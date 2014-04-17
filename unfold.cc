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
			TH1F *histo = (TH1F*)f->Get(var_y+"__"+name);
			
			// Scale histos
			histo->Scale(scales[i+1]);
			preds.push_back(histo->Integral());
			
			sum_nonrot += histo->Integral();
			bkghistos.push_back((TH1F*)histo);

		}
		cout << "background events: " << sum_nonrot << endl;

		// Read in covariance matrix
		TFile *fcov = new TFile("fitresults/cov.root");
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
	//TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeSize); // FIXME For tests
	//TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeDerivative); // FIXME For tests

  //Float_t tau = 3.69986e-05; // mu 0.4
  //Float_t tau = 2.41005e-05; // mu 0.8
  //Float_t tau = 3.25254e-05; // mu 0.6
  //Float_t tau = 2.65416e-05; // ele 0.6

  //Float_t tau = 2.43043e-05; // mu cut based
  Float_t tau = 2.81052e-05; // new mu 0.6

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
  cout << "asymmetry: " << asymmetry(hurec) << endl;

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
  // mu histograms
  TFile *fmu = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_mu.root");
  TFile *fele = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_ele.root");
  TFile *ftau = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_tau.root");
	TFile *f2 = new TFile("histos/"+sample+"/mu/merged/cos_theta_lj.root");
  
  // ele histograms
  /*
  TFile *fmu = new TFile("histos/"+sample+"/ele/tmatrix_nocharge__gen_mu.root");
  TFile *fele = new TFile("histos/"+sample+"/ele/tmatrix_nocharge__gen_ele.root");
  TFile *ftau = new TFile("histos/"+sample+"/ele/tmatrix_nocharge__gen_tau.root");
	TFile *f2 = new TFile("histos/"+sample+"/ele/merged/cos_theta_lj.root");
  */
 
	TH2F *hgenrecmu = (TH2F*)fmu->Get("tm__nominal");
	TH2F *hgenrecele = (TH2F*)fele->Get("tm__nominal");
	TH2F *hgenrectau = (TH2F*)ftau->Get("tm__nominal");
	
  TH2F *hgenrec = (TH2F*)hgenrecmu->Clone();
  hgenrec->Add(hgenrecele);
  hgenrec->Add(hgenrectau);

/*
  // FIXME Check with Comphep TMs

  //TH2F *hgenrecmu = (TH2F*)fmu->Get("tm__comphep_nominal");
	//TH2F *hgenrecele = (TH2F*)fele->Get("tm__comphep_nominal");
	//TH2F *hgenrectau = (TH2F*)ftau->Get("tm__comphep_nominal");
  
  TH2F *hgenrecmu_unphys = (TH2F*)fmu->Get("tm__comphep_anom_unphys");
  TH2F *hgenrecele_unphys = (TH2F*)fele->Get("tm__comphep_anom_unphys");
  TH2F *hgenrectau_unphys = (TH2F*)ftau->Get("tm__comphep_anom_unphys");

  TH2F *hgenrec_unphys = (TH2F*)hgenrecmu_unphys->Clone();
  hgenrec_unphys->Add(hgenrecele_unphys);
  hgenrec_unphys->Add(hgenrectau_unphys);
  
  TH2F *hgenrecmu_bsm = (TH2F*)fmu->Get("tm__comphep_anom_0100");
  TH2F *hgenrecele_bsm = (TH2F*)fele->Get("tm__comphep_anom_0100");
  TH2F *hgenrectau_bsm = (TH2F*)ftau->Get("tm__comphep_anom_0100");
  
  TH2F *hgenrec_bsm = (TH2F*)hgenrecmu_bsm->Clone();
  hgenrec_bsm->Add(hgenrecele_bsm);
  hgenrec_bsm->Add(hgenrectau_bsm);

  Double_t V_L = 1.0;
  Double_t V_R = 0.3;

  Double_t wtot = (pow(V_L,2) + pow(V_R,2));
  Double_t m = pow(V_L,4)/wtot;
  Double_t n = pow(V_L,2)*pow(V_R,2)/wtot;
  Double_t k = pow(V_R,4)/wtot;

  hgenrec->Scale(m);
  hgenrec_unphys->Scale(n);
  hgenrec_bsm->Scale(k);
  
  hgenrec->Add(hgenrec_unphys);
  hgenrec->Add(hgenrec_bsm);
*/


	TH1F *hgen = (TH1F*)hgenrec->ProjectionX();

	// DATA
	TH1F *hrec = (TH1F*)f2->Get(var_y+"__DATA");
	
	// reconstructed, generated, matrix, histo file
  unfold(hrec,hgen,hgenrec,f2);
}
