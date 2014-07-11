from ROOT import TFile
from unfolding2bins import *

histo_file = 'histos/scanned_hists_may29/hists/0.60000/mu/merged/cos_theta_lj.root'
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
    data = DataDistribution(a, b,True,0.0)

    bgdists = []
    for s in res.keys():
        if s == 'beta_signal': continue
        print s
        h = f.Get(var_y+'__'+s)
        h.Scale(float(res[s][0]))
        h.Rebin(24)
        unc = res[s][1]/res[s][0]
        a, b = get_events(h) 
        bg = DataDistribution(a, b, False, unc)
        bgdists.append(bg)

    return data, bgdists
 
if __name__ == '__main__':
    res = read_fitres()
   
    measured, bgdists = read_dists()

    for bg in bgdists:
        measured = CompoundDistribution(Subtraction(), measured, bg)
        
    print "m1=",measured.getMean(0)," +- ",measured.getUncertainty(0)
    print "m1=",measured.getMeanSampled(0)," +- ",measured.getUncertaintySampled(0)," (toys)"
    print "m2=",measured.getMean(1)," +- ",measured.getUncertainty(1)
    print "m1=",measured.getMeanSampled(1)," +- ",measured.getUncertaintySampled(1)," (toys)"
    print "rho=",measured.getCorrelation(0,1)
    print "rho=",measured.getCorrelationSampled(0,1)," (toys)"
    print "-----------------------"
    
    #unfold the measured distribution
    responseMatrix=numpy.array([[283.2637427,-32.23640403],[-92.50766353,210.2191294]])
    unfolded= CompoundDistribution(Unfolding(responseMatrix),measured)
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
