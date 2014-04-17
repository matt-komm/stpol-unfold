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
calc_asymmetry: calc_asymmetry.C info.hpp utils.hpp
calc_asymmetry_syst: calc_asymmetry_syst.C info.hpp utils.hpp

clean:
	rm -f unfold unfold_pseudo calc_asymmetry unfold_systematics calc_asymmetry_syst
