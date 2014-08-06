#!/bin/bash
UNFOLD_DIR=/home/fynu/mkomm/stpol/unfold/tunfold17.3
LD_LIBRARY_PATH=$UNFOLD_DIR:$LD_LIBRARY_PATH

CHANNEL="mu"
python auto_neyman2bins.py \
--histFile="/home/mkomm/Analysis/STpol/scanned_hists_jul31/0.60000/"$CHANNEL"/merged/cos_theta_lj.root" \
--responseFile="/home/mkomm/Analysis/STpol/scanned_hists_jul31/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_ele.root" \
--responseFile="/home/mkomm/Analysis/STpol/scanned_hists_jul31/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_mu.root" \
--responseFile="/home/mkomm/Analysis/STpol/scanned_hists_jul31/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_tau.root" \
--responseMatrixName="tm__nominal" \
--comphep1000Name="tm__comphep__nominal" \
--comphep1100Name="tm__comphep_anom_unphys" \
--comphep0100Name="tm__comphep_anom_0100" \
--output=./histos/neyman__$CHANNEL".csv" \
-v 
python plotNeyman.py ./histos/neyman__$CHANNEL".csv" ./histos/neyman__$CHANNEL".pdf"
