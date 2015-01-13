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

DATA=["DATA"]
SIGNAL="tchan"
BACKGROUND=["ttjets","wzjets","qcd"]
HISTPREFIX="2j1t_cos_theta_lj__"
TMPREFIX="tm"
REBIN_RECO=2
REBIN_GEN=2

verbose=False

rootObj=[]


def readHist1d(fileNames,histName,sys,scale=1.0,rebin=1):
    resulthist=None
    for fileName in fileNames:
        f=ROOT.TFile(fileName,"r")
        if (verbose):
            print "opening ",fileName
        hist=None
        if ((sys=="" or sys=="nominal") and f.FindKey(histName)):
            hist = f.Get(histName)
        elif (f.FindKey(histName+"__"+sys)):
            hist = f.Get(histName+"__"+sys)
        elif (f.FindKey(histName)):
            hist = f.Get(histName)
            print " ... WARNING using ",histName," as fallback for ",histName+"__"+sys
        else:
            continue
            
        if (verbose):
            print " ... found ",histName+"__"+sys," with entries=",hist.GetEntries()
            print " ... rebin with factor ",hist.GetNbinsX()/rebinX,"x",hist.GetNbinsY()/rebinY
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
        raise Exception("hist '"+histName+"' with sys '"+sys+"' not found in the given rootfiles")
    return resulthist

def readHist2d(fileNames,histName,sys,scale=1.0,rebinX=1,rebinY=1):
    resulthist=None
    for fileName in fileNames:
        f=ROOT.TFile(fileName,"r")
        if (verbose):
            print "opening ",fileName
        hist=None
        if (sys=="" and f.FindKey(histName)):
            hist = f.Get(histName)
        elif (f.FindKey(histName+"__"+sys)):
            hist = f.Get(histName+"__"+sys)
        elif (f.FindKey(histName+"__nominal")):
            hist = f.Get(histName+"__nominal")
            print " ... WARNING using ",histName+"__nominal"," as fallback for ",histName+"__"+sys
        else:
            continue
            
        if (verbose):
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
        
        raise Exception("hist '"+histName+"' with sys '"+sys+"' not found in the given rootfiles")
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

def doUnfolding(histFiles,signalHistName,backgroundHistNames,dataHistNames,responseFiles,responseHistName,fitResult,systematic="nominal",useStatUnc=True,useMCStatUnc=True,useFitUnc=True):


    measured = None
    if options.mc_only:
        #fake data by adding up all nominal histograms
        fakeData=None
        for histName in fitResult.keys():
            if histName==signalHistName or histName in backgroundHistNames:
                if fakeData==None:
                    fakeData=readHist1d(histFiles,HISTPREFIX+histName,"nominal",fitResult[histName]["scale"],REBIN_RECO)
                else:
                    fakeData.Add(readHist1d(histFiles,HISTPREFIX+histName,"nominal",fitResult[histName]["scale"],REBIN_RECO))

        measured = u2b.DataDistribution.createFromHistogram(fakeData,useStatUnc,False,0.0)
    else:
        #read data histograms
        data=None
        for histName in dataHistNames:
            if data==None:
                data = readHist1d(histFiles,HISTPREFIX+histName,"nominal",1,REBIN_RECO)
        measured = u2b.DataDistribution.createFromHistogram(data,useStatUnc,False,0.0)




    for histName in fitResult.keys():
        if histName in backgroundHistNames:
            backgroundHist=readHist1d(histFiles,HISTPREFIX+histName,systematic,fitResult[histName]["scale"],REBIN_RECO)
            backgroundDist = u2b.DataDistribution.createFromHistogram(backgroundHist,False,useMCStatUnc, fitResult[histName]["uncertainty"] if useFitUnc else 0.0)
            #subtract background from measured data
            measured = u2b.CompoundDistribution(u2b.Subtraction(),measured,backgroundDist)

    tmHist = readHist2d(responseFiles,responseHistName,systematic,fitResult[signalHistName]["scale"],REBIN_GEN,REBIN_RECO)
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

    return {"mean":asymmetry.getMeanSampled(0),"uncertainty":asymmetry.getUncertaintySampled(0)}
    
    
def getScale(histFiles,nameRef, sysRef, fitResultRef, name, sys, fitResult):
    refHist = readHist1d(histFiles,HISTPREFIX+nameRef,sysRef,fitResultRef[nameRef]["scale"],1)
    hist = readHist1d(histFiles,HISTPREFIX+name,sys,fitResult[name]["scale"],1)
    return hist.Integral()/refHist.Integral()
    
if __name__=="__main__":

    parser = OptionParser()
    parser.add_option("-v", "--verbose",action="store_true", dest="verbose", default=False, help="Verbose output")
    parser.add_option("--mc-only",action="store_true",dest="mc_only",default=False,help="Substitude data with MC")
    parser.add_option("--histFile",action="append",dest="histFiles",default=[],help="The rootfile containing the signal/background histograms for unfolding")
    parser.add_option("--sys",action="store",dest="systematic",default="nominal",help="Systematic shift.")
    parser.add_option("--responseFile",action="append",dest="responseFiles",default=[],help="The <rootfile> containing the 2d response matrix.")
    parser.add_option("--responseMatrixName",action="store",dest="responseMatrixName",default="tm",help="The<histname> containing the 2d response matrix.")
    parser.add_option("--fitResultPrefix",action="store",dest="fitResultPrefix",default="",help="Path prefix to the fit result")
    parser.add_option("--fitResult",action="store",dest="fitResult",default="mu.txt",help="Path to the fit result")
    parser.add_option("--fitCovariance",action="store",dest="fitCovariance",default="mu_cov.root",help="Path to the fit covariance root file")
    parser.add_option("--output",action="store",dest="output",default="out.csv",help="The output file")
    parser.add_option("--overrideOutputSysName",action="store",dest="outputsysname",default=None,help="Overrides the sys name stored in the csv file")
    parser.add_option("--normalizeSysToNominal",action="store_true",dest="normsys",default=False,help="Normalizes the sys hists to nominal. Discards fit result.")
    parser.add_option("--no-stat",action="store_false",default=True,dest="stat",help="Deactivates statistical uncertainties (taken from data).")
    parser.add_option("--no-mcstat",action="store_false",default=True,dest="mcstat",help="Deactivates limited MC statistics uncertainties (taken from background MC).")
    parser.add_option("--no-fiterror",action="store_false",default=True,dest="fiterror",help="Deactivates uncertainties from the fit (taken from file with fit result).")
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
        print "sys: "
    

    if (verbose):
        if options.systematic=="nominal":
            print " ... scale: "+os.path.join(options.fitResultPrefix,"nominal",options.fitResult)
            print " ... covaraince: "+os.path.join(options.fitResultPrefix,"nominal",options.fitCovariance)
        else:
            print " ... scale (up): "+os.path.join(options.fitResultPrefix,options.systematic+"__up",options.fitResult)
            print " ... scale (down): "+os.path.join(options.fitResultPrefix,options.systematic+"__down",options.fitResult)
            print " ... covaraince (up): "+os.path.join(options.fitResultPrefix,options.systematic+"__up",options.fitCovariance)
            print " ... covaraince (down): "+os.path.join(options.fitResultPrefix,options.systematic+"__down",options.fitCovariance)
        print
            
    if (options.outputsysname):
        print "WARNING: will override the output sys name in the csv file to: ",options.outputsysname
        
    outputFile = open(options.output, 'wb')
    writer = csv.DictWriter(outputFile, ["syst","up","down","dup","ddown","d"], restval='NAN', extrasaction='raise', dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
    writer.writeheader()
    if options.systematic=="nominal":

        fitResult = readFitResult(
            os.path.join(options.fitResultPrefix,"nominal",options.fitResult),
            os.path.join(options.fitResultPrefix,"nominal",options.fitCovariance)
        )
        result = doUnfolding(
            options.histFiles,
            SIGNAL,
            BACKGROUND,
            DATA,
            options.responseFiles,
            options.responseMatrixName,
            fitResult,
            systematic="nominal",
            useStatUnc=options.stat,
            useMCStatUnc=options.mcstat,
            useFitUnc=options.fiterror
        )
        if options.stat and options.mcstat and options.fiterror:
            writer.writerow({
                "syst":options.outputsysname if options.outputsysname else "nominal",
                "up":result["mean"]+0.5*result["uncertainty"],
                "down":result["mean"]-0.5*result["uncertainty"],
                "dup":0.5*result["uncertainty"],
                "ddown":0.5*result["uncertainty"],
                "d":0.5*result["uncertainty"]
            })
        elif options.stat and not options.mcstat and not options.fiterror:
            writer.writerow({
                "syst":options.outputsysname if options.outputsysname else "stat",
                "up":result["mean"]+0.5*result["uncertainty"],
                "down":result["mean"]-0.5*result["uncertainty"],
                "dup":0.5*result["uncertainty"],
                "ddown":0.5*result["uncertainty"],
                "d":0.5*result["uncertainty"]
            })
        elif not options.stat and options.mcstat and not options.fiterror:
            writer.writerow({
                "syst":options.outputsysname if options.outputsysname else "mcstat",
                "up":result["mean"]+0.5*result["uncertainty"],
                "down":result["mean"]-0.5*result["uncertainty"],
                "dup":0.5*result["uncertainty"],
                "ddown":0.5*result["uncertainty"],
                "d":0.5*result["uncertainty"]
            })
        elif not options.stat and not options.mcstat and options.fiterror:
            writer.writerow({
                "syst":options.outputsysname if options.outputsysname else "fiterror",
                "up":result["mean"]+0.5*result["uncertainty"],
                "down":result["mean"]-0.5*result["uncertainty"],
                "dup":0.5*result["uncertainty"],
                "ddown":0.5*result["uncertainty"],
                "d":0.5*result["uncertainty"]
            })
        else:
            print "WARNING: systematic configuration not known"
            writer.writerow({
                "syst":options.outputsysname if options.outputsysname else "unknown",
                "up":result["mean"]+0.5*result["uncertainty"],
                "down":result["mean"]-0.5*result["uncertainty"],
                "dup":0.5*result["uncertainty"],
                "ddown":0.5*result["uncertainty"],
                "d":0.5*result["uncertainty"]
            })

    else:

        fitResultNominal = readFitResult(
            os.path.join(options.fitResultPrefix,"nominal",options.fitResult),
            os.path.join(options.fitResultPrefix,"nominal",options.fitCovariance)
        )
        fitResultUp = readFitResult(
            os.path.join(options.fitResultPrefix,options.systematic+"__up",options.fitResult),
            os.path.join(options.fitResultPrefix,options.systematic+"__up",options.fitCovariance)
        )
        fitResultDown = readFitResult(
            os.path.join(options.fitResultPrefix,options.systematic+"__down",options.fitResult),
            os.path.join(options.fitResultPrefix,options.systematic+"__down",options.fitCovariance)
        )
        if (options.normsys):
            fitResultUp=fitResultNominal
            fitResultDown=fitResultNominal
            for histName in fitResultNominal.keys():
                scaleUp=getScale(options.histFiles,histName, options.systematic+"__up", fitResultNominal, 
                                histName, "nominal", fitResultNominal)
                scaleDown=getScale(options.histFiles,histName, options.systematic+"__down", fitResultNominal, 
                                histName, "nominal", fitResultNominal)
                                
                print histName,options.systematic,"up",scaleUp
                print histName,options.systematic,"down",scaleDown
                fitResultUp[histName]["scale"]=fitResultNominal[histName]["scale"]*scaleUp
                fitResultDown[histName]["scale"]=fitResultNominal[histName]["scale"]*scaleDown
            
        resultNominal = doUnfolding(
            options.histFiles,
            SIGNAL,
            BACKGROUND,
            DATA,
            options.responseFiles,
            options.responseMatrixName,
            fitResultNominal,
            systematic="nominal",
            useStatUnc=False,
            useMCStatUnc=False,
            useFitUnc=False
        )
            
        resultUp = doUnfolding(
            options.histFiles,
            SIGNAL,
            BACKGROUND,
            DATA,
            options.responseFiles,
            options.responseMatrixName,
            fitResultUp,
            systematic=options.systematic+"__up",
            useStatUnc=False,
            useMCStatUnc=False,
            useFitUnc=False
        )

        resultDown = doUnfolding(
            options.histFiles,
            SIGNAL,
            BACKGROUND,
            DATA,
            options.responseFiles,
            options.responseMatrixName,
            fitResultDown,
            systematic=options.systematic+"__down",
            useStatUnc=False,
            useMCStatUnc=False,
            useFitUnc=False
        )
        unc = max(math.fabs(resultNominal["mean"]-resultDown["mean"]),math.fabs(resultNominal["mean"]-resultUp["mean"]))
        writer.writerow({
            "syst":options.outputsysname if options.outputsysname else options.systematic,
            "up":resultUp["mean"],
            "down":resultDown["mean"],
            "dup":resultUp["mean"]-resultNominal["mean"],
            "ddown":resultNominal["mean"]-resultDown["mean"],
            "d":unc
        })
    outputFile.close()
