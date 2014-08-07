CC=g++
#the full path to compiled TUnfold
UNFOLD_DIR=/home/joosep/Dropbox/kbfi/top/stpol/tunfold
#UNFOLD_DIR=/home/fynu/mkomm/stpol/unfold/tunfold17.3
#UNFOLD_DIR=/Users/joosep/Documents/tunfold

#CXXFLAGS= -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs` -std=gnu++0x
#clang
CXXFLAGS= -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs` -std=c++11
LDFLAGS=-L$(UNFOLD_DIR) -lunfold -lMinuit -lXMLParser

all: unfold unfoldPro

unfoldPro: unfoldPro.cc info.hpp
	$(CC) $(CXXFLAGS) $(LDFLAGS) unfoldPro.cc info.hpp
	mv a.out unfoldPro

clean:
	rm -f unfoldPro

OUTPUTFOLDER=./histos

SYSLIST=btag_bc btag_l jer jes lepton_id lepton_iso lepton_trigger mass wzjets_matching ttjets_matching met pu tchan_scale top_weight ttjets_scale wzjets_scale wjets_shape qcd_yield qcd_antiiso ttjets wjets dyjets schan twchan lepton_weight
#SYSLIST=btag_bc


#path to histograms
DATADIR=../../results/hists/Aug5/merged/0.60000
#DATADIR=/nfs/user/mkomm/scanned_hists_jul31/0.60000
#DATADIR=../../notes/notes/AN-14-001/trunk/data/hists_for_fit_unfolding/0.60000
LD_LIBRARY_PATH:=$(UNFOLD_DIR):$(LD_LIBRARY_PATH)

test:
	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--output=test.root\
		-v

do_unfold_mu:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
		echo $$SYS; \
		./unfoldPro \
			--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=fitresults \
			--fitResult=mu.txt \
			--fitCovariance=mu_cov.root \
			--sys=$$(echo $$SYS)__up \
			--output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS)__up.root \
			-v \
			--no-stat \
			--no-mcstat \
			--no-fiterror; \
		./unfoldPro \
			--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=fitresults \
			--fitResult=mu.txt \
			--fitCovariance=mu_cov.root \
			--sys=$$(echo $$SYS)__down \
			--output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS)__down.root \
			-v \
			--no-stat \
			--no-mcstat \
			--no-fiterror; \
	done

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/mu__nominal.root \
		-v

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--responseMatrixName="tm__comphep" \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/mu__generator.root \
		-v

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/mu__stat.root \
		-v \
		--no-mcstat \
		--no-fiterror

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/mu__mcstat.root \
		-v \
		--no-stat \
		--no-fiterror

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/mu__fiterror.root \
		-v \
		--no-stat \
		--no-mcstat

	julia unfold.jl mu

do_unfold_combined:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
		echo $$SYS; \
		./unfoldPro \
			--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=fitresults \
			--fitResult=combined.txt \
			--fitCovariance=combined_cov.root \
			--sys=$$(echo $$SYS)__up \
			--output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS)__up.root \
			-v \
			--no-stat \
			--no-mcstat \
			--no-fiterror; \
		./unfoldPro \
			--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=fitresults \
			--fitResult=combined.txt \
			--fitCovariance=combined_cov.root \
			--sys=$$(echo $$SYS)__down \
			--output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS)__down.root \
			-v \
			--no-stat \
			--no-mcstat \
			--no-fiterror; \
	done

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/combined__nominal.root \
		-v

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--responseMatrixName="tm__comphep" \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/combined__nominal.root \
		-v

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/combined__stat.root \
		-v \
		--no-mcstat \
		--no-fiterror

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/combined__mcstat.root \
		-v \
		--no-stat \
		--no-fiterror

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/combined__fiterror.root \
		-v \
		--no-stat \
		--no-mcstat
	julia unfold.jl combined

do_unfold_ele:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
		./unfoldPro \
			--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=fitresults \
			--fitResult=ele.txt \
			--fitCovariance=ele_cov.root \
			--sys=$$(echo $$SYS)__up \
			--output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS)__up.root \
			-v \
			--no-stat \
			--no-mcstat \
			--no-fiterror; \
	   ./unfoldPro \
			--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=fitresults \
			--fitResult=ele.txt \
			--fitCovariance=ele_cov.root \
			--sys=$$(echo $$SYS)__down \
			--output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS)__down.root \
			-v \
			--no-stat \
			--no-mcstat \
			--no-fiterror; \
	done

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/ele__nominal.root \
		-v

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--responseMatrixName="tm__comphep" \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/ele__nominal.root \
		-v

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/ele__stat.root \
		-v \
		--no-mcstat \
		--no-fiterror

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/ele__mcstat.root \
		-v \
		--no-stat \
		--no-fiterror

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--output=$(OUTPUTFOLDER)/ele__fiterror.root \
		-v \
		--no-stat \
		--no-mcstat
	julia unfold.jl ele

unfold_2bin_mu:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=$$(echo $$SYS) \
		--output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS).csv; \
	done

	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--responseMatrixName="tm__comphep" \
		--output=$(OUTPUTFOLDER)/mu__generator.csv

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
		--output=$(OUTPUTFOLDER)/mu__stat.csv

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
		--output=$(OUTPUTFOLDER)/mu__mcstat.csv

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
		--output=$(OUTPUTFOLDER)/mu__fiterror.csv
	sed -i .bak "s/nominal/generator/" histos/mu__generator.csv
	#julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_mu_2bin.csv histos/mu_*.csv

unfold_2bin_ele:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=$$(echo $$SYS) \
		--output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS).csv; \
	done

	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--responseMatrixName="tm__comphep" \
		--output=$(OUTPUTFOLDER)/ele__generator.csv

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
		--output=$(OUTPUTFOLDER)/ele__stat.csv

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
		--output=$(OUTPUTFOLDER)/ele__mcstat.csv

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
		--output=$(OUTPUTFOLDER)/ele__fiterror.csv
	sed -i .bak "s/nominal/generator/" histos/ele__generator.csv
	julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_ele_2bin.csv histos/ele_*.csv

unfold_2bin_combined:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=$$(echo $$SYS) \
		--output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS).csv; \
	done

	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=fitresults \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--responseMatrixName="tm__comphep" \
		--output=$(OUTPUTFOLDER)/combined__generator.csv

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
		--output=$(OUTPUTFOLDER)/combined__stat.csv

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
		--output=$(OUTPUTFOLDER)/combined__mcstat.csv

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
		--output=$(OUTPUTFOLDER)/combined__fiterror.csv
	sed -i .bak "s/nominal/generator/" histos/combined__generator.csv
	julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_combined_2bin.csv histos/combined_*.csv

unfold_all: do_unfold_mu do_unfold_ele unfold_2bin_mu unfold_2bin_ele do_unfold_combined unfold_2bin_combined
