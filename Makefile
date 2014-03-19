CC=g++
CXXFLAGS= -Wall -O2 -g -I./tunfold  `root-config --cflags --libs`
LDFLAGS=-L./tunfold -Wl,-rpath=./tunfold -lunfold -lMinuit

all: unfold unfold_pseudo calc_asymmetry

unfold: unfold.cc binnings.h utils.hpp
unfold_pseudo: unfold_pseudo.cc binnings.h utils.hpp
unfold_scan_dice: unfold_scan_dice.cc unfold.hpp utils.hpp binnings.h

calc_asymmetry: calc_asymmetry.C binnings.h utils.hpp

clean:
	rm unfold unfold_scan_dice
