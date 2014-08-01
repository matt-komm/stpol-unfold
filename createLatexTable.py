import csv
import os
import math
import os.path

def formatUnc(unc):

    return "{0:6.5f}".format(unc)

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
        print f
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
for sys in ["statistical","generator","tchan_scale","ttjets_scale","wzjets_scale",
            "mass","wjets_shape","wjets_flavor","top_weight","wzjets_matching","ttjets_matching",
            "pdf","jes","jer","met","lepton_id","lepton_iso","lepton_trigger",
            "pu","btag_bc","btag_l","lepton_weight","qcd_range","qcd_cont",
            "fituncertainty","bias","limited_mc"]:
    outFile.write(sys.replace("_"," ")+" & ")
    if sysDictMu.has_key(sys):
        unc = math.fabs(sysDictMu[sys]["d"])
        outFile.write(formatUnc(unc)+" & ")
        totalMu+=unc**2
    else:
        outFile.write("- & ")
    if sysDictEle.has_key(sys):
        unc = math.fabs(sysDictEle[sys]["d"])
        outFile.write(formatUnc(unc)+" \\\\ \n")
        totalEle+=unc**2
    else:
        outFile.write("- \\\\ \n")
outFile.write("\hline \n")
outFile.write("total & "+formatUnc(math.sqrt(totalMu))+" & "+formatUnc(math.sqrt(totalEle))+" \\\\\n")
outFile.write("\hline \n")
outFile.write("\\end{tabular}\n")
outFile.close()
