import csv
import os
import math
import os.path

sysDictMu={}
for f in os.listdir(os.getcwd()):
    if os.path.isfile(f) and f.startswith("mu_") and f.endswith(".csv"):
        inFile = open(f,"rb")
        print f
        csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
        result = csvFile.next()
        sysDictMu[result["syst"]]=result
        inFile.close()
        
sysDictEle={}
for f in os.listdir(os.getcwd()):
    if os.path.isfile(f) and f.startswith("ele_") and f.endswith(".csv"):
        inFile = open(f,"rb")
        csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
        result = csvFile.next()
        sysDictEle[result["syst"]]=result
        inFile.close()

outFile = open("table.tex","w")
outFile.write("\\begin{tabular}[htc]{|r || c | c|}\n")
outFile.write("\hline \n")
outFile.write("systematic & $\delta A_{\mu}$ & $\delta A_{e}$ \\\\\n")
outFile.write("\hline \n")
totalMu=0.0
totalEle=0.0
for sys in ["generator","scale_tchan","scale_ttjets","scale_wjets",
            "mass","wjets_shape","wjets_flavor","top_weight","matching",
            "pdf","jes","jer","met","lepton_id","lepton_iso","lepton_trigger",
            "pu","btag_bc","btag_l","lepton_weight","qcd_range","qcd_cont",
            "fituncertainty","bias","limited_mc","statistical"]:
    outFile.write(sys.replace("_"," ")+" & ")
    if sysDictMu.has_key(sys):
        unc = round(math.fabs(sysDictMu[sys]["up"]-sysDictMu[sys]["down"])*0.5,3)
        outFile.write(str(unc)+" & ")
        totalMu+=unc**2
    else:
        outFile.write("- & ")
    if sysDictEle.has_key(sys):
        unc = round(math.fabs(sysDictEle[sys]["up"]-sysDictEle[sys]["down"])*0.5,3)
        outFile.write(str(unc)+" \\\\ \n")
        totalEle+=unc**2
    else:
        outFile.write("- \\\\ \n")
outFile.write("\hline \n")
outFile.write("total & "+str(round(math.sqrt(totalMu),3))+" & "+str(round(math.sqrt(totalEle),3))+" \\\\\n")
outFile.write("\hline \n")
outFile.write("\\end{tabular}\n")
outFile.close()
