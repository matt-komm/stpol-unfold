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
#include "info.hpp"
#include "unfold.hpp"

using namespace std;

const int NPSEUDO = 10000;

// float unfold_pseudo(TH1F *hrec, TH1F *hgen, TH2F *hgenrec, TFile *f)
// {
//     // only show errors
//     gErrorIgnoreLevel = kError;
//
//     // dummy canvas
//     TCanvas *c1 = new TCanvas("canvas","canvas");
//     c1->Clear();
//
//     TRandom3 random(0);
//
//     TH1::SetDefaultSumw2(true);
//
//     TFile *fo = new TFile("histos/pseudoexperiments.root","RECREATE");
//
//     bool subtractData = true;
//     //bool subtractData = false;
//
//     Double_t sum_tau = 0;
//     Double_t sum_corr = 0;
//
//     // Background subtraction
//     vector<TString> names;
//     vector<Float_t> scales;
//     vector<Float_t> uncs;
//     vector<TH1F*> bkghistos;
//     TH1F *hsignal;
//
//     vector<TH1F*> eigenhistos;
//     vector<Float_t> eigenerrors;
//
//     Int_t nbkgs = 0;
//     Float_t sum_nonrot = 0;
//
//
//     if(hrec == NULL) throw;
//
//     if(subtractData)
//     {
//         // Order of fit results must be the same as in covariance matrix:
//         // first entry beta_signal, rest alphabetic
//         read_fitres("nominal",names,scales,uncs);
//
//         nbkgs = names.size()-1;
//
//         hsignal = (TH1F*)f->Get(var_y+"__tchan");
//         if(hsignal == NULL) throw;
//         hsignal->Scale(scales[0]);
//
//         // Read in background histograms
//         for(int i = 0; i < nbkgs ; i++)
//         {
//             TString name = names.at(i+1);
//             const TString bkg_name(var_y+"__"+name);
//             TH1F *histo = (TH1F*)f->Get(bkg_name);
//
//             // Scale histos
//             histo->Scale(scales[i+1]);
//
//             sum_nonrot += histo->Integral();
//             bkghistos.push_back((TH1F*)histo);
//
//         }
//         cout << "background events: " << sum_nonrot << endl;
//
//         // FIXME currently disabled
//         /*
//                 // Read in covariance matrix
//                 TFile *fcov = new TFile("cov.root");
//                 TH2F *hcov = (TH2F*)fcov->Get("covariance");
//                 // Decorrelate background templates
//             decorrelate(hcov, bkghistos, eigenhistos, eigenerrors);
//         */
//     }
//
//     cout << "Unfolding: " + varname << endl;
//
//     // Prepare unfolding
//     TUnfoldSys unfold(hgenrec,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature);
//
//     // subtract backgrounds
//     if(subtractData)
//     {
//         for(int i = 0; i < nbkgs; i++)
//         {
//             //unfold.SubtractBackground(eigenhistos[i],names[i+1],1.0, eigenerrors[i]);
//             unfold.SubtractBackground(bkghistos[i],names[i+1],1.0, uncs[i+1]); // FIXME Test subtracting nominal histos
//         }
//     }
//
//
//     fo->cd();
//     // pseudo experiment histos
//     TH1F *hPull[bin_x];
//     TH1F *hBin[bin_x];
//     TH1F hStatErr("staterr","staterr",1000,0.0,1.0);
//     TH1F hasy("asymmetry","asymmetry",100,0.0,1.0);
//     TH1F hasy_bias("asymmetry_bias","asymmetry_bias",100,-1.0,1.0);
//     TH1F hasy_pull("asymmetry_pull","asymmetry_pull",100,-3.0,3.0);
//
//     TString pull_name = "pull_";
//     TString bin_name = "reldiff_";
//     for(Int_t i=1; i <= bin_x; i++)
//     {
//         TString pname = pull_name;
//         pname += i;
//         TString bname = bin_name;
//         bname += i;
//         hPull[i-1] = new TH1F(pname,pname,60,-3.0,3.0);
//         hBin[i-1] = new TH1F(bname,bname,100,-1.0,1.0);
//     }
//
//     // do PEs
//     cout << "Dicing " << NPSEUDO << " pseudo events..." << endl;
//     TH1F *hgen_produced = (TH1F*)hgen->Clone("hgen_produced");
//     Float_t genasy = asymmetry(hgen_produced);
//     cout << "Generated asymmetry: " << genasy << endl;
//     TH1F *hpseudo = new TH1F("pseudo","pseudo", bin_y, var_min, var_max);
//
//
//     vector<float> tau_vec;
//     ofstream outfile;
//     outfile.open("pseudos.csv");
//     outfile << "\"correlation\",\"tau\",\"asymmetry\"" << endl;
//
//     for(Int_t p=1; p<=NPSEUDO; p++)
//     {
//
//         if(p%1000 == 0) cout << p << endl;
//
//         hpseudo->Reset();
//         if(subtractData)
//         {
//             // Dice from backgrounds
//             for(int i = 0; i < nbkgs ; i++)
//             {
//                 //TH1F *heigen = (TH1F*)eigenhistos[i];
//                 TH1F *heigen = (TH1F*)bkghistos[i]; // FIXME Test dicing nominal
//                 TH1F *hclone = (TH1F*)heigen->Clone();
//
//                 //Float_t bla = random.Gaus(heigen->Integral(),eigenerrors[i]*heigen->Integral());
//                 Float_t bla = random.Gaus(heigen->Integral(),uncs[i+1]*heigen->Integral()); // FIXME test dicing nominal
//
//                 int n = random.Poisson(bla);
//
//                 for(int ibin = 1; ibin <= bin_y; ibin++)
//                 {
//                     Float_t val = hclone->GetBinContent(ibin);
//                     Float_t err = hclone->GetBinError(ibin);
//                     hclone->SetBinContent(ibin, random.Gaus(val, err));
//                 }
//
//                 for(int j = 0; j < n; j++)
//                 {
//                     hpseudo->Fill(hclone->GetRandom());
//                 }
//                 delete hclone;
//             }
//             // Dice from signal
//             int n = random.Poisson(hsignal->Integral());
//             TH1F *hclone = (TH1F*)hsignal->Clone();
//             for(int ibin = 1; ibin <= bin_y; ibin++)
//             {
//                 Float_t val = hclone->GetBinContent(ibin);
//                 Float_t err = hclone->GetBinError(ibin);
//                 hclone->SetBinContent(ibin, random.Gaus(val, err));
//             }
//             for(int j = 0; j < n; j++)
//             {
//                 hpseudo->Fill(hclone->GetRandom());
//             }
//             delete hclone;
//         }
//         else     // signal only
//         {
//             int n = random.Poisson(hrec->Integral());
//             TH1F *hclone = (TH1F*)hrec->Clone();
//             // Additional smearing
//             for(int ibin = 1; ibin <= bin_y; ibin++)
//             {
//                 Float_t val = hclone->GetBinContent(ibin);
//                 Float_t err = hclone->GetBinError(ibin);
//                 hclone->SetBinContent(ibin, random.Gaus(val, err));
//             }
//
//             for(int j = 0; j < n; j++)
//             {
//                 hpseudo->Fill(hclone->GetRandom());
//             }
//             delete hclone;
//         }
//
//         unfold.SetInput(hpseudo);
//         minimizeRhoAverage(&unfold, hpseudo, 1000, -6, -1); // mu
//         //minimizeRhoAverage(&unfold, hrec, 1000, -6, 0); // ele
//         const float tau = unfold.GetTau();
//         tau_vec.push_back(tau);
//         sum_tau += tau;
//
//         Double_t corr = unfold.DoUnfold(tau,hpseudo,scaleBias);
//         sum_corr += corr;
//
//         TH1F *hupseudo = new TH1F("upseudo","pseudo unfolded",bin_x,var_min,var_max);
//         unfold.GetOutput(hupseudo);
//
//         // Ematrix not containing all errors,
//         TH2F *hperr = new TH2F("perror","perror",bin_x,1,bin_x,bin_x,1,bin_x);
//         unfold.GetEmatrix(hperr);
//         // Add migration matrix stat. error
//         unfold.GetEmatrixSysUncorr(hperr, 0, false);
//
//         // Calculate asymmetry
//         Float_t asy = asymmetry(hupseudo);
//         outfile << corr << "," << tau << "," << asy << std::endl;
//         hasy.Fill(asy);
//         Float_t asy_diff = genasy - asy;
//         hasy_bias.Fill(asy_diff);
//         Float_t perror = error_unfold(hperr,hupseudo);
//         hStatErr.Fill(perror);
//         hasy_pull.Fill(asy_diff/perror);
//         // pull, rel. diff.
//         for(Int_t k=1; k<=bin_x; k++)
//         {
//             Float_t diff = (hgen_produced->GetBinContent(k) - hupseudo->GetBinContent(k));
//
//             hPull[k-1]->Fill(diff/hupseudo->GetBinError(k));
//             hBin[k-1]->Fill(diff/hgen_produced->GetBinContent(k));
//         }
//         delete hperr;
//         delete hupseudo;
//     }
//     // end pseudo
//
//     // write pseudo exp results
//     hasy.Write();
//     hasy_bias.Write();
//     hasy_pull.Write();
//     hStatErr.Write();
//
//     // write pull, bin histos
//     for(Int_t i=0; i < bin_x; i++)
//     {
//         hBin[i]->Write();
//         hPull[i]->Write();
//     }
//
//     fo->Close();
//     outfile.close();
//
//     const float mean_tau = sum_tau/NPSEUDO;
//
//     cout << "mean correlation: " << sum_corr/NPSEUDO << " mean tau: " << mean_tau << endl;
//
//     return mean_tau;
// }

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
	vector<Float_t> scales;
	vector<Float_t> uncs;
	vector<Float_t> preds;
	vector<TH1F*> bkghistos;
	TH1F *hsignal;

	vector<TH1F*> eigenhistos;
	vector<Float_t> eigenerrors;

	Int_t nbkgs = 0;
	Float_t sum_nonrot = 0;


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
            cout << "Could not load " << sig_name << ", using nominal" << endl;
            hsignal = (TH1F*)f->Get(var_y+"__tchan");
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
                cout << "Unable to read background histo " << bkg_name << ", using nominal" << endl;
                histo = (TH1F*)f->Get(var_y + "__" + name);
            } else {
                cout << "loaded background histogram for name=" << name << " " << bkg_name << endl;
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

	// set input distribution
	unfold.SetInput(hrec);

	// set different bias dist
    TH1F *hgen_produced = (TH1F*)hgen->Clone("hgen_produced");
	unfold.SetBias(hgen_produced);


	// subtract backgrounds
  // FIXME errors need to be relative!
	if(subtractData) {
		for(int i = 0; i < nbkgs; i++)
		{
			//unfold.SubtractBackground(eigenhistos[i],names[i+1],1.0, eigenerrors[i]);
			//unfold.SubtractBackground(bkghistos[i],names[i+1],1.0, uncs[i+1]); // FIXME Test subtracting nominal histos

          unfold.SubtractBackground(bkghistos[i],names[i+1], scales[i+1], uncs[i+1]/scales[i+1]); // FIXME Test subtracting nominal histos
		}
	}


	Float_t corr;
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
    std::cout << "mu x=" << hgenrecmu->ProjectionX()->GetEntries() << " y=" << hgenrecmu->ProjectionY()->GetEntries() << std::endl;
    std::cout << "ele x=" << hgenrecele->ProjectionX()->GetEntries() << " y=" << hgenrecele->ProjectionY()->GetEntries() << std::endl;
    std::cout << "tau x=" << hgenrectau->ProjectionX()->GetEntries() << " y=" << hgenrectau->ProjectionY()->GetEntries() << std::endl;
    TH2F *hgenrec = (TH2F*)hgenrecmu->Clone();

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
    std::cout << "tchan " << hrec_tchan->GetEntries() << std::endl;

    cout << "Rebinning hrec " << hrec->GetNbinsX() << " by " << NREBIN << endl;
    hrec->Rebin(NREBIN);
    cout << "Rebinning hgen " << hgen->GetNbinsX() << " by " << NREBIN << endl;
    hgen->Rebin(NREBIN);
    cout << "Rebinning hgenrec " << hgenrec->GetNbinsX() << ":" << hgenrec->GetNbinsY() << " by " << NREBIN << endl;
    hgenrec->Rebin2D(NREBIN, NREBIN);

    //hgen->Print("ALL");
    //hrec->Print("ALL");
    //hgenrec->Print("ALL");
    // reconstructed, generated, matrix, histo file
    unfold(hrec, hgen, hgenrec, f2, syst);

    cout << "writing comphep matrices" << endl;
    TFile *fo = new TFile(outfile,"UPDATE");
    TH2D *hgenrecmu_comphep = (TH2D*)fmu->Get("tm__comphep_nominal");
    if (hgenrecmu_comphep!=NULL) {

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
