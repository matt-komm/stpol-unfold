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

void unfold_syst(TString syst, TH1F *hrec, TH2F *hgenrec, TFile *f)
{
	// only show errors
	gErrorIgnoreLevel = kError;

	cout << "using TUnfold " << TUnfold_VERSION << endl;
	
	// dummy canvas
	TCanvas *c1 = new TCanvas("canvas","canvas");
	c1->Clear();
	
	TRandom3 random(0);

	TH1::SetDefaultSumw2(true);

	TFile *fo = new TFile("histos/unfolded_syst_"+syst+".root","RECREATE");
	
	bool subtractData = true;
	//bool subtractData = false;

	// Background subtraction
	vector<TString> names;
	vector<Float_t> scales;
	vector<Float_t> uncs;
	vector<Float_t> preds;
	vector<TH1F*> bkghistos;

	vector<TH1F*> eigenhistos;
	vector<Float_t> eigenerrors;

	Int_t nbkgs = 0;
	Float_t sum_nonrot = 0;


  if(hrec == NULL) throw;

	if(subtractData) {
		
		// Order of fit results must be the same as in covariance matrix:
		// first entry beta_signal, rest alphabetic
		//read_fitres("nominal",names,scales,uncs);
		read_fitres("syst_"+syst,names,scales,uncs);
				
		nbkgs = names.size()-1;

		// Read in background histograms
		for(int i = 0; i < nbkgs ; i++) {
      // First entry must be beta_signal
			TString name = names.at(i+1);
      TH1F *histo = NULL;
			histo = (TH1F*)f->Get(var_y+"__"+name+"__"+syst);
      if(histo == NULL) {
        //cout << "syst not available: " << name << endl;
        histo = (TH1F*)f->Get(var_y+"__"+name);
      } else {
        // FIXME normalize to nominal
        TH1F *nominal = (TH1F*)f->Get(var_y+"__"+name);
        histo->Scale(nominal->Integral()/histo->Integral());
      }
			
			// Scale histos
			histo->Scale(scales[i+1]);
			preds.push_back(histo->Integral());

      //cout << histo->Integral() << endl;
			
			sum_nonrot += histo->Integral();
			bkghistos.push_back((TH1F*)histo);

		}
		cout << "background events: " << sum_nonrot << endl;

		// Read in covariance matrix
		TFile *fcov = new TFile("fitresults/cov.root"); // FIXME systematics
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
	//Float_t overflow = hgenrec->Integral(1,bin_x,0,0);
	//Float_t total = hgenrec->Integral(1,bin_x,1,bin_y)+overflow;
	//Float_t sel_eff = hgenrec->Integral(1,bin_x,1,bin_y)/total;

  //cout << "overflow: " << overflow << endl;
	//cout << "data events: " << hrec->Integral() << endl;
	cout << "expected signal events: " << expected << endl;
	//cout << "matrix integral " << hgenrec->Integral() << endl;
	cout << "Unfolding: " + varname << endl;
	
	// Prepare unfolding
	TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);
	//TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeNone); // FIXME For tests

  // FIXME automate this
  Float_t tau = 2.41005e-05; // mu 0.8

	// set input distribution
	unfold.SetInput(hrec);
	

	// subtract backgrounds
	if(subtractData) {
		for(int i = 0; i < nbkgs; i++)
		{
			//unfold.SubtractBackground(eigenhistos[i],names[i+1],1.0, eigenerrors[i]);
			unfold.SubtractBackground(bkghistos[i],names[i+1],1.0, uncs[i+1]); // FIXME Test subtracting nominal histos
		}
	}


	Float_t corr;
	corr = unfold.DoUnfold(tau,hrec, scaleBias);

	cout << "global correlation: " << corr << endl;

	fo->cd();

	TH1F *hurec = new TH1F("unfolded","unfolded",bin_x,var_min,var_max);
	unfold.GetOutput(hurec);

	//cout << "selection eff: " << sel_eff << endl;
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

  cout << "asymmetry: " << asymmetry(hurec) << endl;

	// write results
	hurec->Write();
	hrhoij->Write();
	hrhoi->Write();
	hematrix->Write();
	
	fo->Close();
  delete c1;
}

int main()
{	
	// load histograms
  TFile *fmu = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_mu.root");
  TFile *fele = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_ele.root");
  TFile *ftau = new TFile("histos/"+sample+"/mu/tmatrix_nocharge__gen_tau.root");

  vector<TString> systematics;
  systematics.push_back("pu__up");
  systematics.push_back("pu__down");
  systematics.push_back("jer__up");
  systematics.push_back("jer__down");
  systematics.push_back("jes__up");
  systematics.push_back("jes__down");
  systematics.push_back("lepton_id__up");
  systematics.push_back("lepton_id__down");
  systematics.push_back("lepton_iso__up");
  systematics.push_back("lepton_trigger__up");
  systematics.push_back("lepton_trigger__down");
  systematics.push_back("mass__up");
  systematics.push_back("mass__down");
  systematics.push_back("met__up");
  systematics.push_back("met__down");
  systematics.push_back("matching__up");
  systematics.push_back("matching__down");
  systematics.push_back("met__up");
  systematics.push_back("met__down");
  systematics.push_back("scale_tchan__up");
  systematics.push_back("scale_tchan__down");
  systematics.push_back("scale_ttjets__up");
  systematics.push_back("scale_ttjets__down");
  systematics.push_back("scale_wjets__up");
  systematics.push_back("scale_wjets__down");
  systematics.push_back("wjets_shape__up");
  systematics.push_back("wjets_shape__down");
  systematics.push_back("top_weight__up");
  systematics.push_back("top_weight__down");
  systematics.push_back("btag_bc__up");
  systematics.push_back("btag_bc__down");
  systematics.push_back("btag_l__up");
  systematics.push_back("btag_l__down");


  for(vector<TString>::iterator it = systematics.begin(); it != systematics.end(); it++) {
    // systematic reconstructed, matrix
    TString syst = (*it);
    cout << syst << endl;
    TFile *f2 = new TFile("histos/"+sample+"/mu/merged/cos_theta_lj.root");
  
    //if(syst.Contains("fraction") or syst.Contains("FSIM") or syst.Contains("ttbar_scale") or syst.Contains("ttbar_matching") or syst.Contains("wjets_shape") or syst.Contains("wjets_flat") or syst.Contains("iso")) syst = "nominal";
    TH2F *hgenrec = (TH2F*)fmu->Get("tm__nominal"); // FIXME change to systematic
    TH2F *hgenrecele = (TH2F*)fele->Get("tm__nominal");
    TH2F *hgenrectau = (TH2F*)ftau->Get("tm__nominal");
    hgenrec->Add(hgenrecele);
    hgenrec->Add(hgenrectau);


    TH1F *hrec = (TH1F*)f2->Get(var_y+"__DATA");

    unfold_syst(syst,hrec,hgenrec,f2);
    f2->Close();
  }

  return 0;
}
