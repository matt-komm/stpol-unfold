#!/usr/bin/python
import ROOT
import numpy
import math
import os
import os.path
import sys
from optparse import OptionParser
import unfolding2bins as u2b

verbose=False

rootObj=[]


def readHist1d(fileNames,histName,sys,scale=1.0,rebin=1):
    resulthist=None
    for fileName in fileNames:
        f=ROOT.TFile(fileName,"r")
        if sys!="nominal":
            if (f.FindKey(histName+"__"+sys)):
                hist = f.Get(histName+"__"+sys)
                if (verbose):
                    print "opening ",fileName
                    print " ... found ",histName+"__"+sys," with entries=",hist.GetEntries()
                    print " ... rebin with factor ",hist.GetNbinsX()/rebin
                    print " ... scale with factor ",scale
                    print
                hist.Rebin(hist.GetNbinsX()/rebin)
                    
                hist.Scale(scale)
                    
                if (resulthist==None):
                    resulthist=hist
                    resulthist.SetDirectory(0)
                    rootObj.append(resulthist)
                else:
                    resulthist.Add(hist)
            else:
                if (f.FindKey(histName)):
                    hist = f.Get(histName)
                    print "WARNING: opening ",fileName
                    print " ... WARNING using ",histName," as fallback for ",histName+"__"+sys
                    if (verbose):
                        print " ... rebin with factor ",hist.GetNbinsX()/rebin
                        print " ... scale with factor ",scale
                    print
                    hist.Rebin(hist.GetNbinsX()/rebin)
                        
                    hist.Scale(scale)
                        
                    if (resulthist==None):
                        resulthist=hist
                        resulthist.SetDirectory(0)
                        rootObj.append(resulthist)
                    else:
                        resulthist.Add(hist)
        else:
            if (f.FindKey(histName)):
                hist = f.Get(histName)
                if (verbose):
                    print "opening ",fileName
                    print " ... found ",histName," with entries=",hist.GetEntries()
                    print " ... rebin with factor ",hist.GetNbinsX()/rebin
                    print " ... scale with factor ",scale
                    print
                hist.Rebin(hist.GetNbinsX()/rebin)
                    
                hist.Scale(scale)
                    
                if (resulthist==None):
                    resulthist=hist
                    resulthist.SetDirectory(0)
                    rootObj.append(resulthist)
                else:
                    resulthist.Add(hist)
        f.Close()
    if (resulthist==None):    
        raise Exception("hist '"+histName+"' not found in the given rootfiles")
    return resulthist
    
def readHist2d(fileNames,histName,sys,scale=1.0,rebinX=1,rebinY=1):
    resulthist=None
    for fileName in fileNames:
        f=ROOT.TFile(fileName,"r")
        if (f.FindKey(histName+"__"+sys)):
            hist = f.Get(histName+"__"+sys)
            if (verbose):
                print "opening ",fileName
                print " ... found ",histName+"__"+sys," with entries=",hist.GetEntries()
                print " ... rebin with factor ",hist.GetNbinsX()/rebinX,"x",hist.GetNbinsY()/rebinY
                print " ... scale with factor ",scale
                print
            hist.Rebin2D(hist.GetNbinsX()/rebinX,hist.GetNbinsY()/rebinY)
            hist.Scale(scale)
            
            if (resulthist==None):
                resulthist=hist
                resulthist.SetDirectory(0)
                rootObj.append(resulthist)
            else:
                resulthist.Add(hist)
        f.Close()
    if (resulthist==None):    
        raise Exception("hist '"+histName+"__"+sys+"' not found in the given rootfiles")
    return resulthist
    
def readFitResult(fitResult,fitCovariance):
    fitDict={}
    f=open(fitResult,"r")
    try:
        for line in f:
            lineSplit = line.split(" ")
            if lineSplit[0]=="beta_signal":
                fitDict["tchan"]={"scale":float(lineSplit[1]),"uncertainty":float(lineSplit[2])}
            else:
                fitDict[lineSplit[0]]={"scale":float(lineSplit[1]),"uncertainty":float(lineSplit[2])}
    except Exception,e:
        raise Exception("Error during parsing of fit result: "+fitResult)
    f.close()
    
    covFile = ROOT.TFile(fitCovariance,"R")
    covHist = covFile.Get("covariance")
    covMatrix = ROOT.TMatrixD(len(fitDict.keys())-2,len(fitDict.keys())-2)
    for ibin in range(len(fitDict.keys())-2):
        for jbin in range(len(fitDict.keys())-2):
            covMatrix[ibin][jbin]=covHist.GetBinContent(ibin+2,jbin+2) #skip bin 1 = tchan, QCD not included in covariance hist
    
    eigenValues = ROOT.TVectorD(len(fitDict.keys())-2)
    eigenVectors = covMatrix.EigenVectors(eigenValues)
    
    return fitDict

if __name__=="__main__":
    DATA=["DATA"]
    SIGNAL="tchan"
    BACKGROUND=["ttjets","wzjets","qcd"]
    HISTPREFIX="2j1t_cos_theta_lj__"
    TMPREFIX="tm"
    REBIN_RECO=2
    REBIN_GEN=2
    FITRESULTFILE="mu.txt"
    FITCOVFILE="mu_cov.root"
    
    parser = OptionParser()
    parser.add_option("-v", "--verbose",action="store_true", dest="verbose", default=False, help="Verbose output")
    parser.add_option("--mc-only",action="store_true",dest="mc_only",default=False,help="Substitude data with MC")
    parser.add_option("--histFile",action="append",dest="histFiles",default=[],help="The rootfile containing the signal/background histograms for unfolding")
    parser.add_option("--sys",action="store",dest="systematic",default="nominal",help="Systematic shift.")
    parser.add_option("--responseFile",action="append",dest="responseFiles",default=[],help="The <rootfile>:<histpath> containing the 2d response matrix.")
    parser.add_option("--fitResultPrefix",action="store",dest="fitResultPrefix",default="",help="Path prefix for the fit result")
    parser.add_option("--output",action="store",dest="output",default="",help="The output file")
    (options, args) = parser.parse_args()
    verbose=options.verbose
    
    if (verbose):
        print "histFiles: "
        print "".join(" ... "+histFile+"\n" for histFile in options.histFiles)
        print "responseFiles: "
        print "".join(" ... "+responseFile+"\n" for responseFile in options.responseFiles)
        print "systematic:"
        print " ... "+options.systematic
        print
        print "fit: "
        print " ... scale: "+os.path.join(options.fitResultPrefix,options.systematic,FITRESULTFILE)
        print " ... covaraince: "+os.path.join(options.fitResultPrefix,options.systematic,FITCOVFILE)
        print
        
    '''
    #h = readHist1d(options.histFiles,HISTPREFIX+DATA[0],1,REBIN_RECO)
    h = readHist2d(options.responseFiles,TMPREFIX+"nominal",1,REBIN_GEN,REBIN_RECO)
    cv = ROOT.TCanvas("cv","",800,600)
    h.Draw("colz")
    cv.Update()
    cv.WaitPrimitive()
    '''
    
    
    fitResult = readFitResult(os.path.join(options.fitResultPrefix,options.systematic,FITRESULTFILE),os.path.join(options.fitResultPrefix,options.systematic,FITCOVFILE))
    
    
    
    measured = None
    if options.mc_only:
        signalHist=readHist1d(options.histFiles,HISTPREFIX+SIGNAL,options.systematic,fitResult[SIGNAL]["scale"],REBIN_RECO)
        for background in BACKGROUND:
            backgroundHist=readHist1d(options.histFiles,HISTPREFIX+background,options.systematic,fitResult[background]["scale"],REBIN_RECO)
            signalHist.Add(backgroundHist)
        measured = u2b.DataDistribution.createFromHistogram(signalHist,True,False,0.0)
    else:
        measured = u2b.DataDistribution.createFromHistogram(readHist1d(options.histFiles,HISTPREFIX+DATA[0],"nominal",1,REBIN_RECO),True,False,0.0)
        
    
    

    #TODO: decorrelate before subtracting - or define correlation between bgs

    for background in BACKGROUND:
        backgroundHist=readHist1d(options.histFiles,HISTPREFIX+background,options.systematic,fitResult[background]["scale"],REBIN_RECO)
        backgroundDist = u2b.DataDistribution.createFromHistogram(backgroundHist,False,True,fitResult[background]["uncertainty"])
        measured = u2b.CompoundDistribution(u2b.Subtraction(),measured,backgroundDist)
        
    tmHist = readHist2d(options.responseFiles,TMPREFIX,options.systematic,fitResult[SIGNAL]["scale"],REBIN_GEN,REBIN_RECO)
    tmHistinverted=u2b.readResponseFromHistogramAndInvert(tmHist)
    
    if verbose:
        print "measured after bg subtraction:"
        print " ... m1=",measured.getMean(0)," +- ",measured.getUncertainty(0)
        print " ... m1=",measured.getMeanSampled(0)," +- ",measured.getUncertaintySampled(0)," (toys)"
        print " ... m2=",measured.getMean(1)," +- ",measured.getUncertainty(1)
        print " ... m1=",measured.getMeanSampled(1)," +- ",measured.getUncertaintySampled(1)," (toys)"
        print " ... rho=",measured.getCorrelation(0,1)
        print " ... rho=",measured.getCorrelationSampled(0,1)," (toys)"
        print

    unfolded= u2b.CompoundDistribution(u2b.Unfolding(tmHistinverted),measured)
    
    if verbose:
        print "unfolded distribution:"
        print " ... u1=",unfolded.getMean(0)," +- ",unfolded.getUncertainty(0)
        print " ... u1=",unfolded.getMeanSampled(0)," +- ",unfolded.getUncertaintySampled(0)," (toys)"
        print " ... u2=",unfolded.getMean(1)," +- ",unfolded.getUncertainty(1)
        print " ... u2=",unfolded.getMeanSampled(1)," +- ",unfolded.getUncertaintySampled(1)," (toys)"
        print " ... rho=",unfolded.getCorrelation(0,1)
        print " ... rho=",unfolded.getCorrelationSampled(0,1)," (toys)"
        print
        #calculate asymmetry
    asymmetry=u2b.CompoundDistribution(u2b.Asymmetry(),unfolded)
    
    if verbose:
        print "final result:"
        print " ... A=",asymmetry.getMean(0)," +- ",asymmetry.getUncertainty(0)
    
