#!/bin/bash
UNFOLD_DIR=/home/mkomm/programs/tunfold17.3
LD_LIBRARY_PATH=$UNFOLD_DIR:$LD_LIBRARY_PATH

SYS="nominal"
CHANNEL="mu"
./unfoldPro \
--histFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/merged/cos_theta_lj.root" \
--responseFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_ele.root" \
--responseFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_mu.root" \
--responseFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_tau.root" \
--fitResultPrefix="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/results/bdt_reproc" \
--fitResult=$CHANNEL".txt" \
--fitCovariance=$CHANNEL"_cov.root" \
--sys=$SYS \
--output=$CHANNEL"_"$SYS".root" \
--no-stat \
--no-mcstat \
--no-fiterror 
