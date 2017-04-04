#ifndef BINNINGS_H
#define BINNINGS_H
#include "TString.h"

// Number of reconstructed bins should be twice as much as generated
// x: generated
// y: reconstructed
// cosTheta

TString varname = "cos#theta_{l,j}";

TString var_x = "true_cos_theta";
TString var_y = "2j1t_cos_theta_lj";

const Double_t var_min = -1;
const Double_t var_max = 1;

//Rebinning constant
const unsigned int NREBIN = 4;

//Number of generated(x) and reconstructed(y) bins in the transfer matrix
const Int_t bin_x = 24/NREBIN;
const Int_t bin_y = 48/NREBIN;

//Systematic scenario, e.g. "nominal", "met__up", "jes__down" etc
TString syst;

//path to the histograms
TString sample;

//path to the fitresults
TString fitres;

//output file path
TString outfile;

//regularization parameter, determined by Steffen "manually"
Float_t tau = 2.63086e-05;

//number of pseudo-experiments to throw
const int NPSEUDO = 10000;

#endif
