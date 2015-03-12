include settings.mk

all: unfoldPro neymanPro collectUnfoldProResult

unfoldPro: unfoldPro.cc logging.hpp loadHistogram.hpp loadFitResult.hpp scanTau.hpp
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o unfoldPro unfoldPro.cc
	
neymanPro: neymanPro.cc logging.hpp loadHistogram.hpp asymmetryCalculation.hpp
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o neymanPro neymanPro.cc
	
collectUnfoldProResult: collectUnfoldProResult.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o collectUnfoldProResult collectUnfoldProResult.cc
    

clean:
	rm -f unfoldPro neymanPro collectUnfoldProResult

test:
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=generator \
		--responseMatrixName="tm__aMCatNLO" \
		--output=$(OUTPUTFOLDER)/combined__generator.csv
	    
do_unfold_mu:
	mkdir -p $(OUTPUTFOLDER)
	
	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--no-mcstat \
		--no-fiterror \
		--output=$(OUTPUTFOLDER)/mu__nominal 
		
	for SYS in $(SYSLIST); do \
		./unfoldPro \
			--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=$(FITDIR) \
			--fitResult=mu.txt \
			--fitCovariance=mu_cov.root \
			--sys=$$(echo $$SYS)__up \
			--output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS)__up \
			--no-mcstat \
		    --no-fiterror \
			$(REGMODE); \
		./unfoldPro \
			--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=$(FITDIR) \
			--fitResult=mu.txt \
			--fitCovariance=mu_cov.root \
			--sys=$$(echo $$SYS)__down \
			--output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS)__down \
			--no-mcstat \
		    --no-fiterror \
			$(REGMODE); \
		./collectUnfoldProResult \
	        --nominal=$(OUTPUTFOLDER)/mu__nominal.root \
	        --down=$(OUTPUTFOLDER)/mu__$$(echo $$SYS)__down.root \
	        --up=$(OUTPUTFOLDER)/mu__$$(echo $$SYS)__up.root \
	        --output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS).csv \
	        --sys=$$(echo $$SYS); \
	done

	
		

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--responseMatrixName="tm__aMCatNLO" \
		--sys=nominal \
		$(REGMODE) \
		--no-mcstat \
		--no-fiterror \
		--output=$(OUTPUTFOLDER)/mu__generator 
	./collectUnfoldProResult \
        --nominal=$(OUTPUTFOLDER)/mu__nominal.root \
        --down=$(OUTPUTFOLDER)/mu__generator.root \
        --up=$(OUTPUTFOLDER)/mu__generator.root \
        --output=$(OUTPUTFOLDER)/mu__generator.csv \
        --sys=generator
	

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/mu__stat \
		--no-mcstat \
		--no-fiterror
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/mu__stat.root \
	    --output=$(OUTPUTFOLDER)/mu__stat.csv \
	    --sys=stat \
	    --fromError

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/mu__mcstat \
		--no-stat \
		--no-fiterror 
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/mu__mcstat.root \
	    --output=$(OUTPUTFOLDER)/mu__mcstat.csv \
	    --sys=mcstat \
	    --fromError

	./unfoldPro \
		--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/mu__fiterror \
		--no-stat \
		--no-mcstat
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/mu__fiterror.root \
	    --output=$(OUTPUTFOLDER)/mu__fiterror.csv \
	    --sys=fiterror \
	    --fromError

	#julia unfold.jl mu

do_unfold_combined:
	mkdir -p $(OUTPUTFOLDER)
	
	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--no-mcstat \
		--no-fiterror \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/combined__nominal 
	
	for SYS in $(SYSLIST); do \
		./unfoldPro \
			--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=$(FITDIR) \
			--fitResult=combined.txt \
			--fitCovariance=combined_cov.root \
			--sys=$$(echo $$SYS)__up \
			$(REGMODE) \
			--no-mcstat \
		    --no-fiterror \
			--output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS)__up; \
		./unfoldPro \
			--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=$(FITDIR) \
			--fitResult=combined.txt \
			--fitCovariance=combined_cov.root \
			--sys=$$(echo $$SYS)__down \
			$(REGMODE) \
			--no-mcstat \
		    --no-fiterror \
			--output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS)__down; \
		./collectUnfoldProResult \
	        --nominal=$(OUTPUTFOLDER)/combined__nominal.root \
	        --down=$(OUTPUTFOLDER)/combined__$$(echo $$SYS)__down.root \
	        --up=$(OUTPUTFOLDER)/combined__$$(echo $$SYS)__up.root \
	        --output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS).csv \
	        --sys=$$(echo $$SYS); \
	done

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--responseMatrixName="tm__aMCatNLO" \
		--sys=nominal \
		--no-mcstat \
		--no-fiterror \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/combined__generator 
	./collectUnfoldProResult \
        --nominal=$(OUTPUTFOLDER)/combined__nominal.root \
        --down=$(OUTPUTFOLDER)/combined__generator.root \
        --up=$(OUTPUTFOLDER)/combined__generator.root \
        --output=$(OUTPUTFOLDER)/combined__generator.csv \
        --sys=generator; \

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/combined__stat \
		--no-mcstat \
		--no-fiterror
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/combined__stat.root \
	    --output=$(OUTPUTFOLDER)/combined__stat.csv \
	    --sys=stat \
	    --fromError

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/combined__mcstat \
		--no-stat \
		--no-fiterror
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/combined__mcstat.root \
	    --output=$(OUTPUTFOLDER)/combined__mcstat.csv \
	    --sys=mcstat \
	    --fromError

	./unfoldPro \
		--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/combined__fiterror \
		--no-stat \
		--no-mcstat
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/combined__fiterror.root \
	    --output=$(OUTPUTFOLDER)/combined__fiterror.csv \
	    --sys=fiterror \
	    --fromError
	#julia unfold.jl combined

do_unfold_ele:
	mkdir -p $(OUTPUTFOLDER)
	
	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
	    --no-mcstat \
		--no-fiterror \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/ele__nominal 
	
	for SYS in $(SYSLIST); do \
		./unfoldPro \
			--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=$(FITDIR) \
			--fitResult=ele.txt \
			--fitCovariance=ele_cov.root \
			--sys=$$(echo $$SYS)__up \
			$(REGMODE) \
			--no-mcstat \
	        --no-fiterror \
			--output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS)__up; \
	   ./unfoldPro \
			--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
			--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
			--fitResultPrefix=$(FITDIR) \
			--fitResult=ele.txt \
			--fitCovariance=ele_cov.root \
			--sys=$$(echo $$SYS)__down \
			$(REGMODE) \
			--no-mcstat \
		    --no-fiterror \
			--output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS)__down; \
		./collectUnfoldProResult \
	        --nominal=$(OUTPUTFOLDER)/ele__nominal.root \
	        --down=$(OUTPUTFOLDER)/ele__$$(echo $$SYS)__down.root \
	        --up=$(OUTPUTFOLDER)/ele__$$(echo $$SYS)__up.root \
	        --output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS).csv \
	        --sys=$$(echo $$SYS); \
	done



	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--responseMatrixName="tm__aMCatNLO" \
		--sys=nominal \
		--no-mcstat \
		--no-fiterror \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/ele__generator 
	./collectUnfoldProResult \
        --nominal=$(OUTPUTFOLDER)/ele__nominal.root \
        --down=$(OUTPUTFOLDER)/ele__generator.root \
        --up=$(OUTPUTFOLDER)/ele__generator.root \
        --output=$(OUTPUTFOLDER)/ele__generator.csv \
        --sys=generator \

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/ele__stat \
		--no-mcstat \
		--no-fiterror
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/ele__stat.root \
	    --output=$(OUTPUTFOLDER)/ele__stat.csv \
	    --sys=stat \
	    --fromError

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/ele__mcstat \
		--no-stat \
		--no-fiterror
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/ele__mcstat.root \
	    --output=$(OUTPUTFOLDER)/ele__mcstat.csv \
	    --sys=mcstat \
	    --fromError

	./unfoldPro \
		--histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		$(REGMODE) \
		--output=$(OUTPUTFOLDER)/ele__fiterror \
		--no-stat \
		--no-mcstat
	./collectUnfoldProResult \
	    --nominal=$(OUTPUTFOLDER)/ele__fiterror.root \
	    --output=$(OUTPUTFOLDER)/ele__fiterror.csv \
	    --sys=fiterror \
	    --fromError
	#julia unfold.jl ele

unfold_2bin_mu:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=$$(echo $$SYS) \
		--output=$(OUTPUTFOLDER)/mu__$$(echo $$SYS).csv; \
	done

	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=generator \
		--responseMatrixName="tm__aMCatNLO" \
		--overrideOutputSysName="generator" \
		--output=$(OUTPUTFOLDER)/mu__generator.csv

	python auto_unfold2bins.py --histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
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
		--fitResultPrefix=$(FITDIR) \
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
		--fitResultPrefix=$(FITDIR) \
		--fitResult=mu.txt \
		--fitCovariance=mu_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=$(OUTPUTFOLDER)/mu__fiterror.csv
	#sed -i .bak "s/nominal/generator/" histos/mu__generator.csv
	#julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_mu_2bin.csv histos/mu_*.csv

unfold_2bin_ele:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=$$(echo $$SYS) \
		--output=$(OUTPUTFOLDER)/ele__$$(echo $$SYS).csv; \
	done

	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=generator \
		--responseMatrixName="tm__aMCatNLO" \
		--overrideOutputSysName="generator" \
		--output=$(OUTPUTFOLDER)/ele__generator.csv

	python auto_unfold2bins.py --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
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
		--fitResultPrefix=$(FITDIR) \
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
		--fitResultPrefix=$(FITDIR) \
		--fitResult=ele.txt \
		--fitCovariance=ele_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=$(OUTPUTFOLDER)/ele__fiterror.csv
	#sed -i .bak "s/nominal/generator/" histos/ele__generator.csv
	#julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_ele_2bin.csv histos/ele_*.csv

unfold_2bin_combined:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=$$(echo $$SYS) \
		--output=$(OUTPUTFOLDER)/combined__$$(echo $$SYS).csv; \
	done

	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=generator \
		--responseMatrixName="tm__aMCatNLO" \
		--output=$(OUTPUTFOLDER)/combined__generator.csv

	python auto_unfold2bins.py --histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
		--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
		--fitResultPrefix=$(FITDIR) \
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
		--fitResultPrefix=$(FITDIR) \
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
		--fitResultPrefix=$(FITDIR) \
		--fitResult=combined.txt \
		--fitCovariance=combined_cov.root \
		--sys=nominal \
		--no-stat \
		--no-mcstat \
		--output=$(OUTPUTFOLDER)/combined__fiterror.csv
	#sed -i .bak "s/nominal/generator/" histos/combined__generator.csv
	#julia ../analysis/merge_csv.jl ../../results/tables/asymmetries_combined_2bin.csv histos/combined_*.csv

unfold_all: do_unfold_mu do_unfold_ele unfold_2bin_mu unfold_2bin_ele do_unfold_combined unfold_2bin_combined
