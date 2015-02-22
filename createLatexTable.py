import csv
import os
import math
import os.path

def formatUnc(unc):

    return "{0:4.3f}".format(unc)
    
tableHeader=["","systematic"]
tableTotal=["","total uncertainty"]
    
tableRows=[
['stat', "statistical"],
["line"],

#fitting
['fiterror', "ML-fit uncertainty"],
['diboson', "DiBoson fraction"],
['dyjets', "DrellYan fraction"],
['schan', "s-channel fraction"],
['twchan', "tW-channel fraction"],
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
['wjets_flavour_heavy', "\\wjets heavy flavor yield"],
['wjets_flavour_light', "\\wjets light flavor yield"],
['wjets_shape', "\\wjets shape reweighting"],
["line"],

#theory
['generator', "generator model"],
['mass', "top quark mass"],
['me_weight', "t-channel $Q^{2}$ scale"],
#['tchan_scale', "$Q^{2}$ scale t-channel"],
['ttjets_scale', "\\ttbar $Q^{2}$ scale"],
['ttjets_matching', "\\ttbar matching"],
['wzjets_scale', "\\wjets $Q^{2}$ scale"],
['wzjets_matching', "\\wjets matching"],
['pdf', "PDF"],
["line"],
['mcstat', "limited MC"],
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
            #print sysName, round(math.fabs(sysDict[sysName]["dup"]+sysDict[sysName]["ddown"]),5)
            if value<0.000005:
                #tableRows[row].append("$<10^{-3}$")
                tableRows[row].append("$<0.01$")
            
            else: #math.fabs(sysDict[sysName]["dup"]+sysDict[sysName]["ddown"])<0.01:
                tableRows[row].append("$%4.1f$" % (sysDict[sysName]["d"]*1000.0))
            '''
            else:
                tableRows[row].append("${}^{%+4.3f}_{%+4.3f}$" % (sysDict[sysName]["dup"],sysDict[sysName]["ddown"] ))
            '''
        else:
            tableRows[row].append("-")
    tableTotal.append("$%4.1f$" % (math.sqrt(totalSum2)*1000.0))
    
addColumn("muon",readCSV("histos/scan/tunfold/0.60","mu_"))
addColumn("electron",readCSV("histos/scan/tunfold/0.60","ele_"))
addColumn("combined",readCSV("histos/scan/tunfold/0.60","combined_"))
#addColumn("2-bin muon",readCSV("histos/2bin","mu_"))
#addColumn("2-bin electron",readCSV("histos/2bin","ele_"))

outFile = open("table.tex","w")
outFile.write("\\begin{tabular}[htc]{|r || r | r | r |}\n")
outFile.write("\\hline \n")

outFile.write(
    ''' 
        & $\delta A_{\mu}\\cdot 10^{3}$
        & $\delta A_{e}\\cdot 10^{3}$
        & $\delta A_\mathrm{comb.}\\cdot 10^{3}$
         \\\\\n
    '''
)
outFile.write("\\hline \n")
outFile.write("\\hline \n")
for row in range(len(tableRows)):
    if tableRows[row][0]=="line":
        outFile.write("\\hline\n")
        continue
    formattedRow=tableRows[row][1]
    for i in range(2,len(tableRows[row])):
        formattedRow+=" & "+tableRows[row][i]+ " \\hspace{0.1cm} " #.replace(".","$&$")
    formattedRow+=" \\\\ "#\\hline"
    
    outFile.write(formattedRow+"\n")

outFile.write("\\hline \n")
outFile.write("\\hline \n")
formattedRow=tableTotal[1]
for i in range(2,len(tableTotal)):
    formattedRow+=" & "+tableTotal[i] + " \\hspace{0.1cm} "#.replace(".","$&$")
formattedRow+=" \\\\ "
outFile.write(formattedRow+"\n")
    
outFile.write("\\hline \n")
outFile.write("\\end{tabular}\n")
outFile.close()
