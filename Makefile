CC=g++

#the full path to compiled TUnfold
#UNFOLD_DIR=/home/joosep/Dropbox/kbfi/top/stpol/tunfold
UNFOLD_DIR=/home/fynu/mkomm/stpol/unfold/tunfold17.3

CXXFLAGS= -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs`
LDFLAGS=-L$(UNFOLD_DIR) $(UNFOLD_DIR)/libunfold.so -lMinuit -lXMLParser

all: unfold

#unfold: unfold.cc info.hpp utils.hpp
#unfold_pseudo: unfold_pseudo.cc info.hpp utils.hpp
#unfold_systematics: unfold_systematics.cc info.hpp utils.hpp
#unfold_scan_dice: unfold_scan_dice.cc unfold.hpp utils.hpp info.hpp

#only works in SLC6, or new osx
unfold: unfold.cc info.hpp utils.hpp
	c++ -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs` $(LDFLAGS) unfold.cc info.hpp utils.hpp
	mv a.out unfold

#unfold_pseudo: unfold_pseudo.cc info.hpp utils.hpp
#	c++ -Wall -O2 -I/Users/joosep/Documents/tunfold  `root-config --cflags --libs`  -L/Users/joosep/Documents/tunfold -lunfold0 -lMinuit -lXMLParser unfold_pseudo.cc info.hpp utils.hpp
#	mv a.out unfold_pseudo

calc_asymmetry: calc_asymmetry.C info.hpp utils.hpp
calc_asymmetry_syst: calc_asymmetry_syst.C info.hpp utils.hpp

closure: closure.C info.h utils.hpp

clean:
	rm -f unfold unfold_pseudo calc_asymmetry unfold_systematics calc_asymmetry_syst

#path to histograms
DATADIR=/nfs/user/mkomm/scanned_hists_jul31/0.60000

do_unfold_mu:
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ nominal fitresults/nominal/mu histos/mu__nominal.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ met__down fitresults/met__down/mu histos/mu__met__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ met__up fitresults/met__up/mu histos/mu__met__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ jes__down fitresults/jes__down/mu histos/mu__jes__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ jes__up fitresults/jes__up/mu histos/mu__jes__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ jer__down fitresults/jer__down/mu histos/mu__jer__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ jer__up fitresults/jer__up/mu histos/mu__jer__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ scale_tchan__down fitresults/scale_tchan__down/mu histos/mu__scale_tchan__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ scale_tchan__up fitresults/scale_tchan__up/mu histos/mu__scale_tchan__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ scale_ttjets__down fitresults/scale_ttjets__down/mu histos/mu__scale_ttjets__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ scale_ttjets__up fitresults/scale_ttjets__up/mu histos/mu__scale_ttjets__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ scale_wjets__down fitresults/scale_wjets__down/mu histos/mu__scale_wjets__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ scale_wjets__up fitresults/scale_wjets__up/mu histos/mu__scale_wjets__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ pu__down fitresults/pu__down/mu histos/mu__pu__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ pu__up fitresults/pu__up/mu histos/mu__pu__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ top_weight__down fitresults/top_weight__down/mu histos/mu__top_weight__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ top_weight__up fitresults/top_weight__up/mu histos/mu__top_weight__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ mass__down fitresults/mass__down/mu histos/mu__mass__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ mass__up fitresults/mass__up/mu histos/mu__mass__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ matching__down fitresults/matching__down/mu histos/mu__matching__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ matching__up fitresults/matching__up/mu histos/mu__matching__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ btag_bc__down fitresults/btag_bc__down/mu histos/mu__btag_bc__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ btag_bc__up fitresults/btag_bc__up/mu histos/mu__btag_bc__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ btag_l__down fitresults/btag_l__down/mu histos/mu__btag_l__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ btag_l__up fitresults/btag_l__up/mu histos/mu__btag_l__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ lepton_id__down fitresults/lepton_id__down/mu histos/mu__lepton_id__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ lepton_id__up fitresults/lepton_id__up/mu histos/mu__lepton_id__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ lepton_iso__down fitresults/lepton_iso__down/mu histos/mu__lepton_iso__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ lepton_iso__up fitresults/lepton_iso__up/mu histos/mu__lepton_iso__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ lepton_trigger__down fitresults/lepton_trigger__down/mu histos/mu__lepton_trigger__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ lepton_trigger__up fitresults/lepton_trigger__up/mu histos/mu__lepton_trigger__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ wjets_shape__down fitresults/wjets_shape__down/mu histos/mu__wjets_shape__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ wjets_shape__up fitresults/wjets_shape__up/mu histos/mu__wjets_shape__up.root


do_unfold_ele:
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ nominal fitresults/nominal/ele histos/ele__nominal.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ met__down fitresults/met__down/ele histos/ele__met__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ met__up fitresults/met__up/ele histos/ele__met__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ jes__down fitresults/jes__down/ele histos/ele__jes__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ jes__up fitresults/jes__up/ele histos/ele__jes__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ jer__down fitresults/jer__down/ele histos/ele__jer__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ jer__up fitresults/jer__up/ele histos/ele__jer__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ scale_tchan__down fitresults/scale_tchan__down/ele histos/ele__scale_tchan__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ scale_tchan__up fitresults/scale_tchan__up/ele histos/ele__scale_tchan__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ scale_ttjets__down fitresults/scale_ttjets__down/ele histos/ele__scale_ttjets__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ scale_ttjets__up fitresults/scale_ttjets__up/ele histos/ele__scale_ttjets__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ scale_wjets__down fitresults/scale_wjets__down/ele histos/ele__scale_wjets__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ scale_wjets__up fitresults/scale_wjets__up/ele histos/ele__scale_wjets__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ pu__down fitresults/pu__down/ele histos/ele__pu__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ pu__up fitresults/pu__up/ele histos/ele__pu__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ top_weight__down fitresults/top_weight__down/ele histos/ele__top_weight__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ top_weight__up fitresults/top_weight__up/ele histos/ele__top_weight__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ mass__down fitresults/mass__down/ele histos/ele__mass__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ mass__up fitresults/mass__up/ele histos/ele__mass__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ matching__down fitresults/matching__down/ele histos/ele__matching__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ matching__up fitresults/matching__up/ele histos/ele__matching__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ btag_bc__down fitresults/btag_bc__down/ele histos/ele__btag_bc__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ btag_bc__up fitresults/btag_bc__up/ele histos/ele__btag_bc__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ btag_l__down fitresults/btag_l__down/ele histos/ele__btag_l__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ btag_l__up fitresults/btag_l__up/ele histos/ele__btag_l__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ lepton_id__down fitresults/lepton_id__down/ele histos/ele__lepton_id__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ lepton_id__up fitresults/lepton_id__up/ele histos/ele__lepton_id__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ lepton_iso__down fitresults/lepton_iso__down/ele histos/ele__lepton_iso__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ lepton_iso__up fitresults/lepton_iso__up/ele histos/ele__lepton_iso__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ lepton_trigger__down fitresults/lepton_trigger__down/ele histos/ele__lepton_trigger__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ lepton_trigger__up fitresults/lepton_trigger__up/ele histos/ele__lepton_trigger__up.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ wjets_shape__down fitresults/wjets_shape__down/ele histos/ele__wjets_shape__down.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ wjets_shape__up fitresults/wjets_shape__up/ele histos/ele__wjets_shape__up.root
	
	
unfold_nominal:	
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/mu/ nominal fitresults/nominal/mu histos/mu__nominal.root
	DYLD_LIBRARY_PATH=$(UNFOLD_DIR):$(DYLD_LIBRARY_PATH) ./unfold $(DATADIR)/ele/ nominal fitresults/nominal/ele histos/ele__nominal.root

unfold_2bin_mu:
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met nominal pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape; do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(DATADIR)/fitresult \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=$$SYS \
		--output=mu_$$SYS.csv; \
	done
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(DATADIR)/fitresult \
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
		--fitResultPrefix=$(DATADIR)/fitresult \
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
		--fitResultPrefix=$(DATADIR)/fitresult \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=mu_fiterror.csv

unfold_2bin_ele:
	for SYS in btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met nominal pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape; do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(DATADIR)/fitresult \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=$$SYS \
		--output=ele_$$SYS.csv; \
	done
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(DATADIR)/fitresult \
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
		--fitResultPrefix=$(DATADIR)/fitresult \
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
		--fitResultPrefix=$(DATADIR)/fitresult \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=ele_fiterror.csv

