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

const Int_t bin_x = 6;
const Int_t bin_y = 12;

//const Int_t bin_x = 12;
//const Int_t bin_y = 24;

//TString sample = "scanned_hists_may29/hists/0.60000";
TString sample = "scanned_hists_jun13/hists/0.60000";

const Float_t tau = 2.63086e-05;
//const Float_t tau = 2.80988e-05; // new mu
//const Float_t tau = 2.24251e-05; // new ele

#endif
