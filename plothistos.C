#include "binnings.h"
#include "utils.hpp"
#include "plotutils.hpp"

plot_unfolded()
{
	TCanvas *cunf = new TCanvas("unfolded","unfolded",800,600);
	TFile *f1 = new TFile("histos/unfolded.root");
	TFile *f2 = new TFile("histos/"+sample+"/mu/tmatrix_nocharge.root");

	TH1D *hunf = (TH1D*)f1->Get("unfolded");
  TH2F *hgenrec = (TH2F*)f2->Get("tm__pdgid_13__nominal");
	TH1F *hgen = (TH1F*)f2->Get("tm__pdgid_13__nominal__proj_x");
	TH1F *hgen = (TH1F*)hgenrec->ProjectionX();
  TH1F *hgen_produced = (TH1F*)hgen->Clone("hgen_produced");

  // Normalize to one
//	hgen_produced->Scale(1.0/hgen_produced->Integral());
//	hunf->Scale(1.0/hunf->Integral());

	// color
  hunf->SetLineColor(kRed+1);
  hgen_produced->SetLineColor(kGreen+1);

	hgen_produced->SetStats(0);
	hgen_produced->SetTitle("");
	hgen_produced->SetMaximum(hgen_produced->GetMaximum()*1.2);
	hgen_produced->SetMinimum(0);
	hgen_produced->GetXaxis()->SetTitle(varname);
	hgen_produced->GetYaxis()->SetTitle("Events");
	hgen_produced->Draw("HIST,E");
	hunf->Draw("SAME E");
	
	TLegend *leg = new TLegend(0.25,0.8,0.45,0.88);
  leg->AddEntry(hunf,"Unfolded","l");
  leg->AddEntry(hgen_produced,"Generated","l");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
	leg->Draw();

	cunf->Print("plots/unfolded.pdf");
	cunf->Print("plots/unfolded.png");
}

plot_shape()
{	
	TCanvas *ceff = new TCanvas("ceff","efficiency",800,600);
	//TFile *f = new TFile("histos/"+sample+"/rebinned.root");
	TFile *f = new TFile("histos/"+sample+"/rebinned_nominal.root");
	TH1D *hgen_presel = (TH1D*)f->Get("hgen_presel");
	TH1D *hgen = (TH1D*)f->Get("hgen");
	TH1D *hrec = (TH1D*)f->Get("hrec");

	// Normalize
	hgen_presel->Scale(1/hgen_presel->Integral());
	hgen->Scale(1/hgen->Integral());
	hrec->Scale(1/hrec->Integral());
	
	// rebin
	//hgen_presel->Rebin(2);
	//hgen->Rebin(2);
	hrec->Rebin(2);

	// color
  hrec->SetLineColor(kRed+1);
  hgen->SetLineColor(kGreen+1);
  hgen_presel->SetLineColor(kBlue-1);

	hgen_presel->SetStats(0);
	hgen_presel->SetTitle("");
	hgen_presel->GetXaxis()->SetTitle("Shape of "+varname);
	hgen_presel->GetYaxis()->SetTitle("Norm. to unit area");
	hgen_presel->SetMaximum(hrec->GetMaximum()*1.1);
	hgen_presel->Draw("HIST");
	hgen->Draw("HIST SAME");
	hrec->Draw("HIST SAME");

	TLegend *leg = new TLegend(0.15,0.76,0.55,0.88);
  leg->AddEntry(hgen_presel,"Generated","l");
  leg->AddEntry(hgen,"Generated after cuts","l");
  leg->AddEntry(hrec,"Reconstructed","l");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
	leg->Draw();
	
	ceff->Print("plots/shape.pdf");
	ceff->Print("plots/shape.png");
}

plot_asymmetry_pull()
{
	TCanvas *casy = new TCanvas("casy","asymmetry",800,600);
	TFile *f = new TFile("histos/pseudoexperiments.root");

	// Plot pull
	TH1D *hasy_pull = (TH1D*)f->Get("asymmetry_pull");
	
	hasy_pull->SetStats(0);
	hasy_pull->SetTitle("");
	hasy_pull->GetXaxis()->SetTitle("Asymmetry pull");
	hasy_pull->GetYaxis()->SetTitle("# PEs");
	
	adaptrange(hasy_pull);
	hasy_pull->SetMaximum(hasy_pull->GetMaximum()*1.3);
	hasy_pull->Draw("HIST");
	
	hasy_pull->Fit("gaus");
	hasy_pull->GetFunction("gaus")->SetLineColor(kBlue);
	TLegend *leg = new TLegend(0.6,0.76,0.85,0.88);
  leg->AddEntry(hasy_pull,"PEs","l");
  leg->AddEntry(hasy_pull->GetFunction("gaus"),"Gaussian fit","l");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
	leg->Draw();
	
	Double_t mean = hasy_pull->GetMean();
	Double_t mean_err = hasy_pull->GetMeanError();
	Double_t rms = hasy_pull->GetRMS();
	Double_t rms_err = hasy_pull->GetRMSError();

	//TString info1 = TString::Format("Mean: %.4f#pm%.4f",mean,mean_err);
	//TString info2 = TString::Format("RMS:  %.4f#pm%.4f",rms,rms_err);
	TString info1 = TString::Format("Mean: %.4f",mean);
	TString info2 = TString::Format("RMS:  %.4f",rms);
	TLatex linfo1;
	linfo1.SetNDC();
	linfo1.DrawLatex(0.15,0.85,info1);
	TLatex linfo2;
	linfo2.SetNDC();
	linfo2.DrawLatex(0.15,0.82,info2);

	casy->Print("plots/asymmetry_pull.pdf");
	casy->Print("plots/asymmetry_pull.png");
}

plot_asymmetry_bias()
{
	TCanvas *casy = new TCanvas("casy","asymmetry",800,600);
	TFile *f = new TFile("histos/pseudoexperiments.root");

	// Plot bias
	TH1D *hasy_bias = (TH1D*)f->Get("asymmetry_bias");
	
	hasy_bias->SetStats(0);
	hasy_bias->SetTitle("");
	hasy_bias->GetXaxis()->SetTitle("Asymmetry bias");
	hasy_bias->GetYaxis()->SetTitle("# PEs");
	
	adaptrange(hasy_bias);
	hasy_bias->SetMaximum(hasy_bias->GetMaximum()*1.3);
	hasy_bias->Draw("HIST");
	
	hasy_bias->Fit("gaus");
	hasy_bias->GetFunction("gaus")->SetLineColor(kBlue);
	TLegend *leg = new TLegend(0.6,0.76,0.85,0.88);
  leg->AddEntry(hasy_bias,"PEs","l");
  leg->AddEntry(hasy_bias->GetFunction("gaus"),"Gaussian fit","l");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
	leg->Draw();
	
	Double_t mean = hasy_bias->GetMean();
	Double_t mean_err = hasy_bias->GetMeanError();
	Double_t rms = hasy_bias->GetRMS();
	Double_t rms_err = hasy_bias->GetRMSError();

	//TString info1 = TString::Format("Mean: %.4f#pm%.4f",mean,mean_err);
	//TString info2 = TString::Format("RMS:  %.4f#pm%.4f",rms,rms_err);
	TString info1 = TString::Format("Mean: %.4f",mean);
	TString info2 = TString::Format("RMS:  %.4f",rms);
	TLatex linfo1;
	linfo1.SetNDC();
	linfo1.DrawLatex(0.15,0.85,info1);
	TLatex linfo2;
	linfo2.SetNDC();
	linfo2.DrawLatex(0.15,0.82,info2);

	casy->Print("plots/asymmetry_bias.pdf");
	casy->Print("plots/asymmetry_bias.png");
}

plot_asymmetry()
{	
	TCanvas *casy = new TCanvas("casy","asymmetry",800,600);
	TFile *f = new TFile("histos/pseudoexperiments.root");

	// Plot pull
	TH1D *hasy = (TH1D*)f->Get("asymmetry");
	
	hasy->SetStats(0);
	hasy->SetTitle("");
	hasy->GetXaxis()->SetTitle("Asymmetry");
	hasy->GetYaxis()->SetTitle("# PEs");
	
	adaptrange(hasy);
	hasy->SetMaximum(hasy->GetMaximum()*1.3);
	hasy->Draw("HIST");
	
	hasy->Fit("gaus");
	hasy->GetFunction("gaus")->SetLineColor(kBlue);
	TLegend *leg = new TLegend(0.6,0.76,0.85,0.88);
  leg->AddEntry(hasy,"PEs","l");
  leg->AddEntry(hasy->GetFunction("gaus"),"Gaussian fit","l");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
	leg->Draw();
	
	Double_t mean = hasy->GetMean();
	Double_t mean_err = hasy->GetMeanError();
	Double_t rms = hasy->GetRMS();
	Double_t rms_err = hasy->GetRMSError();

	//TString info1 = TString::Format("Mean: %.4f#pm%.4f",mean,mean_err);
	//TString info2 = TString::Format("RMS:  %.4f#pm%.4f",rms,rms_err);
	TString info1 = TString::Format("Mean: %.4f",mean);
	TString info2 = TString::Format("RMS:  %.4f",rms);
	TLatex linfo1;
	linfo1.SetNDC();
	linfo1.DrawLatex(0.15,0.85,info1);
	TLatex linfo2;
	linfo2.SetNDC();
	linfo2.DrawLatex(0.15,0.82,info2);

	casy->Print("plots/asymmetry.pdf");
	casy->Print("plots/asymmetry.png");
}

plot_staterror()
{
	TCanvas *cstaterr = new TCanvas("cstaterr","error",800,600);
	TFile *f = new TFile("histos/pseudoexperiments.root");
	TH1D *herror = (TH1D*)f->Get("staterr");
	
	
	herror->SetStats(0);
	herror->SetTitle("");
	herror->GetXaxis()->SetTitle("Asymmetry stat. uncertainty");
	herror->GetYaxis()->SetTitle("# PEs");
	
	adaptrange(herror);
	herror->SetMaximum(herror->GetMaximum()*1.3);
	herror->Draw("HIST");
	
	herror->Fit("gaus");
	herror->GetFunction("gaus")->SetLineColor(kBlue);
	TLegend *leg = new TLegend(0.6,0.76,0.85,0.88);
  leg->AddEntry(herror,"PEs","l");
  leg->AddEntry(herror->GetFunction("gaus"),"Gaussian fit","l");
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
	leg->Draw();
	
	Double_t mean = herror->GetMean();
	Double_t mean_err = herror->GetMeanError();
	Double_t rms = herror->GetRMS();
	Double_t rms_err = herror->GetRMSError();

	//TString info1 = TString::Format("Mean: %.4f#pm%.4f",mean,mean_err);
	//TString info2 = TString::Format("RMS:  %.4f#pm%.4f",rms,rms_err);
	TString info1 = TString::Format("Mean: %.4f",mean);
	TString info2 = TString::Format("RMS:  %.4f",rms);
	TLatex linfo1;
	linfo1.SetNDC();
	linfo1.DrawLatex(0.15,0.85,info1);
	TLatex linfo2;
	linfo2.SetNDC();
	linfo2.DrawLatex(0.15,0.82,info2);

	cstaterr->Print("plots/asymmetry_staterror.pdf");
	cstaterr->Print("plots/asymmetry_staterror.png");
}

plot_error()
{
	TCanvas *cerror = new TCanvas("cerror","Error matrix",800,600);
	TFile *f = new TFile("histos/unfolded.root");
	TH2D *herror = (TH2D*)f->Get("error");

	herror->SetStats(0);
	herror->Draw("COLZ");
	//herror->Draw("COLZ TEXT");
	cerror->Print("plots/error.pdf");
	cerror->Print("plots/error.png");
}

plot_correlation()
{
	TCanvas *ccorr = new TCanvas("ccorr","Correlation matrix",800,600);
	TFile *f = new TFile("histos/unfolded.root");
	TH2D *hcorr = (TH2D*)f->Get("correlation");

	hcorr->SetStats(0);
	hcorr->Draw("COLZ");
	ccorr->Print("plots/correlation.pdf");
	ccorr->Print("plots/correlation.png");
}

// FIXME
plot_efficiency()
{
	TCanvas *ceff= new TCanvas("ceff","Efficiency",800,600);
	//TFile *feff = new TFile("histos/"+sample+"/rebinned.root");
	TFile *feff = new TFile("histos/"+sample+"/rebinned_nominal.root");
	TH1D *heff = (TH1D*)feff->Get("efficiency");
	heff->SetStats(0);
	heff->SetTitle("");
	heff->GetXaxis()->SetTitle(varname);
	heff->GetYaxis()->SetTitle("Selection efficiency");

	heff->Draw("HIST E");
	ceff->Print("plots/efficiency.pdf");
	ceff->Print("plots/efficiency.png");
}


plot_matrix()
{
	TCanvas *cmat = new TCanvas("cmat","matrix",800,600);
	TFile *f = new TFile("histos/"+sample+"/mu/tmatrix_nocharge.root");

  TH2F *hmat = (TH2F*)f->Get("tm__pdgid_13__nominal");

	Int_t nbinsx = hmat->GetNbinsX();
	Int_t nbinsy = hmat->GetNbinsY();
	Double_t *n_x = new Double_t[nbinsx+1];
	Double_t *n_y = new Double_t[nbinsy+1];

	// plot in bins
	for(Int_t i=0;i<=nbinsx;i++) n_x[i] = i;
	for(Int_t i=0;i<=nbinsy;i++) n_y[i] = i;
	
	TH2D *hnew = new TH2D("migmatrix","migmatrix",nbinsx,n_x,nbinsy,n_y);
	

	for(Int_t i=1;i<=nbinsx;i++) {
		TString label = "";
		label += i;
		hnew->GetXaxis()->SetBinLabel(i,label);
	}
	for(Int_t j=1;j<=nbinsy;j++) {
		TString label = "";
		label += j;
		hnew->GetYaxis()->SetBinLabel(j,label);
	}

  for(Int_t i=1;i<=nbinsx;i++) {
    Double_t colsum = 0;
    for(Int_t j=1;j<=nbinsy;j++) {
      Double_t binc = hmat->GetBinContent(i,j);
      colsum += binc;
      hnew->SetBinContent(i,j,binc);
    }
    // FIXME percentages
    for(Int_t j=1;j<=nbinsy;j++) {
      Double_t binc = hmat->GetBinContent(i,j);
      hnew->SetBinContent(i,j,binc/colsum);
    }
  }
  
  // Create color palette with higher resolution for small percentages
  const Int_t Number = 5;
  Double_t Red[Number]    ={0.2,0.2,0.67,0.93, 1.00};
  Double_t Green[Number]  = {0.2 ,0.63,0.87,0.68,0.19};
  Double_t Blue[Number]   = {0.8 ,0.87,0.0, 0.05,0.19};
  Double_t Length[Number] = {0.05,0.1,0.3,0.4,.5};
  Int_t nb=200;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

	hnew->SetTitle("");
	hnew->SetStats(0);
	hnew->GetXaxis()->SetTitle("Bins of generated "+varname);
	hnew->GetYaxis()->SetTitle("Bins of reconstructed "+varname);
  hnew->SetContour(100);

	hnew->Draw("COLZ");

	cmat->Print("plots/matrix.pdf");
	cmat->Print("plots/matrix.png");
}

plot_pseudo()
{
	//gStyle->SetOptStat("mr");
	gStyle->SetOptStat("");

	TFile *f = new TFile("histos/pseudoexperiments.root");

	// pull
	TCanvas *c1 = new TCanvas("cpull","pull",800,600);
	c1->Divide(3,2);
	c1->cd();
	for(Int_t i = 1; i <= bin_x; i++) {
		TString pulname = "pull_";
		TString pultitle = "Pull in bin ";
		pulname += i;
		pultitle +=i;
		TH1D *hpull = (TH1D*)f->Get(pulname);
		adaptrange(hpull);
		c1->cd(i);
		hpull->SetMaximum(hpull->GetMaximum()*1.15);
		hpull->SetTitle("");
		hpull->GetXaxis()->SetTitle(pultitle);
		hpull->Draw("HIST E");

		Double_t mean = hpull->GetMean();
		Double_t mean_err = hpull->GetMeanError();
		Double_t rms = hpull->GetRMS();
		Double_t rms_err = hpull->GetRMSError();

		TString info1 = TString::Format("Mean: %.4f#pm%.4f",mean,mean_err);
		TString info2 = TString::Format("RMS:  %.4f#pm%.4f",rms,rms_err);
		TLatex linfo1;
		linfo1.SetNDC();
		linfo1.DrawLatex(0.15,0.85,info1);
		TLatex linfo2;
		linfo2.SetNDC();
		linfo2.DrawLatex(0.15,0.82,info2);

	}
	c1->Print("plots/pull.pdf");
	c1->Print("plots/pull.png");
	
	// reldiff
	TCanvas *c2 = new TCanvas("crel","relative difference",800,600);
	c2->Divide(3,2);
	c2->cd();
	for(Int_t i = 1; i <= bin_x; i++) {
		TString relname = "reldiff_";
		TString reltitle = "Rel. difference in bin ";
		relname += i;
		reltitle += i;
		TH1D *hrel = (TH1D*)f->Get(relname);
		adaptrange(hrel);
		c2->cd(i);
		hrel->SetMaximum(hrel->GetMaximum()*1.15);
		hrel->SetTitle("");
		hrel->GetXaxis()->SetTitle(reltitle);
		hrel->Draw("HIST E");
	
		Double_t mean = hrel->GetMean();
		Double_t mean_err = hrel->GetMeanError();
		Double_t rms = hrel->GetRMS();
		Double_t rms_err = hrel->GetRMSError();

		TString info1 = TString::Format("Mean: %.4f#pm%.4f",mean,mean_err);
		TString info2 = TString::Format("RMS:  %.4f#pm%.4f",rms,rms_err);
		TLatex linfo1;
		linfo1.SetNDC();
		linfo1.DrawLatex(0.15,0.85,info1);
		TLatex linfo2;
		linfo2.SetNDC();
		linfo2.DrawLatex(0.15,0.82,info2);
	}
	c2->Print("plots/reldiff.pdf");
	c2->Print("plots/reldiff.png");
}

plothistos()
{
	plot_unfolded();
	plot_matrix();
	plot_correlation();
	plot_error();
	plot_asymmetry();
	plot_asymmetry_bias();
	plot_asymmetry_pull();
	plot_staterror();
	plot_pseudo();
  /*
	plot_efficiency();
	plot_shape();
  */
}
