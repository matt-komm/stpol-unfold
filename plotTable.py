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
ROOT.gStyle.SetPadLeftMargin(0.31)
ROOT.gStyle.SetPadRightMargin(0.075)
ROOT.gStyle.SetPadTopMargin(0.01)
ROOT.gStyle.SetPadBottomMargin(0.07)
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
ROOT.gStyle.SetGridStyle(3)
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
ROOT.gStyle.SetTitleFont(43, "XYZ")
#ROOT.gStyle.SetTitleSize(0.06, "XYZ")
ROOT.gStyle.SetTitleSize(28, "XYZ")
# ROOT.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
# ROOT.gStyle.SetTitleYSize(Float_t size = 0.02)
ROOT.gStyle.SetTitleXOffset(0.95)
ROOT.gStyle.SetTitleYOffset(1.15)
#ROOT.gStyle.SetTitleOffset(1.1, "Y") # Another way to set the Offset

# For the axis labels:

ROOT.gStyle.SetLabelColor(1, "XYZ")
ROOT.gStyle.SetLabelFont(43, "XYZ")
ROOT.gStyle.SetLabelOffset(0.0077, "XYZ")
ROOT.gStyle.SetLabelSize(24, "X")
ROOT.gStyle.SetLabelSize(20, "Y")
#ROOT.gStyle.SetLabelSize(0.04, "XYZ")

# For the axis:

ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetStripDecimals(True)
ROOT.gStyle.SetTickLength(0.02, "XYZ")
ROOT.gStyle.SetNdivisions(512, "XYZ")

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
ROOT.gStyle.SetPaperSize(14.0, 14.0/800.0*1200.0)
#ROOT.gStyle.SetPaperSize(ROOT.TStyle.kA4)
#ROOT.gStyle.SetPaperSize(27., 29.7)
#ROOT.gStyle.SetPaperSize(27., 29.7)
ROOT.TGaxis.SetMaxDigits(3)
ROOT.gStyle.SetLineScalePS(1)

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

['stat', "statistical"],

#fitting
['fiterror', "ML-fit uncertainty"],
['diboson', "Diboson fraction"],
['dyjets', "Drell-Yan fraction"],
['schan', "s-channel fraction"],
['twchan', "tW fraction"],
['qcd_antiiso', "QCD shape"],
['qcd_yield', "QCD yield"],

#detector
['btag_bc', "b tagging"],
['btag_l', "mistagging"],
['jer', "JER"],
['jes', "JES"],
['met', "unclustered \\MET"],
['pu', "pileup"],
['lepton_id', "lepton ID"],
['lepton_iso', "lepton isolation"],
['lepton_trigger', "trigger efficiency"],

#add reweighting
['top_weight', "top \\pT reweighting"],
['wjets_pt_weight', "\\wjets W \\pT reweighting"],
['wjets_flavour_heavy', "\\wjets heavy flavor fraction"],
['wjets_flavour_light', "\\wjets light flavor fraction"],
['wjets_shape', "\\wjets shape reweighting"],
['bias', "unfolding bias"],

#theory
['generator', "generator model"],
['mass', "top quark mass"],
#['tchan_scale', "$Q^{2}$ scale t-channel"],
['tchan_qscale_me_weight', "$Q^{2}$ scale t-channel"],
['ttjets_scale', "\\ttbar $Q^{2}$ scale"],
#['ttjets_qscale_me_weight', "\\ttbar $Q^{2}$ scale"],
['ttjets_matching', "\\ttbar matching"],
['wzjets_scale', "\\wjets $Q^{2}$ scale"],
#['wzjets_qscale_me_weight', "\\wjets $Q^{2}$ scale"],
['wzjets_matching', "\\wjets matching"],
['pdf', "PDF"],

['mcstat', "limited MC"],
]

sysNames.reverse()
print sysNames[0]

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
        box = ROOT.TBox(0,ypos1,float(sysDict[sysEntry]["d"]),ypos2)
        rootObj.append(box)
        return box
    else:
        box = ROOT.TBox(0,0,0,0)
        rootObj.append(box)
        return box
        

    
if __name__=="__main__":


    basefolder=os.path.join(os.getcwd(),"histos/bdt_Jun22_final/tunfold/0.45")
    basefolder_top=os.path.join(os.getcwd(),"histos/bdt_Jun22_final_top/tunfold/0.45")
    basefolder_antitop=os.path.join(os.getcwd(),"histos/bdt_Jun22_final_antitop/tunfold/0.45")
    
    basefolder2=os.path.join(os.getcwd(),"histos/bdt_Jun22_final/2bin/0.45")
    basefolder2_top=os.path.join(os.getcwd(),"histos/bdt_Jun22_final_top/2bin/0.45")
    basefolder2_antitop=os.path.join(os.getcwd(),"histos/bdt_Jun22_final_antitop/2bin/0.45")
    #eleTUnfoldDict = loadDict(["asymmetries_ele.csv"])
    #eleBinDict = loadDict([f for f in os.listdir(os.getcwd()) if os.path.isfile(f) and f.startswith("ele_") and f.endswith(".csv")])
    #muTUnfoldDict = loadDict(["asymmetries_mu.csv"])
    
    

    topBinDict = loadDict([os.path.join(basefolder_top,f) for f in os.listdir(basefolder_top) if f.startswith("mu__") and f.endswith(".csv")])
    antitopBinDict = loadDict([os.path.join(basefolder_antitop,f) for f in os.listdir(basefolder_antitop) if f.startswith("mu__") and f.endswith(".csv")])
    combBinDict = loadDict([os.path.join(basefolder,f) for f in os.listdir(basefolder) if f.startswith("mu__") and f.endswith(".csv")])
    
    
    topBinDict2bin = loadDict([os.path.join(basefolder2_top,f) for f in os.listdir(basefolder2) if f.startswith("mu__") and f.endswith(".csv")])
    antitopBinDict2bin = loadDict([os.path.join(basefolder2_antitop,f) for f in os.listdir(basefolder2) if f.startswith("mu__") and f.endswith(".csv")])
    combBinDict2bin = loadDict([os.path.join(basefolder2,f) for f in os.listdir(basefolder2) if f.startswith("mu__") and f.endswith(".csv")])
    #eleBinDictn = loadDict([os.path.join(basefolder2,f) for f in os.listdir(basefolder2) if f.startswith("ele__") and f.endswith(".csv")])
    #combBinDictn = loadDict([os.path.join(basefolder2,f) for f in os.listdir(basefolder2) if f.startswith("combined__") and f.endswith(".csv")])
    
    hist = ROOT.TH2F("hist",";uncertainty;",50,0,0.065,len(sysNames),0,len(sysNames))
    cv = ROOT.TCanvas("cv","",800,900)
    hist.Draw("AXIS")
    
    legend = ROOT.TLegend(0.66,0.87,0.9,0.70)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
    legend.SetTextFont(43)
    legend.SetTextSize(30)
    legend.SetBorderSize(0)
    
    hist.GetYaxis().SetLabelFont(43)
    hist.GetYaxis().SetLabelSize(20)
    
    for index in range(len(sysNames)):
        sys = sysNames[index]
        
        hist.GetYaxis().SetBinLabel(index+1,sys[1].replace("$","").replace("\\wjets","W+jets").replace("\\ttbar","t#bar{t}+jets").replace("\\",""))
        
        line = ROOT.TLine(-0.003,index,0.0,index)
        rootObj.append(line)
        line.Draw("Same") if index!=0 else 0
        
        box = createBox(combBinDict,sys[0],index+0.0,index+1)
        box.SetFillColor(ROOT.kGray)
        box.SetLineColor(ROOT.kGray+2)
        box.SetLineWidth(2)
        box.Draw("SameFL")
        legend.AddEntry(box,"t + #bar{t}","F") if index==0 else 0
        
        box = createBox(combBinDict2bin,sys[0],index+0.0,index+1)
        box.SetFillStyle(0)
        box.SetLineColor(ROOT.kBlack)
        box.Draw("SameL")
        
        box = createBox(topBinDict,sys[0],index+0.2,index+0.4)
        box.SetFillColor(ROOT.kAzure-4)
        box.SetLineColor(ROOT.kAzure-6)
        box.SetLineWidth(2)
        box.Draw("SameFL")
        legend.AddEntry(box,"t only","F") if index==0 else 0
        
        box = createBox(topBinDict2bin,sys[0],index+0.2,index+0.4)
        box.SetFillStyle(0)
        box.SetLineColor(ROOT.kBlack)
        box.Draw("SameL")
        
        box = createBox(antitopBinDict,sys[0],index+0.6,index+0.8)
        box.SetFillColor(ROOT.kOrange)
        box.SetLineColor(ROOT.kOrange-3)
        box.SetLineWidth(2)
        box.Draw("SameFL")
        legend.AddEntry(box,"#bar{t} only","F") if index==0 else 0
        
        box = createBox(antitopBinDict2bin,sys[0],index+0.6,index+0.8)
        box.SetFillStyle(0)
        box.SetLineColor(ROOT.kBlack)
        box.Draw("SameL")
        
        legend.AddEntry(box,"2bin","F") if index==0 else 0
        '''
        box = createBox(combBinDict,sys[0],index+0.63,index+0.9)
        box.SetFillColor(ROOT.kMagenta)
        box.Draw("SameF")
        legend.AddEntry(box,"both","F") if index==0 else 0
        
        
        '''

        
        
        '''
        box = createBox(combBinDictn,sys[0],index+0.63,index+0.9)
        box.SetFillStyle(0)
        box.SetLineColor(ROOT.kBlack)
        box.Draw("SameL")
        legend.AddEntry(box,"2-bin unfolding","F") if index==0 else 0
        '''
        
    
    hist.Draw("AXIS SAME")
    legend.Draw("Same")
    
    paveCMS=ROOT.TPaveText(0.65,0.92,0.9,0.92,"NDC")
    paveCMS.SetFillColor(ROOT.kWhite)
    paveCMS.SetTextFont(63)
    paveCMS.SetTextSize(40)
    paveCMS.SetTextAlign(11)
    paveCMS.AddText("CMS")
    paveCMS.Draw("SAME")

    pavePrel=ROOT.TPaveText(0.65,0.88,0.9,0.88,"NDC")
    pavePrel.SetFillColor(ROOT.kWhite)
    pavePrel.SetTextFont(53)
    pavePrel.SetTextSize(40)
    pavePrel.SetTextAlign(11)
    pavePrel.AddText("Preliminary")
    pavePrel.Draw("SAME")
    
    
    cv.Update()
    cv.Print("statEval.pdf")
    cv.Print("statEval.png")
    cv.WaitPrimitive()
