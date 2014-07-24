#!/bin/bash
SYS="mass"
CHANNEL="mu"
python auto_unfold2bins.py \
--mc-only \
--histFile="/nfs/user/mkomm/scanned_hists_jun13/0.60000/"$CHANNEL"/merged/cos_theta_lj.root" \
--responseFile="/nfs/user/mkomm/scanned_hists_jun13/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_ele.root" \
--responseFile="/nfs/user/mkomm/scanned_hists_jun13/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_mu.root" \
--responseFile="/nfs/user/mkomm/scanned_hists_jun13/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_tau.root" \
--fitResultPrefix="/nfs/user/mkomm/scanned_hists_jun13/" \
--fitResult=$CHANNEL".txt" \
--fitCovariance=$CHANNEL"_cov.root" \
--sys=$SYS \
--output=$CHANNEL"_"$SYS".csv"
#-v

#--no-stat \
#--no-mcstat \
#--no-fiterror \
