from blue import BLUE

import os,csv

sysNames=[
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
    
sysDict1 = readCSV("histos/2bin","mu_")
sysDict2 = readCSV("histos/2bin","ele_")

independent=["stat","lepton_id","lepton_iso","lepton_trigger","qcd_yield","qcd_antiiso","mcstat"]

blue = BLUE()
blue.AddMeasurement(0.4386, 0.4386)
# uncorrelated
# statistical
for sys in sysNames:
    if sys[0] in independent:
        blue.AddUncertainty(sysDict1[sys[0]]["d"],sysDict2[sys[0]]["d"],0.0)
    else:
        blue.AddUncertainty(sysDict1[sys[0]]["d"],sysDict2[sys[0]]["d"],1.0)
    
blue.Simple()

