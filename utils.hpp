#ifndef UTILS_HPP
#define UTILS_HPP
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TMatrix.h"
#include "TString.h"
#include "TFile.h"
#include "TVector.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

typedef TMatrixT<double>	TMatrixD;

void read_fitres(TString fresult, vector<TString> &names, vector<double> &scales, vector<double> &uncs)
{
    TString name;
    double scale, unc;
    
	//cout << "reading fit results: " << fresult << endl;
    
    ifstream ifs;
    ifs.open(fresult+".txt");
    if(!ifs.good()) {
        cerr << "ERROR: could not open fit results file! fitresults/" << fresult << ".txt" << endl;
        exit(1);
    }
    while (ifs >> name >> scale >> unc) {
        names.push_back(name);
        scales.push_back(Float_t(scale));
        uncs.push_back(Float_t(unc));
        cout << "fit result: " << name << " " << scale << " " << unc << endl;
    }
    ifs.close();
}

void decorrelate(TH2F* cov, vector<TH1F*> bkghistos, vector<TH1F*> &eigenhistos, vector<double> &eigenerrors) {
	int nbkgs = bkghistos.size();
	// Decorrelate background templates
	// Read in covariance matrix
    
	TMatrixD covmatrix(nbkgs,nbkgs);
    
	// Fill cov matrix, skip first entry with beta_signal
	for(int i = 0; i < nbkgs ; i++) {
		for(int j = 0; j < nbkgs; j++) {
			covmatrix[i][j] = cov->GetBinContent(i+2,j+2);
		}
	}
    
	TVectorD eigenvalues(nbkgs);
	TMatrixD eigenvectors = covmatrix.EigenVectors(eigenvalues);
    
	// Unit vector
	TVectorD unitvec(nbkgs);
	for(int i = 0; i < nbkgs; i++) unitvec[i] = 1;
    
	// Inverted eigenvectors
	TMatrixD inv_eigenvectors(eigenvectors);
	inv_eigenvectors.Invert();
    
	unitvec *= inv_eigenvectors;
	// Scale vector to keep norm
	TVectorD scale_vector(unitvec);
    
	// Apply scale factors to eigenvectors
	for(int i = 0; i < nbkgs; i++)
	{
		for(int j = 0; j < nbkgs; j++)
		{
			eigenvectors[i][j] *= scale_vector[j];
		}
	}
    
	double sum_rot = 0;
	// Rotate backgrounds
	for(int i = 0; i < nbkgs; i++)
	{
		TH1F *eigenhisto = (TH1F*)bkghistos[i]->Clone();
		eigenhisto->Reset();
        
		// Add up eigenhistos
		for(int j = 0; j < nbkgs; j++)
		{
			// First index: row, element of vector
			// Second index: column, index of vector
			eigenhisto->Add(bkghistos[j], eigenvectors(j,i));
		}
		eigenhistos.push_back((TH1F*)eigenhisto);
		sum_rot += eigenhisto->Integral();
		eigenerrors.push_back(sqrt(eigenvalues[i]));
	}
}

void fill_nooverflow_1d(TH1* h, double val, double weight)
{
	if(val > h->GetXaxis()->GetXmax()) val = h->GetXaxis()->GetXmax()-0.00001;
	if(val < h->GetXaxis()->GetXmin()) val = h->GetXaxis()->GetXmin()+0.00001;
	h->Fill(val, weight);
}

void fill_nooverflow_2d(TH2* h, double valx, double valy, double weight)
{
	const double xmax = h->GetXaxis()->GetXmax();
	const double xmin = h->GetXaxis()->GetXmin();
	const double ymax = h->GetYaxis()->GetXmax();
	const double ymin = h->GetYaxis()->GetXmin();
    
	if(valx > xmax) valx =xmax-0.00001;
	if(valx < xmin) valx = xmin+0.00001;
    
	if(valy > ymax) valy = ymax-0.00001;
	if(valy < ymin) valy = ymin+0.00001;
    
	h->Fill(valx, valy, weight);
}

Float_t error_naive(Float_t plus, Float_t minus)
{
	return 2/TMath::Power(plus+minus,2)*TMath::Sqrt(minus*plus*plus+plus*minus*minus);
}

// Im = \int_{-1}^0 dsigma/dcostheta dcosthetae 
// Ip = \int_0^{+1} dsigma/dcostheta dcosthetae 
// dm = 2 * Im / I^2
// dm = 2 * Ip / I^2

Float_t error_unfold(TH2F* errmat, TH1F* unf)
{
	Int_t bin_zero =  unf->FindBin(0.0);
	Int_t bin_last =  unf->GetXaxis()->GetNbins();
    
	Float_t integral_plus = unf->Integral(bin_zero,bin_last);
	Float_t integral_minus = unf->Integral(1,bin_zero-1);
	Float_t integral = unf->Integral(1,bin_last);
    
	// Derivatives
	Float_t dadn_plus = 2*integral_minus/TMath::Power(integral,2);
	Float_t dadn_minus = -2*integral_plus/TMath::Power(integral,2);
    
	TH1F vec("vec","vec",bin_last, 0, bin_last);
	TH2F corr("corr","corr",bin_last, 0, bin_last, bin_last, 0, bin_last);
	TH1F vecT("vecT","vecT",bin_last, 0, bin_last);
    
	for(Int_t i = 1; i <= bin_last; i++)
	{
		if(i<bin_zero)
			vec.SetBinContent(i-1,dadn_minus);
		if(i>=bin_zero)
			vec.SetBinContent(i-1,dadn_plus);
        
		vecT.SetBinContent(i-1,0);
        
		for(Int_t j = 1; j <= bin_last; j++)
		{
			corr.SetBinContent(i-1,j-1,errmat->GetBinContent(i,j));
		}
	}
    
	//
    
	for(Int_t i = 0; i < bin_last; i++)
		for(Int_t j = 0; j < bin_last; j++)
			vecT.SetBinContent(i, vecT.GetBinContent(i) + corr.GetBinContent(i,j)*vec.GetBinContent(j));
    
	Float_t error = 0;
    
	for(Int_t i = 0; i < bin_last; i++)
		error += vecT.GetBinContent(i)*vec.GetBinContent(i);
    
	error = TMath::Sqrt(error);
    
	return error;
}

Double_t asymmetry(TH1F *hist)
{
	// Underflow and overflow should not be filled
	//cout << hist->GetBinContent(0) << endl;
	//cout << hist->GetBinContent(hist->GetNbinsX()+1) << endl;
    
	Int_t bin_zero =  hist->FindBin(0.0);
	Int_t bin_last =  hist->GetXaxis()->GetNbins();
	//cout << "zero bin: " << bin_zero << endl;
	//cout << "last bin: " << bin_last << endl;
    
	Int_t integral_plus = hist->Integral(bin_zero,bin_last);
	Int_t integral_minus = hist->Integral(1,bin_zero-1);
	Int_t integral = hist->Integral(1,bin_last);
	Double_t asym = (integral_plus-integral_minus)/(Double_t)integral;
	//cout << "integral+:" << integral_plus << endl;
	//cout << "integral-:" << integral_minus << endl;
	//cout << "integral:" << integral << endl;
	//cout << "asymmetry: " << asym << " +- " << error_naive(integral_plus , integral_minus) << endl;
	return asym;
}
#endif
