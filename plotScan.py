import csv
import os
import os.path
import numpy

import ROOT
import math

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
        "sys":readCSV(os.path.join(basefolder,folder,"tunfold"),"mu_")
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
        
        
cv = ROOT.TCanvas("cv","",800,600)
axis=ROOT.TH2F("axis",";BDT WP; uncertainty",50,-0.15,0.9,50,0.0,0.1)
axis.Draw("AXIS")
rootObj=[]

legend=ROOT.TLegend(0.8,0.9,0.99,0.2)

stops = numpy.array([0.00, 0.34, 0.61, 0.84, 1.00])
red   = numpy.array([0.00, 0.00, 0.87, 1.00, 0.51])
green = numpy.array([0.00, 0.81, 1.00, 0.20, 0.00])
blue  = numpy.array([0.51, 1.00, 0.12, 0.00, 0.00])


col=ROOT.TColor.CreateGradientColorTable(len(stops), stops, red, green, blue, len(sysNames))

for i,sys in enumerate(sysNames):
    graph=ROOT.TGraph(len(scans),xValues,yValues[i])
    rootObj.append(graph)
    graph.SetLineColor(col+i)
    graph.SetLineWidth(2)
    legend.AddEntry(graph,sys[1],"L")
    graph.Draw("PCSame")
    
graph=ROOT.TGraph(len(scans),xValues,yValues[-1])
rootObj.append(graph)
graph.SetLineWidth(2)
graph.SetLineColor(ROOT.kBlack)
legend.AddEntry(graph,"total","L")
graph.Draw("PCSame")

legend.Draw("Same")
    
cv.Update()
cv.WaitPrimitive()
    



