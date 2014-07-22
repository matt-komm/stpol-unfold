#!/bin/bash
python auto_unfold2bins.py \
-v \
--histFile=/nfs/user/mkomm/scanned_hists_jun13/0.60000/mu/merged/cos_theta_lj.root \
--responseFile=/nfs/user/mkomm/scanned_hists_jun13/0.60000/mu/tmatrix_nocharge__gen_ele.root \
--responseFile=/nfs/user/mkomm/scanned_hists_jun13/0.60000/mu/tmatrix_nocharge__gen_mu.root \
--responseFile=/nfs/user/mkomm/scanned_hists_jun13/0.60000/mu/tmatrix_nocharge__gen_tau.root \
--fitResultPrefix=/nfs/user/mkomm/scanned_hists_jun13 \
--sys=btag_bc__down 
