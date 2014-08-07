import csv
import os
import math
import sys
import numpy
import os.path
import ROOT
from optparse import OptionParser

ROOT.gROOT.Reset()
#ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetPadTopMargin(0.1)
ROOT.gStyle.SetPadLeftMargin(0.15)
ROOT.gStyle.SetPadRightMargin(0.25)
ROOT.gStyle.SetPadBottomMargin(0.15)
ROOT.gStyle.SetMarkerSize(0.16)
ROOT.gStyle.SetHistLineWidth(1)
ROOT.gStyle.SetStatFontSize(0.025)
ROOT.gStyle.SetTitleFontSize(0.05)
ROOT.gStyle.SetTitleSize(0.06, "XYZ")
ROOT.gStyle.SetLabelSize(0.05, "XYZ")
ROOT.gStyle.SetNdivisions(510, "XYZ")

ROOT.gStyle.SetOptFit()
ROOT.gStyle.SetOptStat(0)

# For the canvas:
ROOT.gStyle.SetCanvasBorderMode(0)
ROOT.gStyle.SetCanvasColor(ROOT.TStyle.kWhite)
ROOT.gStyle.SetCanvasDefH(600) #Height of canvas
ROOT.gStyle.SetCanvasDefW(600) #Width of canvas
ROOT.gStyle.SetCanvasDefX(0)   #POsition on screen
ROOT.gStyle.SetCanvasDefY(0)

# For the Pad:
ROOT.gStyle.SetPadBorderMode(0)
# ROOT.gStyle.SetPadBorderSize(Width_t size = 1)
ROOT.gStyle.SetPadColor(ROOT.TStyle.kWhite)
ROOT.gStyle.SetPadGridX(True)
ROOT.gStyle.SetPadGridY(True)
ROOT.gStyle.SetGridColor(ROOT.kBlack)
ROOT.gStyle.SetGridStyle(2)
ROOT.gStyle.SetGridWidth(1)

# For the frame:

ROOT.gStyle.SetFrameBorderMode(0)
ROOT.gStyle.SetFrameBorderSize(0)
ROOT.gStyle.SetFrameFillColor(0)
ROOT.gStyle.SetFrameFillStyle(0)
ROOT.gStyle.SetFrameLineColor(1)
ROOT.gStyle.SetFrameLineStyle(1)
ROOT.gStyle.SetFrameLineWidth(0)

# For the histo:
# ROOT.gStyle.SetHistFillColor(1)
# ROOT.gStyle.SetHistFillStyle(0)
ROOT.gStyle.SetHistLineColor(1)
ROOT.gStyle.SetHistLineStyle(0)
ROOT.gStyle.SetHistLineWidth(1)
# ROOT.gStyle.SetLegoInnerR(Float_t rad = 0.5)
# ROOT.gStyle.SetNumberContours(Int_t number = 20)

ROOT.gStyle.SetEndErrorSize(2)
#ROOT.gStyle.SetErrorMarker(20)
ROOT.gStyle.SetErrorX(0.)

ROOT.gStyle.SetMarkerStyle(20)
#ROOT.gStyle.SetMarkerStyle(20)

#For the fit/function:
ROOT.gStyle.SetOptFit(1)
ROOT.gStyle.SetFitFormat("5.4g")
ROOT.gStyle.SetFuncColor(2)
ROOT.gStyle.SetFuncStyle(1)
ROOT.gStyle.SetFuncWidth(1)

#For the date:
ROOT.gStyle.SetOptDate(0)
# ROOT.gStyle.SetDateX(Float_t x = 0.01)
# ROOT.gStyle.SetDateY(Float_t y = 0.01)

# For the statistics box:
ROOT.gStyle.SetOptFile(0)
ROOT.gStyle.SetOptStat(0) # To display the mean and RMS:   SetOptStat("mr")
ROOT.gStyle.SetStatColor(ROOT.TStyle.kWhite)
ROOT.gStyle.SetStatFont(42)
ROOT.gStyle.SetStatFontSize(0.025)
ROOT.gStyle.SetStatTextColor(1)
ROOT.gStyle.SetStatFormat("6.4g")
ROOT.gStyle.SetStatBorderSize(1)
ROOT.gStyle.SetStatH(0.1)
ROOT.gStyle.SetStatW(0.15)
# ROOT.gStyle.SetStaROOT.TStyle(Style_t style = 1001)
# ROOT.gStyle.SetStatX(Float_t x = 0)
# ROOT.gStyle.SetStatY(Float_t y = 0)


#ROOT.gROOT.ForceStyle(True)
#end modified

# For the Global title:

ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetTitleFont(42)
ROOT.gStyle.SetTitleColor(1)
ROOT.gStyle.SetTitleTextColor(1)
ROOT.gStyle.SetTitleFillColor(10)
ROOT.gStyle.SetTitleFontSize(0.03)
# ROOT.gStyle.SetTitleH(0) # Set the height of the title box
# ROOT.gStyle.SetTitleW(0) # Set the width of the title box
#ROOT.gStyle.SetTitleX(0.35) # Set the position of the title box
#ROOT.gStyle.SetTitleY(0.986) # Set the position of the title box
# ROOT.gStyle.SetTitleStyle(Style_t style = 1001)
#ROOT.gStyle.SetTitleBorderSize(0)

# For the axis titles:
ROOT.gStyle.SetTitleColor(1, "XYZ")
ROOT.gStyle.SetTitleFont(42, "XYZ")
#ROOT.gStyle.SetTitleSize(0.06, "XYZ")
ROOT.gStyle.SetTitleSize(0.048, "XYZ")
# ROOT.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
# ROOT.gStyle.SetTitleYSize(Float_t size = 0.02)
ROOT.gStyle.SetTitleXOffset(0.9)
ROOT.gStyle.SetTitleYOffset(1.15)
#ROOT.gStyle.SetTitleOffset(1.1, "Y") # Another way to set the Offset

# For the axis labels:

ROOT.gStyle.SetLabelColor(1, "XYZ")
ROOT.gStyle.SetLabelFont(42, "XYZ")
ROOT.gStyle.SetLabelOffset(0.0077, "XYZ")
ROOT.gStyle.SetLabelSize(0.04, "XYZ")
#ROOT.gStyle.SetLabelSize(0.04, "XYZ")

# For the axis:

ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetStripDecimals(True)
ROOT.gStyle.SetTickLength(0.03, "XYZ")
ROOT.gStyle.SetNdivisions(510, "XYZ")

ROOT.gStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
ROOT.gStyle.SetPadTickY(1)

# Change for log plots:
ROOT.gStyle.SetOptLogx(0)
ROOT.gStyle.SetOptLogy(0)
ROOT.gStyle.SetOptLogz(0)

#ROOT.gStyle.SetPalette(1) #(1,0)

# another top group addition
ROOT.gStyle.SetHatchesSpacing(1.0)

# Postscript options:
#ROOT.gStyle.SetPaperSize(20., 20.)
#ROOT.gStyle.SetPaperSize(ROOT.TStyle.kA4)
#ROOT.gStyle.SetPaperSize(27., 29.7)
#ROOT.gStyle.SetPaperSize(27., 29.7)
ROOT.TGaxis.SetMaxDigits(3)
ROOT.gStyle.SetLineScalePS(2)

# ROOT.gStyle.SetLineStyleString(Int_t i, const char* text)
# ROOT.gStyle.SetHeaderPS(const char* header)
# ROOT.gStyle.SetTitlePS(const char* pstitle)
#ROOT.gStyle.SetColorModelPS(1)

# ROOT.gStyle.SetBarOffset(Float_t baroff = 0.5)
# ROOT.gStyle.SetBarWidth(Float_t barwidth = 0.5)
# ROOT.gStyle.SetPaintTextFormat(const char* format = "g")
# ROOT.gStyle.SetPalette(Int_t ncolors = 0, Int_t* colors = 0)
# ROOT.gStyle.SetTimeOffset(Double_t toffset)
# ROOT.gStyle.SetHistMinimumZero(kTRUE)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetPaintTextFormat("7.4f")

rootObj=[]

def loadDict(fileList):
    resultDict={}
    for f in fileList:
        
        
        for readDict in csvFile:
            resultDict[readDict["syst"]]=readDict
    return resultDict
    
if __name__=="__main__":
    parser = OptionParser()
    (options, args) = parser.parse_args()
    if len(args)<=1:
        sys.exit(1)
    print "input: ",args[0]
    print "output: ",args[1]
    
    inFile = open(args[0],"rb")
    csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
    
    vl=[]
    vr=[]
    generatedA=[]
    unfoldedA=[]
    uncertaintyA=[]
    for readDict in csvFile:
        vl.append(float(readDict["vl"]))
        vr.append(float(readDict["vr"]))
        generatedA.append(float(readDict["generated"]))
        unfoldedA.append(float(readDict["unfolded"]))
        uncertaintyA.append(float(readDict["uncertainty"]))
    inFile.close()
    
    
    indices=numpy.argsort(numpy.array(generatedA))
    vl=numpy.array(vl)[indices]
    vr=numpy.array(vr)[indices]
    generatedA=numpy.array(generatedA)[indices]
    unfoldedA=numpy.array(unfoldedA)[indices]
    uncertaintyA=numpy.array(uncertaintyA)[indices]
    
    
    
    cv=ROOT.TCanvas("cv","",800,600)
    axis=ROOT.TH2F("axis",";unfolded(A);generated(A)",50,0.0,0.55,50,0.0,0.55)
    axis.Draw("AXIS")
    
    errorBandX=numpy.zeros(len(generatedA)*2)
    errorBandY=numpy.zeros(len(generatedA)*2)
    for i in range(len(generatedA)):
        errorBandX[i]=unfoldedA[i]+uncertaintyA[i]
        errorBandY[i]=generatedA[i]
        errorBandX[-1-i]=unfoldedA[i]-uncertaintyA[i]
        errorBandY[-1-i]=generatedA[i]
        
    poly=ROOT.TPolyLine(len(errorBandX),errorBandX,errorBandY)
    poly.SetFillColor(ROOT.kGray)
    poly.SetLineColor(0)
    poly.Draw("FSame")

    bisector = ROOT.TF1("bisector","x",-1,1)
    bisector.SetLineColor(ROOT.kBlack)
    bisector.SetLineWidth(2)
    bisector.SetLineStyle(2)
    bisector.Draw("Same")

    Number = 5
    Red   = [0.00, 0.00, 0.87, 1.00, 0.71]
    Green = [ 0.00, 0.81, 1.00, 0.20, 0.00]
    Blue   =[  0.71, 1.00, 0.12, 0.00, 0.00]
    Length = [ 0.00, 0.34, 0.61, 0.84, 1.00 ]
    nb=len(unfoldedA)
    start = ROOT.TColor.CreateGradientColorTable(Number,numpy.array(Length),numpy.array(Red),numpy.array(Green),numpy.array(Blue),nb)

    legend=ROOT.TLegend(0.76,0.9,0.99,0.25)
    legend.SetTextFont(42)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)

    graph = ROOT.TGraph(len(unfoldedA),numpy.array(unfoldedA),numpy.array(generatedA))
    graph.SetLineWidth(2)
    graph.Draw("LSame")

    for i in range(len(unfoldedA)):

        
        legendName="VL=%2.1f VR=%2.1f"%(vl[i],vr[i])
        marker = ROOT.TMarker(unfoldedA[i],generatedA[i],21)
        rootObj.append(marker)
        marker.SetMarkerColor(start+i)
        marker.SetMarkerSize(1.2)
        marker.Draw("Same")
        legend.AddEntry(marker,legendName,"P")



    legend.Draw("Same")

    axis.Draw("AXIS SAME")
    cv.Update()
    cv.Print(args[1])
