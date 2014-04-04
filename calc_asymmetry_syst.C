#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <iostream>

#include "utils.hpp"
#include "info.h"

using namespace std;

Double_t calc_asymmetry_syst(TString syst)
{
	TFile *f1 = new TFile("histos/unfolded.root");
	TFile *f2 = new TFile("histos/unfolded_syst_"+syst+".root");

	TH1F *hunf_nom = (TH1F*)f1->Get("unfolded");
	TH1F *hunf_syst = (TH1F*)f2->Get("unfolded");
	
	Double_t asy_nom = asymmetry(hunf_nom);
	Double_t asy_syst = asymmetry(hunf_syst);

//	cout << endl << asy_nom << " " << asy_syst << endl;

	Double_t diff = asy_nom - asy_syst;
	//Double_t uncertainty = diff/asy_nom; // FIXME relative
	Double_t uncertainty = diff;
	//cout << uncertainty << endl;

	return uncertainty;
}

int main()
{	
	vector<TString> systematics;
  systematics.push_back("pu");
  systematics.push_back("jer");
  systematics.push_back("jes");
  systematics.push_back("lepton_id");
  systematics.push_back("lepton_iso");
  systematics.push_back("lepton_trigger");
  systematics.push_back("mass");
  systematics.push_back("met");
  systematics.push_back("matching"); // FIXME strange result down
  systematics.push_back("scale_tchan");
  systematics.push_back("scale_ttjets");
  systematics.push_back("scale_wjets"); // FIXME strange result down
  systematics.push_back("wjets_shape"); 
  systematics.push_back("top_weight");
  systematics.push_back("btag_bc");
  systematics.push_back("btag_l");


  Double_t uncertainty = 0;
	Double_t tot_unc_up = 0;
	Double_t tot_unc_down = 0;

  for(vector<TString>::iterator it = systematics.begin(); it != systematics.end(); it++) {
		//cout << (*it) << "\t";
		cout << (*it) << "\t & ";
		TString up = (*it);
		up += "__up";
		TString down = (*it);
		down += "__down";
		//Double_t unc_up = TMath::Power(calc_asymmetry_syst(up),2);
		Double_t unc_up = calc_asymmetry_syst(up);
		Double_t unc_down = calc_asymmetry_syst(down);
		//cout << unc_up << "\t" << unc_down << endl;
		cout << unc_up << "\t & " << unc_down << " \\\\ " << endl;
		
		if(fabs(unc_up) > fabs(unc_down))
			uncertainty += TMath::Power(unc_up, 2);
		else 
			uncertainty += TMath::Power(unc_down, 2);

		tot_unc_up += TMath::Power(unc_up, 2);
		tot_unc_down += TMath::Power(unc_down, 2);
	}
	cout << "========================" << endl;
	//cout << "relative total uncertainty: " << TMath::Sqrt(uncertainty) << endl;
	cout << "total uncertainty: " << TMath::Sqrt(uncertainty) << endl;
	cout << "total uncertainty (up): " << TMath::Sqrt(tot_unc_up) << endl;
	cout << "total uncertainty (down): " << TMath::Sqrt(tot_unc_down) << endl;

	return 0;
}
