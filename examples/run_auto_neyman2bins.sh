#!/bin/bash
UNFOLD_DIR=/home/mkomm/programs/tunfold17.3
LD_LIBRARY_PATH=$UNFOLD_DIR:$LD_LIBRARY_PATH



CHANNEL="mu"
METHOD="2bin"

python auto_neyman2bins.py \
--responseFile="/home/mkomm/Analysis/STpol/Jun22_final/merged/Jun22_final/merged/0.45000/"$CHANNEL"/tmatrix_nocharge__gen_ele.root" \
--responseFile="/home/mkomm/Analysis/STpol/Jun22_final/merged/Jun22_final/merged/0.45000/"$CHANNEL"/tmatrix_nocharge__gen_mu.root" \
--responseFile="/home/mkomm/Analysis/STpol/Jun22_final/merged/Jun22_final/merged/0.45000/"$CHANNEL"/tmatrix_nocharge__gen_tau.root" \
--responseMatrixName="tm__comphep__nominal" \
--comphep1000Name="tm__comphep__nominal" \
--comphep1100Name="tm__comphep_anom_unphys" \
--comphep0100Name="tm__comphep_anom_0100" \
--output="./histos/neyman__"$METHOD"__"$CHANNEL".csv" 
python plotNeyman.py "./histos/neyman__"$METHOD"__"$CHANNEL".csv" "./histos/neyman__"$METHOD"__"$CHANNEL".pdf" 
