from ROOT import TFile, TH2D
import numpy as np
from unfolding2bins import *

import sys

#histo_file = 'histos/scanned_hists_may29/hists/0.60000/mu/merged/cos_theta_lj.root'
histo_file = 'histos/scanned_hists_may29/hists/0.60000/ele/merged/cos_theta_lj.root'
matrix_file = 'histos/scanned_hists_may29/hists/0.60000/mu/tmatrix_nocharge__gen_mu.root'

var_y = '2j1t_cos_theta_lj'

def read_fitres():
    res = {}
    with open('fitresults/nominal.txt', 'r') as f:
        for line in f:
            name, sf, unc = line.strip().split(' ')
            res[name] = [float(sf), float(unc)]

    return res

def get_events(h):
    a = h.GetBinContent(1)
    b = h.GetBinContent(2)
    return a, b

def read_dists():
    # Read distributions from histograms
    f = TFile(histo_file)
    hdata = f.Get(var_y+'__DATA')
    hdata.Rebin(24)
    a, b = get_events(hdata)
    #print a, b
    data = DataDistribution(a, b,True,0.0)

    bgdists = []
    for s in res.keys():
        if s == 'beta_signal': continue
        #print s
        h = f.Get(var_y+'__'+s)
        h.Scale(float(res[s][0]))
        h.Rebin(24)
        unc = res[s][1]/res[s][0]
        a, b = get_events(h) 
        #print a, b
        bg = DataDistribution(a, b, False, unc)
        bgdists.append(bg)

    f.Close()

    return data, bgdists

def read_tm():
    f = TFile(matrix_file)
    htm = f.Get('tm__nominal')
    htm.Rebin2D(12, 24)
    tm = np.zeros((2, 2))
    tm[0][0] = htm.GetBinContent(1, 1)
    tm[0][1] = htm.GetBinContent(1, 2)
    tm[1][0] = htm.GetBinContent(2, 1)
    tm[1][1] = htm.GetBinContent(2, 2)
    f.Close()
    return tm
 
if __name__ == '__main__':
    res = read_fitres()

    """
    # Test
    tm = read_tm()
    print tm

    signal = np.array([[550.523528636], [1533.0211081]])
    print signal

    tminv = np.linalg.inv(tm)
    print tminv
    print np.dot(tminv, signal)
   
    sys.exit(0)
    """

    """
    measured, bgdists = read_dists()

    for bg in bgdists:
        measured = CompoundDistribution(Subtraction(), measured, bg)
    """

    f = TFile(histo_file)
    hdata = f.Get(var_y+'__tchan')
    hdata.Rebin(24)
    a, b = get_events(hdata)
    measured = DataDistribution(a, b, True, 0.0)
        
    print "m1=",measured.getMean(0)," +- ",measured.getUncertainty(0)
    print "m1=",measured.getMeanSampled(0)," +- ",measured.getUncertaintySampled(0)," (toys)"
    print "m2=",measured.getMean(1)," +- ",measured.getUncertainty(1)
    print "m1=",measured.getMeanSampled(1)," +- ",measured.getUncertaintySampled(1)," (toys)"
    print "rho=",measured.getCorrelation(0,1)
    print "rho=",measured.getCorrelationSampled(0,1)," (toys)"
    print "-----------------------"
    
    #unfold the measured distribution
    f = TFile(matrix_file)
    htm = f.Get('tm__nominal')
    print htm.Integral()
    invertedResponseMatrix=readResponseFromHistogramAndInvert(htm)
    unfolded= CompoundDistribution(Unfolding(invertedResponseMatrix),measured)
    print "u1=",unfolded.getMean(0)," +- ",unfolded.getUncertainty(0)
    print "u1=",unfolded.getMeanSampled(0)," +- ",unfolded.getUncertaintySampled(0)," (toys)"
    print "u2=",unfolded.getMean(1)," +- ",unfolded.getUncertainty(1)
    print "u2=",unfolded.getMeanSampled(1)," +- ",unfolded.getUncertaintySampled(1)," (toys)"
    print "rho=",unfolded.getCorrelation(0,1)
    print "rho=",unfolded.getCorrelationSampled(0,1)," (toys)"
    print "-----------------------"
    
    #calculate asymmetry
    asymmetry=CompoundDistribution(Asymmetry(),unfolded)
    print "A=",asymmetry.getMean(0)," +- ",asymmetry.getUncertainty(0)
    print "A=",asymmetry.getMeanSampled(0)," +- ",asymmetry.getUncertaintySampled(0)," (toys)"
