from cmsplots import *
from cmsplots.Canvas import *
from cmsplots.Histogram import *
from cmsplots.Style import *

import ROOT
import os
import os.path

import numpy,scipy,scipy.optimize



ROOT.gROOT.Reset()
#ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetPadTopMargin(0.07)
ROOT.gStyle.SetPadLeftMargin(0.16)
ROOT.gStyle.SetPadRightMargin(0.05)
ROOT.gStyle.SetPadBottomMargin(0.12)
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
ROOT.gStyle.SetTitleSize(0.057, "XYZ")
# ROOT.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
# ROOT.gStyle.SetTitleYSize(Float_t size = 0.02)
ROOT.gStyle.SetTitleXOffset(1.0)
ROOT.gStyle.SetTitleYOffset(1.15)
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
ROOT.gStyle.SetNdivisions(512, "Y")

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
    "nominal",
    "stat",
    "generator",
    "tchan_scale",
    "ttjets_scale",
    "wzjets_scale",
    "mass",
    "wjets_shape",
    "wjets_flavour_heavy",
    "wjets_flavour_light",
    "top_weight",
    "wzjets_matching",
    "ttjets_matching",
    #"pdf",
    "jes",
    "jer",
    "met",
    "lepton_id",
    "lepton_iso",
    "lepton_trigger",
    "pu",
    "btag_bc",
    "btag_l",
    #"lepton_weight",
    "qcd_yield",
    "qcd_antiiso",
    "fiterror",
    "mcstat",
]


sysNames=[
    "nominal",
    "tchan_scale"
]

def calculateChi2(hist1,covHist1,hist2,covHist2=None):
    chi2=0.0
    
    cov1=ROOT.TMatrixD(hist1.GetNbinsX(),hist1.GetNbinsX())
    if covHist2!=None:
        cov2=ROOT.TMatrixD(hist1.GetNbinsX(),hist1.GetNbinsX())
    for ibin in range(hist1.GetNbinsX()):
        for jbin in range(hist1.GetNbinsX()):
            cov1[ibin][jbin]=covHist1.GetBinContent(ibin+1,jbin+1)
            if covHist2!=None:
                cov2[ibin][jbin]=covHist2.GetBinContent(ibin+1,jbin+1)
    invCov1=ROOT.TMatrixD(ROOT.TMatrixD.kInverted,cov1)
    if covHist2!=None:
        invCov2=ROOT.TMatrixD(ROOT.TMatrixD.kInverted,cov2)
    for ibin in range(hist1.GetNbinsX()):
        for jbin in range(hist1.GetNbinsX()):
            diff=(hist1.GetBinContent(ibin+1)-hist2.GetBinContent(ibin+1))*(hist1.GetBinContent(jbin+1)-hist2.GetBinContent(jbin+1))
            #combine error 1/e=1/(1/e1+1/e2)
            error=1.0/invCov1[ibin][jbin]
            if covHist2!=None:
                error+=1.0/invCov2[ibin][jbin]
            chi2+=diff/error
    return chi2/hist1.GetNbinsX()
    

    
def diceGaus(output,hist,covHist):
    N=hist.GetNbinsX()
    mean=numpy.zeros(N)
    cov=numpy.zeros((N,N))
    for i in range(N):
        for j in range(N):
            cov[i][j]=covHist.GetBinContent(i+1,j+1)
    diced=numpy.random.multivariate_normal(mean,cov)
    for i in range(N):
        output[i]+=diced[i]
            
def diceShape(output,nominalHist,upHist,downHist):
    d=ROOT.gRandom.Gaus(0.0,1.0)
    for i in range(nominalHist.GetNbinsX()):
        
        up=upHist.GetBinContent(i+1)
        nom=nominalHist.GetBinContent(i+1)
        down=downHist.GetBinContent(i+1)
        #print i,down/downHist.Integral()*3
        #symmetrize if only one-sided
        '''
        if (up<nom and down<nom) or (up>nom and down>nom):
            diff=max(math.fabs(up-nom),math.fabs(down-nom))
            up=nom+diff
            down=nom-diff
        '''
        
        #symmetrize always
        
        diff=max(math.fabs(up-nom),math.fabs(down-nom))
        #print nom,down,diff,nom-diff
        up=nom+diff
        down=nom-diff
        
        #d=-1.0
        
        if d>1:
            output[i]+= (up-nom)*math.fabs(d)
        elif d<-1:
            output[i]+= (down-nom)*math.fabs(d)
        else:
            output[i]+= d/2.0*(up-down)+(d*d-math.fabs(d*d*d)/2.0)*(up+down-2.0*nom)
    #print output/sum(output)*3
            
def normalize(output):
    s=0.0
    for i in range(len(output)):
        if output[i]<0.0:
            output[i]=0.0
        s+=output[i]
    for i in range(len(output)):
        output[i]=output[i]/s*len(output)/2.0
        
def calculateAsymmetry(output,cov):
    if type(output)==type(ROOT.TH1D()):
        norm=output.Integral()
        N=output.GetNbinsX()
    else:
        norm=sum(output)
        N=len(output)
    measured=numpy.zeros(N)
    covMatrix=numpy.zeros((N,N))
    for i in range(N):
        if type(output)==type(ROOT.TH1D()):
            measured[i]=output.GetBinContent(i+1)/norm
        else:
            measured[i]=output[i]/norm
        for j in range(N):
            covMatrix[i][j]=cov.GetBinContent(i+1,j+1)/norm**2
    covMatrixInf=numpy.linalg.inv(covMatrix)
    
    def chi2(p):
        chi2Sum=0.0
        expect=numpy.zeros(N)
        for i in range(N):
            icos=-1.0+2.0/N*i+2.0/N*0.5
            expect[i]=2.0/N*0.5*(1.0+2.0*p[0]*icos)
        for i in range(N):
            for j in range(N):
                chi2Sum+=(measured[i]-expect[i])*covMatrixInf[i][j]*(measured[j]-expect[j])
        return numpy.array([math.sqrt(chi2Sum)])
        
        
    p0 = numpy.array([0.4])
    #print chi2(p0)
    p1, success = scipy.optimize.leastsq(chi2, p0)#,method='BFGS',tol=0.001, jac=False, bounds=[(-1.0,1.0)])
    return p1
    
    '''
    N=len(output)
    sumUp=0.0
    sumDown=0.0
    for i in range(N/2):
        sumUp+=output[N/2+i]
        sumDown+=output[i]
    return (sumUp-sumDown)/(sumUp+sumDown)
    '''
        
def applyLineStyle(line,color=ROOT.kBlack):
    line.SetLineWidth(2)
    line.SetLineColor(color)

def readHistograms(folder,prefix="mu__"):
    histDict={}
    for sys in sysNames:
        if sys in ["nominal","stat","mcstat","fiterror"]:
            
            fileName=os.path.join(folder,prefix+sys+".root")
            if os.path.exists(fileName):
                histDict[sys]={"isShape":False,"unfolded":{},"input":{}}
                rootFile=ROOT.TFile(fileName)
                histDict[sys]["input"]["nominal"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["nominal"].SetDirectory(0)
                histDict[sys]["unfolded"]["nominal"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["nominal"].SetDirectory(0)
                histDict[sys]["unfolded"]["error"]=rootFile.Get("error")
                histDict[sys]["unfolded"]["error"].SetDirectory(0)
                histDict[sys]["unfolded"]["gen"]=rootFile.Get("gen")
                histDict[sys]["unfolded"]["gen"].SetDirectory(0)
                
                norm=histDict[sys]["unfolded"]["nominal"].Integral()
                histDict[sys]["unfolded"]["nominal"].Scale(3.0/norm)
                histDict[sys]["unfolded"]["error"].Scale((3.0/norm)**2)
                histDict[sys]["unfolded"]["gen"].Scale(3.0/histDict[sys]["unfolded"]["gen"].Integral())
                
                rootFile.Close()
                
        elif sys == "generator":
            fileName=os.path.join(folder,prefix+sys+".root")
            if os.path.exists(fileName):
                histDict[sys]={"isShape":True,"unfolded":{},"input":{}}
                rootFile=ROOT.TFile(fileName)
                histDict[sys]["input"]["up"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["up"].SetDirectory(0)
                
                histDict[sys]["unfolded"]["up"].Scale(3.0/histDict[sys]["unfolded"]["up"].Integral())
                
                #symmetrize uncertainty
                histDict[sys]["input"]["down"]=histDict[sys]["input"]["up"].Clone()
                histDict[sys]["input"]["down"].SetDirectory(0)
                histDict[sys]["input"]["down"].Scale(-1.0)
                histDict[sys]["input"]["down"].Add(histDict["nominal"]["input"]["nominal"],2.0)
                histDict[sys]["unfolded"]["down"]=histDict[sys]["unfolded"]["up"].Clone()
                histDict[sys]["unfolded"]["down"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"].Scale(-1.0)
                histDict[sys]["unfolded"]["down"].Add(histDict["nominal"]["unfolded"]["nominal"],2.0)
                
                
                rootFile.Close()
             
        else:
            
            upFileName=os.path.join(folder,prefix+sys+"__up.root")
            downFileName=os.path.join(folder,prefix+sys+"__down.root")
            if os.path.exists(upFileName) and os.path.exists(downFileName):
                histDict[sys]={"isShape":True,"unfolded":{},"input":{}}
                rootFile=ROOT.TFile(upFileName)
                histDict[sys]["input"]["up"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"].Scale(3.0/histDict[sys]["unfolded"]["up"].Integral())
                rootFile.Close()
                rootFile=ROOT.TFile(downFileName)
                histDict[sys]["input"]["down"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["down"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["down"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"].Scale(3.0/histDict[sys]["unfolded"]["down"].Integral())
                rootFile.Close()
    return histDict
    


    

folderTUnfold=os.path.join(os.getcwd(),"histos","tunfold")

sysDict = readHistograms(folderTUnfold)

nominalHist=sysDict["nominal"]["unfolded"]["nominal"]


#cv1=ROOT.TCanvas("cv1","",800,600)
statCov=sysDict["nominal"]["unfolded"]["error"]

#statCov.Draw("colz")
#cv1.WaitPrimitive()

genHist=sysDict["nominal"]["unfolded"]["gen"]

asymmetryHist=ROOT.TH1F("asym","",500,-2.0,2.0)

NTOYS=1000
output=numpy.zeros((NTOYS,nominalHist.GetNbinsX()))

for toy in range(NTOYS):
    if toy%500==0:
        print "process... ",100.0*toy/NTOYS,"%"
    for ibin in range(nominalHist.GetNbinsX()):
        output[toy][ibin]=nominalHist.GetBinContent(ibin+1)
     
    for sysName in sysDict.keys():
        if sysName=="nominal":
            continue
        if not sysDict[sysName]["isShape"]:
            diceGaus(output[toy],nominalHist,sysDict[sysName]["unfolded"]["error"])
        else:
            diceShape(output[toy],nominalHist,sysDict[sysName]["unfolded"]["up"],sysDict[sysName]["unfolded"]["down"])
    
    asymmetryHist.Fill(calculateAsymmetry(output[toy],statCov))  
    #normalize(output[toy])
   
#print calculateAsymmetry(sysDict["jer"]["unfolded"]["up"],statCov)-asymmetryHist.GetMean(),asymmetryHist.GetMean()-calculateAsymmetry(sysDict["jer"]["unfolded"]["down"],statCov)
downSys,mean,upSys=numpy.percentile(output, [15.866,50.0,84.134],0)

#cv2=ROOT.TCanvas("cv2","",800,600)
#asymmetryHist.Draw()
#cv2.WaitPrimitive()
print asymmetryHist.GetMean(),"+-",asymmetryHist.GetRMS()

rootObj=[]


cv=ROOT.TCanvas("cv","",750,600)

axis=ROOT.TH2F("axis",";unfolded cos#theta_{l}*;#lower[-0.05]{#scale[1.0]{#frac{d#sigma}{#sigma#upoint d(cos#theta_{l}*)}}} #lower[0.1]{#scale[1.4]{/}}"+str(round(2.0/genHist.GetNbinsX(),2))+" units",50,-1,1,50,0.0,max(upSys)*1.1)
axis.Draw("AXIS")

legend=ROOT.TLegend(0.19,0.89,0.45,0.68)
legend.SetBorderSize(0)
legend.SetTextFont(42)
legend.SetFillColor(ROOT.kWhite)

'''
statCov.Scale((1.0/nominalHist.Integral()*nominalHist.GetNbinsX()/2.0)**2)
nominalHist.Scale(1.0/nominalHist.Integral()*nominalHist.GetNbinsX()/2.0)
genHist.Scale(1.0/genHist.Integral()*genHist.GetNbinsX()/2.0)
'''

for ibin in range(len(downSys)):
    n=nominalHist.GetBinContent(ibin+1)
    c=nominalHist.GetBinCenter(ibin+1)
    w=nominalHist.GetBinWidth(ibin+1)
    u=upSys[ibin]
    d=downSys[ibin]
    #print ibin,u,d
    #u=max(0.0,upSys[ibin])
    #d=max(0.0,downSys[ibin])
    lineSysUp=ROOT.TLine(c-0.17*w,u,c+0.17*w,u)
    lineSysDown=ROOT.TLine(c-0.17*w,d,c+0.17*w,d)
    lineSysMid=ROOT.TLine(c,d,c,u)
    #applyLineStyle(lineSysUp,ROOT.kBlack)
    #applyLineStyle(lineSysDown,ROOT.kBlack)
    applyLineStyle(lineSysMid,ROOT.kBlack)
    
    rootObj.append(lineSysUp)
    rootObj.append(lineSysDown)
    rootObj.append(lineSysMid)
    #lineSysUp.Draw("Same")
    #lineSysDown.Draw("Same")
    lineSysMid.Draw("Same")
    '''
    box=ROOT.TBox(c-0.5*w,d,c+0.5*w,u)
    box.SetFillColor(17)
    box.SetLineColor(15)
    rootObj.append(box)
    box.Draw("LFSAME")
    '''
    '''
    if ibin==0:
        legend.AddEntry(lineSysUp,"stat.+sys. uncertainty","PE")
    '''
    u=n+math.sqrt(statCov.GetBinContent(ibin+1,ibin+1))
    d=n-math.sqrt(statCov.GetBinContent(ibin+1,ibin+1))
    #print ibin,u,d
    #u=max(0.0,upSys[ibin])
    #d=max(0.0,downSys[ibin])
    lineStatUp=ROOT.TLine(c-0.12*w,u,c+0.12*w,u)
    lineStatDown=ROOT.TLine(c-0.12*w,d,c+0.12*w,d)
    lineStatMid=ROOT.TLine(c,d,c,u)
    applyLineStyle(lineStatUp,ROOT.kBlack)
    applyLineStyle(lineStatDown,ROOT.kBlack)
    applyLineStyle(lineStatMid,ROOT.kBlack)
    rootObj.append(lineStatUp)
    rootObj.append(lineStatDown)
    rootObj.append(lineStatMid)
    lineStatUp.Draw("Same")
    lineStatDown.Draw("Same")
    #lineStatMid.Draw("Same")
    '''
    box=ROOT.TBox(c-0.5*w,d,c+0.5*w,u)
    box.SetFillColor(ROOT.kOrange+6)
    box.SetLineColor(ROOT.kOrange+10)
    rootObj.append(box)
    box.Draw("LFSAME")
    '''
    '''
    if ibin==0:
        legend.AddEntry(lineStatUp,"stat. uncertainty","PE")
    '''
    
nominalHist.SetMarkerStyle(21)
nominalHist.SetMarkerSize(1.2)
nominalHist.Draw("SAME hist P")
legend.AddEntry(nominalHist,"unfolded data","P")

genHist.SetLineColor(ROOT.kBlue)
genHist.SetLineWidth(4)
genHist.SetLineStyle(2)
genHist.Draw("SameLhist")
legend.AddEntry(genHist,"PowHeg SM","L")



pave=ROOT.TPaveText(0.5,0.92,0.96,0.98,"NDC")
pave.SetFillColor(ROOT.kWhite)
pave.SetTextFont(42)
pave.SetTextAlign(31)
#pave.AddText("e+jets, 16.9 fb^{-1} #lower[-0.1]{#scale[0.9]{(}}8 TeV#lower[-0.1]{#scale[0.9]{)}}")
pave.AddText("#mu+jets, 15.3 fb^{-1} #lower[-0.1]{#scale[0.9]{(}}8 TeV#lower[-0.1]{#scale[0.9]{)}}")
pave.Draw("SAME")

axis.Draw("AXIS SAME ")

legend.Draw("SAME")
cv.Update()
cv.WaitPrimitive()


        






