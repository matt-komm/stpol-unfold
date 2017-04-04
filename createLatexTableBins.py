import csv
import os
import math
import os.path
import ROOT

def formatUnc(unc):

    return "{0:4.3f}".format(unc)
    
tableHeader=["","systematic"]
tableTotal=["","total uncertainty"]
    
tableRows=[
['stat', "statistical"],
["line"],

#fitting
['fiterror', "ML-fit uncertainty"],
['diboson', "Diboson fraction"],
['dyjets', "Drell-Yan fraction"],
['schan', "s-channel fraction"],
['twchan', "tW fraction"],
['qcd_antiiso', "QCD shape"],
['qcd_yield', "QCD yield"],
["line"],

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
["line"],

#add reweighting
['top_weight', "top \\pT reweighting"],
['wjets_pt_weight', "\\wjets W \\pT reweighting"],
['wjets_flavour_heavy', "\\wjets heavy flavor fraction"],
['wjets_flavour_light', "\\wjets light flavor fraction"],
['wjets_shape', "\\wjets shape reweighting"],
#['bias', "unfolding bias"],
["line"],

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
["line"],
['mcstat', "limited MC"],
]


tmp=["nominal"]
for sys in tableRows:
    if sys[0]=="line":
        continue
    tmp.append(sys[0])

sysNames=tmp



def addErrorMatrix(hist,error,scale=1.0):
    for ibin in range(hist.GetNbinsX()):
        hist.SetBinContent(ibin+1,hist.GetBinContent(ibin+1)+scale*math.sqrt(error.GetBinContent(ibin+1,ibin+1)))
    
def readHistograms(folder,sysNames,prefix="mu__"):
    histDict={}
    norm = None
    for sys in sysNames:
        if sys in ["nominal","stat","mcstat","fiterror"]:
            
            fileName=os.path.join(folder,prefix+sys+".root")
            if os.path.exists(fileName):
                histDict[sys]={"isShape":False,"unfolded":{},"input":{}, "gen":{}}
                rootFile=ROOT.TFile(fileName)
                histDict[sys]["input"]["nominal"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["nominal"].SetDirectory(0)
                histDict[sys]["unfolded"]["nominal"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["nominal"].SetDirectory(0)
                histDict[sys]["unfolded"]["error"]=rootFile.Get("error")
                histDict[sys]["unfolded"]["error"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"]=histDict[sys]["unfolded"]["nominal"].Clone()
                histDict[sys]["unfolded"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"]=histDict[sys]["unfolded"]["nominal"].Clone()
                histDict[sys]["unfolded"]["down"].SetDirectory(0)
                
                for ibin in range(histDict[sys]["unfolded"]["nominal"].GetNbinsX()):
                    histDict[sys]["unfolded"]["up"].SetBinContent(ibin+1,histDict[sys]["unfolded"]["nominal"].GetBinContent(ibin+1)+math.sqrt(histDict[sys]["unfolded"]["error"].GetBinContent(ibin+1,ibin+1)))
                    histDict[sys]["unfolded"]["down"].SetBinContent(ibin+1,histDict[sys]["unfolded"]["nominal"].GetBinContent(ibin+1)-math.sqrt(histDict[sys]["unfolded"]["error"].GetBinContent(ibin+1,ibin+1)))
                
                if sys=="nominal":
                    norm=histDict["nominal"]["unfolded"]["nominal"].Integral()
                
                
                histDict[sys]["unfolded"]["nominal"].Scale(3.0/norm)
                histDict[sys]["unfolded"]["up"].Scale(3.0/norm)
                histDict[sys]["unfolded"]["down"].Scale(3.0/norm)
                histDict[sys]["unfolded"]["error"].Scale((3.0/norm)**2)
                
                histDict[sys]["gen"]["nominal"]=rootFile.Get("gen")
                histDict[sys]["gen"]["nominal"].SetDirectory(0)
                histDict[sys]["gen"]["nominal"].Scale(3.0/histDict[sys]["gen"]["nominal"].Integral())
                
                rootFile.Close()
                
        elif sys == "generator":
            fileName=os.path.join(folder,prefix+sys+".root")
            if os.path.exists(fileName):
            
                histDict[sys]={"isShape":True,"unfolded":{},"input":{}, "gen":{}}
                rootFile=ROOT.TFile(fileName)
                histDict[sys]["input"]["up"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["up"].SetDirectory(0)
                
                histDict[sys]["unfolded"]["up"].Scale(3.0/norm)
                
                #symmetrize uncertainty
                histDict[sys]["input"]["down"]=histDict[sys]["input"]["up"].Clone()
                histDict[sys]["input"]["down"].SetDirectory(0)
                histDict[sys]["input"]["down"].Scale(-1.0)
                histDict[sys]["input"]["down"].Add(histDict["nominal"]["input"]["nominal"],2.0)
                histDict[sys]["unfolded"]["down"]=histDict[sys]["unfolded"]["up"].Clone()
                histDict[sys]["unfolded"]["down"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"].Scale(-1.0)
                histDict[sys]["unfolded"]["down"].Add(histDict["nominal"]["unfolded"]["nominal"],2.0)
                
                histDict[sys]["gen"]["nominal"]=rootFile.Get("gen")
                histDict[sys]["gen"]["nominal"].SetDirectory(0)
                rootFile.Close()
             
        else:
            
            upFileName=os.path.join(folder,prefix+sys+"__up.root")
            downFileName=os.path.join(folder,prefix+sys+"__down.root")
            if os.path.exists(upFileName) and os.path.exists(downFileName):
                histDict[sys]={"isShape":True,"unfolded":{},"input":{}, "gen":{}}

                rootFile=ROOT.TFile(upFileName)
                histDict[sys]["input"]["up"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["up"].SetDirectory(0)
                histDict[sys]["unfolded"]["up"].Scale(3.0/norm)
                
                histDict[sys]["gen"]["up"]=rootFile.Get("gen")
                histDict[sys]["gen"]["up"].SetDirectory(0)
                histDict[sys]["gen"]["up"].Scale(3.0/histDict[sys]["gen"]["up"].Integral())
                rootFile.Close()
                
                rootFile=ROOT.TFile(downFileName)
                histDict[sys]["input"]["down"]=rootFile.Get("substractedData")
                histDict[sys]["input"]["down"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"]=rootFile.Get("unfolded")
                histDict[sys]["unfolded"]["down"].SetDirectory(0)
                histDict[sys]["unfolded"]["down"].Scale(3.0/norm)
                
                histDict[sys]["gen"]["down"]=rootFile.Get("gen")
                histDict[sys]["gen"]["down"].SetDirectory(0)
                histDict[sys]["gen"]["down"].Scale(3.0/histDict[sys]["gen"]["down"].Integral())
                rootFile.Close()
    return histDict
    
folderTUnfold=os.path.join(os.getcwd(),"histos","bdt_Jun22_final_antitop","tunfold","0.45")
sysDict = readHistograms(folderTUnfold,sysNames)

nominalHist=sysDict["nominal"]["unfolded"]["nominal"]
    

def addColumn(sysDict):


    for ibin in range(6):
        tableHeader.append("bin "+str(ibin+1))
        totalSum2=0.0
        for row in range(len(tableRows)):
            if tableRows[row][0]=="line":
                continue
            sysName=tableRows[row][0]
            valueNominal = nominalHist.GetBinContent(ibin+1)

            valueUp = sysDict[sysName]["unfolded"]["up"].GetBinContent(ibin+1)
            valueDown = sysDict[sysName]["unfolded"]["down"].GetBinContent(ibin+1)
            
            value = max(math.fabs(valueUp-valueNominal),math.fabs(valueDown-valueNominal))
            

            totalSum2+=value**2
            if value*1000.0<0.5:
                #tableRows[row].append("$<10^{-3}$")
                tableRows[row].append("$<0.1$")
            
            else: #math.fabs(sysDict[sysName]["dup"]+sysDict[sysName]["ddown"])<0.01:
                tableRows[row].append("$%4.1f$" % (value*1000.0))

            '''
            else:
                tableRows[row].append("${}^{%+4.3f}_{%+4.3f}$" % (sysDict[sysName]["dup"],sysDict[sysName]["ddown"] ))
            '''

        tableTotal.append("$%4.1f$" % (math.sqrt(totalSum2)*1000.0))


addColumn(sysDict)
#addColumn("electron",readCSV("histos/scan/2bin/0.45","ele_"))
#addColumn("combined",readCSV("histos/scan/2bin/0.45","combined_"))

outFile = open("table.tex","w")
outFile.write("\\begin{tabular}[htc]{|r || r | r | r | r | r | r |}\n")
outFile.write("\\hline \n")

outFile.write("\\parbox[t][][c]{2.3cm}{\\centering "+tableHeader[1]+"} & ")
for h in tableHeader[2:-1]:
    outFile.write("\\parbox[t][][c]{2.3cm}{\\centering $\\delta\\big[$"+h+"$\\big]\\cdot 10^{3}$} & ")
outFile.write("\\parbox[t][][c]{2.3cm}{\\centering $\\delta\\big[$"+tableHeader[-1]+"$\\big]\\cdot 10^{3}$} \\\\[0.5cm] \n")

outFile.write("\\hline \n")
outFile.write("\\hline \n")
for row in range(len(tableRows)):
    if tableRows[row][0]=="line":
        outFile.write("\\hline\n")
        continue
        
    formattedRow=""
    for i in range(1,len(tableRows[row])-1):
        formattedRow+=tableRows[row][i]+ " \\hspace{0.1cm} & " #.replace(".","$&$")
    formattedRow+=tableRows[row][-1]+ " \\hspace{0.1cm} "
    formattedRow+=" \\\\ "#\\hline"
    
    outFile.write(formattedRow+"\n")

outFile.write("\\hline \n")
outFile.write("\\hline \n")
formattedRow=""
for i in range(1,len(tableTotal)-1):
    formattedRow+=tableTotal[i] + " \\hspace{0.1cm} & "#.replace(".","$&$")
formattedRow+=tableTotal[-1]+ " \\hspace{0.1cm} "
formattedRow+=" \\\\ "
outFile.write(formattedRow+"\n")
    
outFile.write("\\hline \n")
outFile.write("\\end{tabular}\n")
outFile.close()
