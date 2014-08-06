include settings.mk

all: unfold unfoldPro

unfold: unfold.cc info.hpp utils.hpp
	$(CC) $(CXXFLAGS) $(LDFLAGS) unfold.cc info.hpp utils.hpp
	mv a.out unfold
	
unfoldPro: unfoldPro.cc info.hpp utils.hpp
	$(CC) $(CXXFLAGS) $(LDFLAGS) unfoldPro.cc info.hpp utils.hpp
	mv a.out unfoldPro

calc_asymmetry: calc_asymmetry.C info.hpp utils.hpp
calc_asymmetry_syst: calc_asymmetry_syst.C info.hpp utils.hpp

closure: closure.C info.h utils.hpp

clean:
	rm -f unfold unfold_pseudo calc_asymmetry unfold_systematics calc_asymmetry_syst unfoldPro

do_unfold_mu:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
		echo $$SYS; \
		./unfoldPro \
			--histFile=$(DATADIR)/mu/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_mu.root \
	        --responseFile=$(DATADIR)/mu/tmatrix_nocharge__gen_tau.root \
	        --fitResultPrefix=$(FITDIR) \
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
            --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
        --fitResult=mu.txt \
        --fitCovariance=mu_cov.root \
        --sys=nominal \
        --output=$(OUTPUTFOLDER)/mu__fiterror.root \
        -v \
        --no-stat \
        --no-mcstat


do_unfold_combined:
	mkdir -p $(OUTPUTFOLDER)
	for SYS in $(SYSLIST); do \
		echo $$SYS; \
		./unfoldPro \
			--histFile=$(DATADIR)/combined/merged/cos_theta_lj.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_ele.root \
			--responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_mu.root \
	        --responseFile=$(DATADIR)/combined/tmatrix_nocharge__gen_tau.root \
	        --fitResultPrefix=$(FITDIR) \
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
            --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
        --fitResult=combined.txt \
        --fitCovariance=combined_cov.root \
        --sys=nominal \
        --output=$(OUTPUTFOLDER)/combined__fiterror.root \
        -v \
        --no-stat \
        --no-mcstat



do_unfold_ele:
	mkdir -p $(OUTPUTFOLDER)
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
            --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
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
        --fitResultPrefix=$(FITDIR) \
        --fitResult=ele.txt \
        --fitCovariance=ele_cov.root \
        --sys=nominal \
        --output=$(OUTPUTFOLDER)/ele__fiterror.root \
        -v \
        --no-stat \
        --no-mcstat

comphep:
	./unfoldPro \
        --histFile=$(DATADIR)/ele/merged/cos_theta_lj.root \
        --responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_ele.root \
        --responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_mu.root \
        --responseFile=$(DATADIR)/ele/tmatrix_nocharge__gen_tau.root \
        --fitResultPrefix=$(FITDIR) \
        --fitResult=ele.txt \
        --fitCovariance=ele_cov.root \
        --sys=nominal \
		--responseMatrixName="tm__comphep" \
        --output=$(OUTPUTFOLDER)/ele__generator.root \
        -v 
	
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

