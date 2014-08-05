CC=g++

#the full path to compiled TUnfold
#on cluster UI *.hep.kbfi.ee
#UNFOLD_DIR=/home/joosep/Dropbox/kbfi/top/stpol/tunfold
#on OSX laptop
UNFOLD_DIR=/Users/joosep/Documents/tunfold
#UNFOLD_DIR=/home/fynu/mkomm/stpol/unfold/tunfold17.3

CXXFLAGS= -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs`
#LDFLAGS=-L$(UNFOLD_DIR) $(UNFOLD_DIR)/libunfold.so -lMinuit -lXMLParser
#need to ling libunfold0 on OSX
LDFLAGS=-L$(UNFOLD_DIR) $(UNFOLD_DIR)/libunfold0.so -lMinuit -lXMLParser

all: unfold

#only works in SLC6, or new osx
unfold: unfold.cc info.hpp utils.hpp
	c++ -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs` $(LDFLAGS) unfold.cc info.hpp utils.hpp
	mv a.out unfold

calc_asymmetry: calc_asymmetry.C info.hpp utils.hpp
calc_asymmetry_syst: calc_asymmetry_syst.C info.hpp utils.hpp

closure: closure.C info.h utils.hpp

clean:
	rm -f unfold unfold_pseudo calc_asymmetry unfold_systematics calc_asymmetry_syst

clean_output:
	rm -f mu_*.csv ele_*.csv

#path to histograms
DATADIR=../../results/hists/Aug1/merged/0.60000

do_unfold_mu:
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ nominal fitresults/nominal/mu histos/mu__nominal.root
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape wjets_flavour_heavy wjets_flavour_light qcd_antiiso wjets dyjets diboson ttjets twchan schan qcd_yield; do \
		DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ "$$SYS"__up fitresults/"$$SYS"__up/mu histos/mu__"$$SYS"__up.root; \
		DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ "$$SYS"__down fitresults/"$$SYS"__down/mu histos/mu__"$$SYS"__down.root; \
	done

do_unfold_combined:
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/combined/ nominal fitresults/nominal/combined histos/combined__nominal.root
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape wjets_flavour_heavy wjets_flavour_light qcd_antiiso wjets dyjets diboson ttjets twchan schan qcd_yield; do \
		DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/combined/ "$$SYS"__up fitresults/"$$SYS"__up/combined histos/combined__"$$SYS"__up.root; \
		DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/combined/ "$$SYS"__down fitresults/"$$SYS"__down/combined histos/combined__"$$SYS"__down.root; \
	done

do_unfold_ele:
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ nominal fitresults/nominal/ele histos/ele__nominal.root
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape wjets_flavour_heavy wjets_flavour_light qcd_antiiso wjets dyjets diboson ttjets twchan schan qcd_yield; do \
		DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ "$$SYS"__up fitresults/"$$SYS"__up/ele histos/ele__"$$SYS"__up.root; \
		DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ "$$SYS"__down fitresults/"$$SYS"__down/ele histos/ele__"$$SYS"__down.root; \
	done
	
unfold_nominal:	
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ nominal fitresults/nominal/mu histos/mu__nominal.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ nominal fitresults/nominal/ele histos/ele__nominal.root
	

unfold_2bin_mu:
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met nominal pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape wjets_flavour_heavy wjets_flavour_light qcd_antiiso wjets dyjets diboson ttjets twchan schan qcd_yield ; do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=$$SYS \
		--output=mu_$$SYS.csv; \
	done
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--no-mcstat \
		--no-fiterror \
		--output=mu_stat.csv
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--no-stat \
		--no-fiterror \
		--output=mu_mcstat.csv
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=mu_fiterror.csv
	julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_mu_2bin.csv mu_*.csv

unfold_2bin_ele:
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met nominal pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape wjets_flavour_heavy wjets_flavour_light qcd_antiiso wjets dyjets diboson ttjets twchan schan qcd_yield ; do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=$$SYS \
		--output=ele_$$SYS.csv; \
	done
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--no-mcstat \
		--no-fiterror \
		--output=ele_stat.csv
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--no-stat \
		--no-fiterror \
		--output=ele_mcstat.csv
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=ele_fiterror.csv
	julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_ele_2bin.csv ele_*.csv

unfold_2bin_combined:
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met nominal pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape wjets_flavour_heavy wjets_flavour_light qcd_antiiso wjets dyjets diboson ttjets twchan schan qcd_yield ; do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=$$SYS \
		--output=combined_$$SYS.csv; \
	done
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--no-mcstat \
		--no-fiterror \
		--output=combined_stat.csv
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--no-stat \
		--no-fiterror \
		--output=combined_mcstat.csv
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=combined_fiterror.csv
	julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_combined_2bin.csv combined_*.csv

.PHONY: clean clean_output
