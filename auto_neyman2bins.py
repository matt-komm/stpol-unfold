#!/usr/bin/python
import ROOT
import numpy
import math
import os
import os.path
import sys
import csv
from optparse import OptionParser
import unfolding2bins as u2b
from auto_unfold2bins import *


DATA=["DATA"]
SIGNAL="tchan"
BACKGROUND=["ttjets","wzjets","qcd"]
HISTPREFIX="2j1t_cos_theta_lj__"
TMPREFIX="tm"
REBIN_RECO=2
REBIN_GEN=2

verbose=False

rootObj=[]

def generateWeightedHist(comphep1000,comphep1100,comphep0100,vl,vr):
    gammaTop=1.49
    w1000=gammaTop
    wart=gammaTop
    w0100=gammaTop
    wtot=gammaTop*(vl**2+vr**2)
    m=vl**4*w1000/wtot
    n=vl**2*vr**2*wart/wtot
    k=vr**4*w0100/wtot
    result=ROOT.TH1D(comphep1000)
    rootObj.append(result)
    result.Scale(0.0)
    result.Add(comphep1000,m)
    result.Add(comphep1100,n)
    result.Add(comphep0100,k)
    return result
    
def calcAsymmetry(hist):
    sumUp=0.0
    sumDown=0.0
    for ibin in range(hist.GetNbinsX()):
        if hist.GetBinCenter(ibin+1)>0.0:
            sumUp+=hist.GetBinContent(ibin+1)
        else:
            sumDown+=hist.GetBinContent(ibin+1)
    return (sumUp-sumDown)/(sumUp+sumDown)
    
def doNeyman(responseMatrix,comphep1000Matrix,comphep1100Matrix,comphep0100Matrix,N):
    result=[]
    comphep1000Gen=comphep1000Matrix.ProjectionX("comphep1000Gen")
    comphep1100Gen=comphep1100Matrix.ProjectionX("comphep1100Gen")
    comphep0100Gen=comphep0100Matrix.ProjectionX("comphep0100Gen")
    
    comphep1000Reco=comphep1000Matrix.ProjectionY("comphep1000Reco")
    comphep1100Reco=comphep1100Matrix.ProjectionY("comphep1100Reco")
    comphep0100Reco=comphep0100Matrix.ProjectionY("comphep0100Reco")
    '''
    cv=ROOT.TCanvas("cv","",800,600)
    cv.Divide(1,2)
    cv.cd(1)
    comphep1000Gen.Draw()
    comphep1100Gen.Draw("Same")
    comphep0100Gen.Draw("Same")
    cv.cd(2)
    comphep1000Reco.Draw()
    comphep1100Reco.Draw("Same")
    comphep0100Reco.Draw("Same")
    cv.Update()
    cv.WaitPrimitive()
    '''
    tmHistinverted=u2b.readResponseFromHistogramAndInvert(responseMatrix)
    
    for i in range(N):
        vl=1.0
        vr=1.0*i/(N-1)
        
        genDist=generateWeightedHist(comphep1000Gen,comphep1100Gen,comphep0100Gen,vl,vr)
        recoDist=generateWeightedHist(comphep1000Reco,comphep1100Reco,comphep0100Reco,vl,vr)
        
        measured = u2b.DataDistribution.createFromHistogram(recoDist,True,False,0.0)
        unfolded= u2b.CompoundDistribution(u2b.Unfolding(tmHistinverted),measured)
        asymmetry=u2b.CompoundDistribution(u2b.Asymmetry(),unfolded)
        
        result.append({
            "vl":vl,
            "vr":vr,
            "generated":calcAsymmetry(genDist),
            "unfolded":asymmetry.getMean(0),
            "uncertainty":asymmetry.getUncertainty(0),
            "bias":math.fabs(calcAsymmetry(genDist)-asymmetry.getMean(0))
        })
    return result
    
if __name__=="__main__":
    
    parser = OptionParser()
    parser.add_option("-v", "--verbose",action="store_true", dest="verbose", default=False, help="Verbose output")
    parser.add_option("--responseFile",action="append",dest="responseFiles",default=[],help="The <rootfile> containing the 2d response matrix.")
    parser.add_option("--responseMatrixName",action="store",dest="responseMatrixName",default="tm",help="The<histname> containing the 2d response matrix.")
    
    parser.add_option("--comphep1000Name",action="store",dest="comphep1000Name",default=None,help="name of the comphep1000 response matrix")
    parser.add_option("--comphep1100Name",action="store",dest="comphep1100Name",default=None,help="name of the comphep1100 response matrix")
    parser.add_option("--comphep0100Name",action="store",dest="comphep0100Name",default=None,help="name of the comphep0100 response matrix")
    parser.add_option("-N",action="store",dest="N",default=11,help="Number of variations")
    parser.add_option("--output",action="store",dest="output",default="out.csv",help="The output file")
    (options, args) = parser.parse_args()
    verbose=options.verbose
    
    
    if (verbose):
        print "responseFiles: "
        print "".join(" ... "+responseFile+"\n" for responseFile in options.responseFiles)
        print "responseMatrixName:"
        print " ... "+options.responseMatrixName
        print
        print "output:"
        print " ... "+options.output
        print
        
    
    responseHist = readHist2d(options.responseFiles,options.responseMatrixName,"",1.0,REBIN_GEN,REBIN_RECO)
    
    comphep1000Matrix = readHist2d(options.responseFiles,options.comphep1000Name,"",1.0,REBIN_GEN,REBIN_RECO)
    comphep1100Matrix = readHist2d(options.responseFiles,options.comphep1100Name,"",1.0,REBIN_GEN,REBIN_RECO)
    comphep0100Matrix = readHist2d(options.responseFiles,options.comphep0100Name,"",1.0,REBIN_GEN,REBIN_RECO)
    
    scanResult=doNeyman(responseHist,comphep1000Matrix,comphep1100Matrix,comphep0100Matrix,int(options.N))
    #print scanResult
    '''
    scanResult=[]
    for i in range(11):
        scanResult.append({
            "vl":1.0,
            "vr":i/10.0,
            "generated":0.5-0.03*i,
            "unfolded":0.5-0.04*i,
            "uncertainty":0.02+0.001*i,
        })
    '''
    outputFile = open(options.output, 'wb')
    writer = csv.DictWriter(outputFile, ["vl","vr","generated","unfolded","uncertainty","bias"], restval='NAN', extrasaction='raise', dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
    writer.writeheader()
    for result in scanResult:
        writer.writerow(result)
    outputFile.close()
    
    
