import csv
import os
import math
import os.path
import ROOT

ROOT.gROOT.Reset()
#ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetPadLeftMargin(0.15)
ROOT.gStyle.SetPadRightMargin(0.2)
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
        inFile = open(f,"rb")
        csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
        for readDict in csvFile:
            resultDict[readDict["syst"]]=readDict
    return resultDict
    
    
def createBox(sysDict,sysEntry,ypos1,ypos2):
    if sysDict.has_key(sysEntry):
        box = ROOT.TBox(ypos1,0,ypos2,float(sysDict[sysEntry]["d"]))
        rootObj.append(box)
        return box
    else:
        box = ROOT.TBox(0,0,0,0)
        rootObj.append(box)
        return box
    
if __name__=="__main__":
    dicts =["statistical","generator","tchan_scale","ttjets_scale","wzjets_scale",
            "mass","wjets_shape","wjets_flavor","top_weight","wzjets_matching","ttjets_matching",
            "pdf","jes","jer","met","lepton_id","lepton_iso","lepton_trigger",
            "pu","btag_bc","btag_l","lepton_weight","qcd_range","qcd_cont",
            "fituncertainty","bias","limited_mc"]

    eleTUnfoldDict = loadDict(["asymmetries_ele.csv"])
    eleBinDict = loadDict([f for f in os.listdir(os.getcwd()) if os.path.isfile(f) and f.startswith("ele_") and f.endswith(".csv")])
    muTUnfoldDict = loadDict(["asymmetries_mu.csv"])
    muBinDict = loadDict([f for f in os.listdir(os.getcwd()) if os.path.isfile(f) and f.startswith("mu_") and f.endswith(".csv")])
    
    hist = ROOT.TH2F("hist","",len(dicts),0,len(dicts),50,0,0.13)
    cv = ROOT.TCanvas("cv","",800,600)
    hist.Draw("AXIS")
    
    for index in range(len(dicts)):
        sys = dicts[index]
        hist.GetXaxis().SetBinLabel(index+1,sys)
        
        box = createBox(eleTUnfoldDict,sys,index,index+0.45)
        box.SetFillColor(ROOT.kAzure-4)
        box.Draw("SameF")
        
        box = createBox(muTUnfoldDict,sys,index+0.45,index+0.90)
        box.SetFillColor(ROOT.kOrange)
        box.Draw("SameF")
        
        box = createBox(eleBinDict,sys,index,index+0.45)
        box.SetLineColor(ROOT.kBlack)
        box.SetFillStyle(0)
        box.Draw("SameL")
        
        box = createBox(muBinDict,sys,index+0.45,index+0.9)
        box.SetLineColor(ROOT.kBlack)
        box.SetFillStyle(0)
        box.Draw("SameL")
        
    hist.Draw("AXIS SAME")
    cv.Update()
    cv.WaitPrimitive()
