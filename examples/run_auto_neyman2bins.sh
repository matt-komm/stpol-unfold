#!/bin/bash
UNFOLD_DIR=/home/mkomm/programs/tunfold17.3
LD_LIBRARY_PATH=$UNFOLD_DIR:$LD_LIBRARY_PATH

CHANNEL="ele"
./neymanPro \
--responseFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_ele.root" \
--responseFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_mu.root" \
--responseFile="/home/mkomm/Analysis/STpol/Oct28_reproc_mconly/merged/0.60000/"$CHANNEL"/tmatrix_nocharge__gen_tau.root" \
--responseMatrixName="tm__nominal" \
--comphep1000Name="tm__comphep__nominal" \
--comphep1100Name="tm__comphep_anom_unphys" \
--comphep0100Name="tm__comphep_anom_0100" \
--output=./histos/neyman_tunfold__$CHANNEL".csv" 
python plotNeyman.py ./histos/neyman_tunfold__$CHANNEL".csv" ./histos/neyman_tunfold__$CHANNEL".pdf" 
