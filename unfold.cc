#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TRandom3.h>
#include "TUnfold.h"
#include "TUnfoldSys.h"
#include <TVector.h>
#include <TMatrix.h>
#include <TMath.h>
#include <TMinuit.h>
#include <iostream>

#include "utils.hpp"
#include "info.hpp"
#include "unfold.hpp"

using namespace std;

//Unfolds the reconstructed distribution to a generated "source" distribution,
//based on the transfer matrix.
//Output is stored in a file.
//Optionally, the a systematic scenario is unfolded
void unfold(TH1F *hrec, TH1F *hgen, TH2F *hgenrec, TFile *f, TString syst)
{
	// only show errors
	// gErrorIgnoreLevel = kError;
    
	// dummy canvas
	TCanvas *c1 = new TCanvas("canvas","canvas");
	c1->Clear();
    
	TRandom3 random(0);
    
	TH1::SetDefaultSumw2(true);
    
	TFile *fo = new TFile(outfile,"RECREATE");
    
	bool subtractData = true;
	//bool subtractData = false;
    
	// Background subtraction
	vector<TString> names;
	vector<double> scales;
	vector<double> uncs;
	vector<double> preds;
	vector<TH1F*> bkghistos;
	TH1F *hsignal;
    
	vector<TH1F*> eigenhistos;
	vector<double> eigenerrors;
    
	Int_t nbkgs = 0;
	double sum_nonrot = 0;
    
    
    if(hrec == NULL) {
        cerr << "hrec==0" << endl;
    };
    
	if(subtractData) {
        
		// Order of fit results must be the same as in covariance matrix:
		// first entry beta_signal, rest alphabetic
		read_fitres(fitres,names,scales,uncs);
        
		nbkgs = names.size()-1;
        
        const TString sig_name(var_y+"__tchan__" + syst);
        hsignal = (TH1F*)f->Get(sig_name);
        if (hsignal==NULL) {
            hsignal = (TH1F*)f->Get(var_y+"__tchan");
            cout << "Could not load " << sig_name << ", using nominal " << hsignal->GetName() << " " << hsignal->Integral() << endl;
        } else {
            cout << "loaded signal histogram " << sig_name << endl;
        }
        if(hsignal == NULL) throw;
        hsignal->Rebin(NREBIN);
		hsignal->Scale(scales[0]);
        
		// Read in background histograms
		for(int i = 0; i < nbkgs ; i++) {
			TString name = names.at(i+1);
            
            const TString bkg_name(var_y + "__" + name + "__" + syst);
            TH1F *histo = (TH1F*)f->Get(bkg_name);
            
            if (histo == NULL) {
                histo = (TH1F*)f->Get(var_y + "__" + name);
                cout << "Could not load " << bkg_name << ", using nominal " << histo->GetName() << " " << histo->Integral() << endl;
            } else {
                cout << "loaded background histogram for name=" << name << " " << bkg_name << " " << histo->Integral() << endl;
            }
            
            histo->Rebin(NREBIN);
			// Scale histos
			//histo->Scale(scales[i+1]);
			preds.push_back(histo->Integral());
            
			sum_nonrot += histo->Integral();
			bkghistos.push_back((TH1F*)histo);
            
		}
		cout << "background events: " << sum_nonrot << endl;
        
		// Read in covariance matrix
		TFile *fcov = new TFile(fitres + "_cov.root");
        if (fcov->IsZombie()) {
            cerr << "ERROR: could not open " << fitres << "_cov.root" << endl;
            exit(1);
        }
		TH2F *hcov = (TH2F*)fcov->Get("covariance");
		//Decorrelate background templates
        decorrelate(hcov, bkghistos, eigenhistos, eigenerrors);
	}
    
	// Current samples are normalized to one
	double expected = 0;
    
	// Number of expected events
	if(subtractData) {
		expected = (hrec->Integral() - sum_nonrot);
    }
	else {
		expected = hrec->Integral();
    }
    
    // Calculate selection efficiency
	double overflow = hgenrec->Integral(1,bin_x,0,0);
	double total = hgenrec->Integral(1,bin_x,1,bin_y)+overflow;
	double sel_eff = hgenrec->Integral(1,bin_x,1,bin_y)/total;
    
    //cout << "overflow: " << overflow << endl;
	cout << "data events: " << hrec->Integral() << endl;
	cout << "expected signal events: " << expected << endl;
	cout << "matrix integral " << hgenrec->Integral() << endl;
	cout << "Unfolding: " + varname << endl;
    
	// Prepare unfolding
	TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);

	// set input distribution
	unfold.SetInput(hrec);
    
	// set different bias dist
    TH1F *hgen_produced = (TH1F*)hgen->Clone("hgen_produced");
	unfold.SetBias(hgen_produced);
    
    
	// subtract backgrounds
	if(subtractData) {
		for(int i = 0; i < nbkgs; i++)
		{
            unfold.SubtractBackground(bkghistos[i],names[i+1], scales[i+1], uncs[i+1]/scales[i+1]);
		}
	}
    
    
	double corr;
	corr = unfold.DoUnfold(tau, hrec, scaleBias);
    
	cout << "global correlation: " << corr << endl;
    
	fo->cd();
    
	TH1F *hurec = new TH1F("unfolded","unfolded", bin_x, var_min, var_max);
	unfold.GetOutput(hurec);
    
	cout << "selection eff: " << sel_eff << endl;
	cout << "reconstructed: " << expected << " unfolded: " << hurec->Integral() << endl;
    cout << "asymmetry: " << asymmetry(hurec) << endl;
    
	// rho, error matrix
    
    cout << "GetRhoIJ" << endl;
	TH2D *hrhoij = new TH2D("correlation","correlation",bin_x,1,bin_x,bin_x,1,bin_x);
	unfold.GetRhoIJ(hrhoij);
    
    cout << "GetRhoI" << endl;
	TH1D *hrhoi = new TH1D("1dcorr","1dcorr",bin_x,1,bin_x);
	unfold.GetRhoI(hrhoi);
    
    cout << "GetEmatrix" << endl;
	TH2D *hematrix = new TH2D("error","error",bin_x,1,bin_x,bin_x,1,bin_x);
	unfold.GetEmatrix(hematrix);
    
    cout << "GetEmatrixSysUncorr " << endl;
    if (fabs(asymmetry(hurec)) < 1.0) {
        // Add migration matrix stat. error
        unfold.GetEmatrixSysUncorr(hematrix, 0, false);  // FIXME remove for mig mag statistics
    } else {
        cout << "not getting GetEmatrixSysUncorr because A=1" << endl;
    }
	// write results
    cout << "writing output" << endl;
	hurec->Write();
	hrhoij->Write();
	hrhoi->Write();
    hgen->Write("gen");
    hrec->Write("reco");
    hgenrec->Write("genrec");
	hematrix->Write();
    
    cout << "closing file" << endl;
	fo->Close();
}

int main(int argc, char* argv[])
{
    
    cout << "using TUnfold " << TUnfold_VERSION << " argc=" << argc << endl;
    
    if (argc==5) {
        sample = argv[1];
        syst = argv[2];
        fitres = argv[3];
        outfile = argv[4];
    } else {
        cout << "./unfold SAMPLE SYST FITRES OUTFILE" << endl;
        exit(1);
    }
    //const TString syst = "met__up";
    cout << " sample=" << sample << "systematic=" << syst << " fitres=" << fitres << endl;
    
    // load histograms
    TFile *fmu = new TFile(sample+"/tmatrix_nocharge__gen_mu.root");
    TFile *fele = new TFile(sample+"/tmatrix_nocharge__gen_ele.root");
    TFile *ftau = new TFile(sample+"/tmatrix_nocharge__gen_tau.root");
    TFile *f2 = new TFile(sample+"/merged/cos_theta_lj.root");
    if (fmu->IsZombie() || fele->IsZombie() || ftau->IsZombie() || f2->IsZombie()) {
        cerr << "ERROR: could not open input files" << endl;
        exit(1);
    }
    
    const TString tm_name("tm__" + syst);
    TH2F *hgenrecmu = (TH2F*)fmu->Get(tm_name);
    TH2F *hgenrecele = (TH2F*)fele->Get(tm_name);
    TH2F *hgenrectau = (TH2F*)ftau->Get(tm_name);
    
    if (hgenrecmu == NULL || hgenrecele==NULL || hgenrectau==NULL) {
        cout << "could not load TM for systematic=" << syst << ", using nominal" << endl;
        hgenrecmu = (TH2F*)fmu->Get("tm__nominal");
        hgenrecele = (TH2F*)fele->Get("tm__nominal");
        hgenrectau = (TH2F*)ftau->Get("tm__nominal");
    } else {
        cout << "loaded TM " << tm_name << endl;
    }
    
    cout << "hgenrecele " << hgenrecele->Integral() << endl;
    cout << "hgenrecmu " << hgenrecmu->Integral() << endl;
    cout << "hgenrectau " << hgenrectau->Integral() << endl;
    
    std::cout << "mu x=" << hgenrecmu->ProjectionX()->GetEntries() << " y=" << hgenrecmu->ProjectionY()->GetEntries() << std::endl;
    std::cout << "ele x=" << hgenrecele->ProjectionX()->GetEntries() << " y=" << hgenrecele->ProjectionY()->GetEntries() << std::endl;
    std::cout << "tau x=" << hgenrectau->ProjectionX()->GetEntries() << " y=" << hgenrectau->ProjectionY()->GetEntries() << std::endl;
    TH2F *hgenrec = (TH2F*)hgenrecmu->Clone();
    hgenrec->Add(hgenrecele);
    hgenrec->Add(hgenrectau);
    TH1F *hgen = (TH1F*)hgenrec->ProjectionX();
    
    // DATA
    TH1F *hrec = (TH1F*)f2->Get(var_y+"__DATA");
    
    //Closure tests
    //TH1F *hrec = (TH1F*)hgenrec->ProjectionY();
    TH1F *hrec_tchan = (TH1F*)f2->Get(var_y+"__tchan__" + syst);
    if (hrec_tchan==NULL) {
        cout << "Could not load " << var_y << "__tchan__" << syst << ", using nominal" << endl;
        hrec_tchan = (TH1F*)f2->Get(var_y+"__tchan");
    }
    std::cout << "tchan " << hrec_tchan->GetEntries() << " " << hrec_tchan->Integral() << std::endl;
    
    cout << "Rebinning hrec " << hrec->GetNbinsX() << " by " << NREBIN << endl;
    hrec->Rebin(NREBIN);
    cout << "Rebinning hgen " << hgen->GetNbinsX() << " by " << NREBIN << endl;
    hgen->Rebin(NREBIN);
    cout << "Rebinning hgenrec " << hgenrec->GetNbinsX() << ":" << hgenrec->GetNbinsY() << " by " << NREBIN << endl;
    hgenrec->Rebin2D(NREBIN, NREBIN);
    
    unfold(hrec, hgen, hgenrec, f2, syst);
    
    cout << "writing comphep matrices" << endl;
    TFile *fo = new TFile(outfile,"UPDATE");
    TH2D *hgenrecmu_comphep = (TH2D*)fmu->Get("tm__comphep_nominal");
    if (hgenrecmu_comphep!=NULL) {
        
        //Add ele and tau matrices to muon transfer matrix
        hgenrecmu_comphep->Add((TH2F*)fele->Get("tm__comphep_nominal"));
        hgenrecmu_comphep->Add((TH2F*)ftau->Get("tm__comphep_nominal"));
        hgenrecmu_comphep->Write("tm__comphep");
        TH1D* hgenrecmu_comphep_projx = hgenrecmu_comphep->ProjectionX();
        hgenrecmu_comphep_projx->Write("tm__comphep__proj_x");
        
        TH1D* hgenrecmu_comphep_projy = hgenrecmu_comphep->ProjectionY();
        hgenrecmu_comphep_projy->Write("tm__comphep__proj_y");
    } else {
        cout << "could not load " << "tm__comphep_nominal" << endl;
    }
    fo->Close();
    
    std::cout << "all done..." << std::endl;
    return 0;
}
