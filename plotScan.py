import csv
import os
import os.path
import numpy

import ROOT
import math

from cmsplots.Axis import CoordinateStyle,EmtpyGrid
from cmsplots.Canvas import Canvas

ROOT.gROOT.Reset()
#ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetPadTopMargin(0.05)
ROOT.gStyle.SetPadLeftMargin(0.12)
ROOT.gStyle.SetPadRightMargin(0.25)
ROOT.gStyle.SetPadBottomMargin(0.13)
ROOT.gStyle.SetMarkerSize(0.16)
ROOT.gStyle.SetHistLineWidth(1)
ROOT.gStyle.SetStatFontSize(0.025)
ROOT.gStyle.SetLabelSize(0.055, "XYZ")
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

# ROOT.gStyle.SetTitleH(0) # Set the height of the title box
# ROOT.gStyle.SetTitleW(0) # Set the width of the title box
#ROOT.gStyle.SetTitleX(0.35) # Set the position of the title box
#ROOT.gStyle.SetTitleY(0.986) # Set the position of the title box
# ROOT.gStyle.SetTitleStyle(Style_t style = 1001)
#ROOT.gStyle.SetTitleBorderSize(0)

# For the axis titles:
ROOT.gStyle.SetTitleColor(1, "XYZ")
ROOT.gStyle.SetTitleFont(42, "XYZ")
ROOT.gStyle.SetTitleSize(0.06, "XYZ")
# ROOT.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
# ROOT.gStyle.SetTitleYSize(Float_t size = 0.02)
ROOT.gStyle.SetTitleXOffset(0.9)
ROOT.gStyle.SetTitleYOffset(1.05)
#ROOT.gStyle.SetTitleOffset(1.1, "Y") # Another way to set the Offset

# For the axis labels:

ROOT.gStyle.SetLabelColor(1, "XYZ")
ROOT.gStyle.SetLabelFont(42, "XYZ")
ROOT.gStyle.SetLabelOffset(0.0077, "XYZ")
ROOT.gStyle.SetLabelSize(0.045, "XYZ")
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

sysNames=[
    ["stat","statistical"],
    ["generator","signal modeling"],
    ["tchan_scale","t-channel scale"],
    ["ttjets_scale","tt+jets scale"],
    ["wzjets_scale","W+jets scale"],
    ["mass","top quark mass"],
    ["wjets_shape","W+jets shape"],
    ["wjets_flavour_heavy","W+jets heavy flavour"],
    ["wjets_flavour_light","W+jets light flavour"],
    ["top_weight","top pT modeling"],
    ["wzjets_matching","W+jets matching"],
    ["ttjets_matching","tt+jets matching"],
    #["pdf","PDF"],
    ["jes","JES"],
    ["jer","JER"],
    ["met","MET"],
    ["lepton_id","lepton id"],
    ["lepton_iso","lepton isolation"],
    ["lepton_trigger","lepton trigger"],        
    ["pu","pileup"],
    ["btag_bc","b-tagging efficiency"],
    ["btag_l","mis-tagging efficiency"],
    #["lepton_weight","lepton weight"],
    ["qcd_yield","QCD yield"],
    ["qcd_antiiso","QCD template"],
    #["fiterror","ML-fit uncertainty"],
    #["bias","unfolding bias"],
    ["mcstat","limited MC"]
]

whiteList=[
    "stat",
    "generator",
    "tchan_scale",
    "ttjets_scale",
    "wzjets_scale",
    "mass",
    "wzjets_matching",
    "ttjets_matching",
    #"jes",
    "qcd_yield",
    #"qcd_antiiso",
    "mcstat",
]

def readCSV(folder=os.getcwd(),match="mu_"):
    sysDict={}
    for f in os.listdir(folder):
        if f.startswith(match) and f.endswith(".csv"):
            inFile = open(os.path.join(folder,f),"rb")
            csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
            result = csvFile.next()
            sysDict[result["syst"]]=result["d"]
            inFile.close()
    return sysDict
    
    
basefolder=os.path.join(os.getcwd(),"histos","scan")
scans=[]
for folder in os.listdir(basefolder):
    scans.append({
        "cut":(float(folder)),
        "sys":readCSV(os.path.join(basefolder,folder,"tunfold"),"ele_")
    })
 
scans=sorted(scans[:],cmp=lambda x,y: int(1000*(x["cut"]-y["cut"])))

    
    
xValues=numpy.zeros(len(scans))
yValues=numpy.zeros((len(sysNames)+1,len(scans)))

for i,scan in enumerate(scans):
    xValues[i]=scan["cut"]
    totalSys=0.0
    for j,sys in enumerate(sysNames):
        #print scan
        totalSys+=scan["sys"][sys[0]]**2
        yValues[j][i]=scan["sys"][sys[0]]
    yValues[-1][i]=math.sqrt(totalSys)
        
'''     
c = Canvas()  
c.addDrawable(EmtpyGrid(-0.15,0.85,0.0,0.1))
style = CoordinateStyle("BDT WP","uncertainty")
c.setCoordinateStyle(style)
c.draw()
'''
cv = ROOT.TCanvas("cv","",800,600)
axis=ROOT.TH2F("axis",";BDT WP; uncertainty",50,-0.15,0.9,50,0.0,0.15)
axis.Draw("AXIS")

rootObj=[]

legend=ROOT.TLegend(0.755,0.9,0.99,0.2)
legend.SetBorderSize(0)
legend.SetFillColor(ROOT.kWhite)
legend.SetTextFont(42)

stops = numpy.array([0.00, 0.2,  0.4,  0.55, 0.68, 0.78, 1.00 ])
red   = numpy.array([0.00, 0.00, 0.5,  0.0,  0.5, 0.9, 0.6])
green = numpy.array([0.00, 0.6,  0.0,  0.9,  0.9, 0.5, 0.00])
blue  = numpy.array([0.71, 0.9,  0.5,  0.4,  0.0, 0.00, 0.00])


col=ROOT.TColor.CreateGradientColorTable(len(stops), stops, red, green, blue, len(whiteList))

n=0
for i,sys in enumerate(sysNames):
    if sys[0] in whiteList:
        graph=ROOT.TGraph(len(scans),xValues,yValues[i])
        rootObj.append(graph)
        graph.SetLineColor(col+n)
        graph.SetLineWidth(3+2*(n%2))
        graph.SetLineStyle(n%2+1)
        legend.AddEntry(graph,sys[1],"L")
        graph.Draw("PCSame")
        n+=1
    
graph=ROOT.TGraph(len(scans),xValues,yValues[-1])
rootObj.append(graph)
graph.SetLineWidth(2)
graph.SetLineColor(ROOT.kBlack)
legend.AddEntry(graph,"total","L")
graph.Draw("PCSame")

legend.Draw("Same")
    
cv.Update()
cv.WaitPrimitive()
    



