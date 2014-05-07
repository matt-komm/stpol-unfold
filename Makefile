CC=g++
UNFOLD_DIR=/Users/joosep/Documents/tunfold
CXXFLAGS= -Wall -O2 -I$(UNFOLD_DIR)  `root-config --cflags --libs`
LDFLAGS=-L$(UNFOLD_DIR) -lunfold0 -lMinuit -lXMLParser

all: unfold

#unfold: unfold.cc info.hpp utils.hpp
#unfold_pseudo: unfold_pseudo.cc info.hpp utils.hpp
#unfold_systematics: unfold_systematics.cc info.hpp utils.hpp
#unfold_scan_dice: unfold_scan_dice.cc unfold.hpp utils.hpp info.hpp
unfold: unfold.cc info.hpp utils.hpp
	c++ -Wall -O2 -I/Users/joosep/Documents/tunfold  `root-config --cflags --libs`  -L/Users/joosep/Documents/tunfold -lunfold0 -lMinuit -lXMLParser unfold.cc info.hpp utils.hpp
	mv a.out unfold

unfold_pseudo: unfold_pseudo.cc info.hpp utils.hpp
	c++ -Wall -O2 -I/Users/joosep/Documents/tunfold  `root-config --cflags --libs`  -L/Users/joosep/Documents/tunfold -lunfold0 -lMinuit -lXMLParser unfold_pseudo.cc info.hpp utils.hpp
	mv a.out unfold_pseudo

calc_asymmetry: calc_asymmetry.C info.hpp utils.hpp
calc_asymmetry_syst: calc_asymmetry_syst.C info.hpp utils.hpp

clean:
	rm -f unfold unfold_pseudo calc_asymmetry unfold_systematics calc_asymmetry_syst

do_unfold_mu:
	cp ../../results/fits/apr21/bdt/mu_cov.root fitresults/cov.root
	cp ../../results/fits/apr21/bdt/mu.txt fitresults/nominal.txt
	
#	rm -f histos/src
#	ln -s ../../../results/hists/apr29/0.90000/mu/ histos/src
#	./unfold
#	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_9__mu.root

	rm -f histos/src
	ln -s ../../../results/hists/apr29/0.80000/mu/ histos/src
	./unfold
	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_8__mu.root
	cp pseudos.csv results/pseudos_mu.csv

#	rm -f histos/src
#	ln -s ../../../results/hists/apr29/0.60000/mu/ histos/src
#	./unfold
#	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_6__mu.root
#
#	rm -f histos/src
#	ln -s ../../../results/hists/apr29/0.40000/mu/ histos/src
#	./unfold
#	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_4__mu.root


do_unfold_ele:
	cp ../../results/fits/apr21/bdt/ele_cov.root fitresults/cov.root
	cp ../../results/fits/apr21/bdt/ele.txt fitresults/nominal.txt

	rm -f histos/src
	ln -s ../../../results/hists/apr29/0.80000/ele/ histos/src
	./unfold
	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_8__ele.root
	
#	rm -f histos/src
#	ln -s ../../../results/hists/apr29/0.60000/ele/ histos/src
#	./unfold
#	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_6__ele.root
#
#	rm -f histos/src
#	ln -s ../../../results/hists/apr29/0.40000/ele/ histos/src
#	./unfold
#	cp histos/unfolded.root results/csvt__qcd_mva__bdt_0_4__ele.root

do_unfold_mu_tchpt:
	cp ../../results/fits/remade_tchpt/bdt/mu_cov.root fitresults/cov.root
	cp ../../results/fits/remade_tchpt/bdt/mu.txt fitresults/nominal.txt
	rm -f histos/src
	ln -s ../../../results/hists/remade_tchpt_2/0.06000/mu/ histos/src
	./unfold
	cp histos/unfolded.root results/tchpt_0_06__mu.root
