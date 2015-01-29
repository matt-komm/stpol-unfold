import csv
import os
import math
import os.path

def formatUnc(unc):

    return "{0:5.4f}".format(unc)
    
tableHeader=["","systematic"]
tableTotal=["","total uncertainty"]
    
tableRows=[
    ["stat","statistical"],
    ["generator","signal modeling"],
    ["tchan_scale","t-channel scale"],
    ["ttjets_scale","\\tt+jets scale"],
    ["wzjets_scale","W+jets scale"],
    ["mass","top quark mass"],
    ["wjets_shape","W+jets shape"],
    ["wjets_flavour_heavy","W+jets heavy flavour"],
    ["wjets_flavour_light","W+jets light flavour"],
    ["top_weight","top pT modeling"],
    ["wzjets_matching","W+jets matching"],
    ["ttjets_matching","\\tt+jets matching"],
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
    ["fiterror","ML-fit uncertainty"],
    ["bias","unfolding bias"],
    ["mcstat","limited MC"]
]
    
def readCSV(folder=os.getcwd(),match="mu_"):
    sysDict={}
    for f in os.listdir(os.path.join(os.getcwd(),folder)):
        if f.startswith(match) and f.endswith(".csv"):
            inFile = open(os.path.join(os.getcwd(),folder,f),"rb")
            csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
            result = csvFile.next()
            sysDict[result["syst"]]=result
            inFile.close()
    return sysDict
    

def addColumn(header,sysDict):
    tableHeader.append(header)
    totalSum2=0.0
    for row in range(len(tableRows)):
        sysName= tableRows[row][0]
        if sysDict.has_key(sysName):
            #value=(0.5*(math.fabs(sysDict[sysName]["dup"])+math.fabs(sysDict[sysName]["ddown"])))
            value=math.fabs(sysDict[sysName]["d"])
            totalSum2+=value**2
            print sysName, round(math.fabs(sysDict[sysName]["dup"]+sysDict[sysName]["ddown"]),5)
            if math.fabs(sysDict[sysName]["dup"]+sysDict[sysName]["ddown"])<0.01:
                tableRows[row].append("$\\pm%4.3f$" % (sysDict[sysName]["d"]))
            else:
                tableRows[row].append("${}^{%+4.3f}_{%+4.3f}$" % (sysDict[sysName]["dup"],sysDict[sysName]["ddown"] ))
        else:
            tableRows[row].append("-")
    tableTotal.append("$\\pm%4.3f$" % math.sqrt(totalSum2))
    print
    
addColumn("TUnfold muon",readCSV("histos/tunfold_smooth5","mu_"))
addColumn("TUnfold electron",readCSV("histos/tunfold_smooth5","ele_"))
addColumn("2-bin muon",readCSV("histos/2bin","mu_"))
addColumn("2-bin electron",readCSV("histos/2bin","ele_"))

outFile = open("table.tex","w")
outFile.write("\\begin{tabular}[htc]{|r || c | c || c | c |}\n")
outFile.write("\\hline \n")

outFile.write("systematic & $\delta A_{\mu}$ & $\delta A_{e}$  & $\delta A_{\mu}$ & $\delta A_{e}$ \\\\\n")
outFile.write("\\hline \n")
for row in range(len(tableRows)):
    formattedRow=tableRows[row][1]
    for i in range(2,len(tableRows[row])):
        formattedRow+=" & "+tableRows[row][i]
    formattedRow+=" \\\\ \\hline"
    
    outFile.write(formattedRow+"\n")

outFile.write("\\hline \n")
formattedRow=tableTotal[1]
for i in range(2,len(tableTotal)):
    formattedRow+=" & "+tableTotal[i]
formattedRow+=" \\\\ "
outFile.write(formattedRow+"\n")
    
outFile.write("\\hline \n")
outFile.write("\\end{tabular}\n")
outFile.close()
