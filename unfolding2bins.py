import ROOT
import numpy
import math
from optparse import OptionParser

#abstract distribution
class Distribution():
    def __init__(self):
        self._ntoys=None
        self._sampledMeans=None
        self._sampledCovariance=None
        
    def getMean(self,index):
        return self.getMeans()[index]
        
    def getMeanSampled(self,index,ntoys=10000):
        return self.getMeansSampled(ntoys)[index]
        
    def getMeans(self):
        raise NotImplemented
        
    def getMeansSampled(self,ntoys=10000):
        if self._ntoys!=ntoys:
            self.generateSample(ntoys)
        return self._sampledMeans
        
    def getUncertainty(self,index):
        return math.sqrt(self.getVarianceMatrix()[index,index])
        
    def getUncertaintySampled(self,index,ntoys=10000):
        if self._ntoys!=ntoys:
            self.generateSample(ntoys)
        return math.sqrt(self._sampledCovariance[index,index])
        
    def getCorrelation(self,index1,index2):
        varianceMatrix=self.getVarianceMatrix()
        return varianceMatrix[index1,index2]/math.sqrt(varianceMatrix[index1,index1]*varianceMatrix[index2,index2]) if varianceMatrix[index1,index1]>0 else float('NaN')
    
    def getCorrelationSampled(self,index1,index2,ntoys=10000):
        if self._ntoys!=ntoys:
            self.generateSample(ntoys)
        return self._sampledCovariance[index1,index2]/math.sqrt(self._sampledCovariance[index1,index1]*self._sampledCovariance[index2,index2]) if self._sampledCovariance[index1,index1]>0 else float('NaN')
    
    def getVarianceMatrix(self):
        raise NotImplemented
        
    def sample(self):
        raise NotImplemented
        
    def generateSample(self,ntoys=10000):
        cov=numpy.zeros((2,2))
        mean=numpy.zeros(2)
        for i in range(ntoys):
            draw=self.sample()
            mean+=draw
            for i in range(2):
                for j in range(2):
                    cov[i,j]+=draw[i]*draw[j]
                
        mean=mean/ntoys
        cov=cov/ntoys
        self._sampledMeans=mean
        self._sampledCovariance=cov-numpy.array([[mean[0]*mean[0],mean[0]*mean[1]],[mean[1]*mean[0],mean[1]*mean[1]]])
        #set diagonal elements to 0 in case of imperfect numerical cancelation
        self._sampledCovariance[0][0]=self._sampledCovariance[0][0] if self._sampledCovariance[0][0]>0 else 0.0
        self._sampledCovariance[1][1]=self._sampledCovariance[0][0] if self._sampledCovariance[0][0]>0 else 0.0
# concrete distribution initialized with measured values and uncertainties
class DataDistribution(Distribution):
    
    def __init__(self):
        Distribution.__init__(self)
        self._content=numpy.zeros(2,dtype=numpy.float32)
        self._contentErrorPoisson=numpy.zeros(2,dtype=numpy.float32)
        self._contentErrorMC=numpy.zeros(2,dtype=numpy.float32)
        self._contentErrorYield=numpy.zeros((2,2),dtype=numpy.float32)
        self._usePoissonUncertainty=False
        self._useMCuncertainty=False
        self._yieldUncertainty=0.0
        
    @staticmethod
    def createFromHistogram(hist,usePoissonUncertainty=False,useMCuncertainty=False,yieldUncertainty=0.0):
        
        
        dist = DataDistribution()
        dist._usePoissonUncertainty=usePoissonUncertainty
        dist._useMCuncertainty=useMCuncertainty
        dist._yieldUncertainty=yieldUncertainty
        dist._content[0]=hist.GetBinContent(1)
        dist._content[1]=hist.GetBinContent(2)
        if (usePoissonUncertainty):
            dist._contentErrorPoisson[0]=math.sqrt(hist.GetBinContent(1))
            dist._contentErrorPoisson[1]=math.sqrt(hist.GetBinContent(2))
        if (useMCuncertainty):
            dist._contentErrorMC[0]=(hist.GetBinError(1))
            dist._contentErrorMC[1]=(hist.GetBinError(2))
         
        return dist
        
    def getMeans(self):
        return self._content
      
    def getVarianceMatrix(self):
        covarianceMatrix=numpy.zeros((2,2),dtype=numpy.float32)
        if (self._usePoissonUncertainty):
            covarianceMatrix[0][0]+=self._contentErrorPoisson[0]**2
            covarianceMatrix[1][1]+=self._contentErrorPoisson[1]**2
        if (self._useMCuncertainty):
            covarianceMatrix[0][0]+=self._contentErrorMC[0]**2
            covarianceMatrix[1][1]+=self._contentErrorMC[1]**2
        
        varBin1=(self._content[0]*self._yieldUncertainty)**2
        varBin2=(self._content[1]*self._yieldUncertainty)**2
        covarianceMatrix+=numpy.array([[varBin1,math.sqrt(varBin1*varBin2)],[math.sqrt(varBin1*varBin2),varBin2]],dtype=numpy.float32)
        return covarianceMatrix
        
    def sample(self):
        result = numpy.zeros(2)
        y=ROOT.gRandom.Gaus(1.0,self._yieldUncertainty)
        result[0] = self._content[0]
        result[1] = self._content[1]
        if self._usePoissonUncertainty:
            pass
            result[0]=ROOT.gRandom.Poisson(self._contentErrorPoisson[0]**2)
            result[1]=ROOT.gRandom.Poisson(self._contentErrorPoisson[1]**2)
        if self._useMCuncertainty:
            result[0]+=ROOT.gRandom.Gaus(0.0,self._contentErrorMC[0])
            result[1]+=ROOT.gRandom.Gaus(0.0,self._contentErrorMC[1])
            pass
        result[0]*=y
        result[1]*=y
        return result
        
class TransformationFunction():
    def __init__(self):
        pass
        
    def getJacobian(self,*args):
        raise NotImplemented
        
    def transform(self,*args):
        raise NotImplemented
        
# distribution defined by combining 2 other distributions
class CompoundDistribution(Distribution):
    def __init__(self,transformationFunction,*args):
        Distribution.__init__(self)
        self._transformationFunction=transformationFunction
        self._dists=args
        self._inputMeans=()
        for dist in self._dists:
            self._inputMeans+=(dist.getMeans(),)
        self._means=self._transformationFunction.transform(*self._inputMeans)
        
    def getMeans(self):
        return self._means
        
    def getVarianceMatrix(self):
        compoundInputVariances=numpy.zeros((len(self._dists)*2,len(self._dists)*2))
        for i in range(len(self._dists)):
            vec=numpy.zeros(len(self._dists))
            vec[i]=1.0
            matrix = numpy.outer(vec.T,vec)
            dist=self._dists[i]
            compoundInputVariances+=numpy.kron(matrix,dist.getVarianceMatrix())
        
        jacobian=self._transformationFunction.getJacobian(*self._inputMeans)
        return numpy.dot(jacobian,numpy.dot(compoundInputVariances,jacobian.T))
        
    def sample(self):
        sampleInput=()
        for dist in self._dists:
            sampleInput+=(dist.sample(),)
        return self._transformationFunction.transform(*sampleInput)


class Subtraction(TransformationFunction):
    def __init__(self):
        TransformationFunction.__init__(self)
        
    def getJacobian(self,*args):
        if len(args)!=2:
            raise NotImplemented
        return numpy.array([[1.0,0.0,-1.0,0.0],[0.0,1.0,0.0,-1.0]])
   
    def transform(self,*args):
        if len(args)!=2:
            raise NotImplemented
        return numpy.add(args[0],-args[1])

class Unfolding(TransformationFunction):
    def __init__(self,invertedResponseMatrix):
        TransformationFunction.__init__(self)
        self._invertedResponseMatrix=invertedResponseMatrix
        
    def getJacobian(self,*args):
        return self._invertedResponseMatrix

    def transform(self,*args):
        if (len(args)!=1):
            raise NotImplemented
        result = numpy.dot(self._invertedResponseMatrix,args[0])
        result = result 
        return result
        
class Asymmetry(TransformationFunction):
    def __init__(self):
        TransformationFunction.__init__(self)
        
    def getJacobian(self,*args):
        if len(args)!=1:
            return NotImplemented
        n1=args[0][0]
        n2=args[0][1]
        return numpy.array([[-1.0/(n1+n2)-(n2-n1)/(n1+n2)**2,
                             1.0/(n1+n2)-(n2-n1)/(n1+n2)**2],
                             [-1.0/(n1+n2)-(n2-n1)/(n1+n2)**2,
                             1.0/(n1+n2)-(n2-n1)/(n1+n2)**2]])

    def transform(self,*args):
        if len(args)!=1:
            return NotImplemented
        asymmetry=(args[0][1]-args[0][0])/(args[0][1]+args[0][0])
        #just return this double array so no problems with the distribution may arise
        return numpy.array([asymmetry,asymmetry])
        
def readResponseFromHistogramAndInvert(rootHist):
    rebinnedHist=rootHist.Rebin2D(rootHist.GetNbinsX()/2,rootHist.GetNbinsY()/2,"rebinned"+rootHist.GetName())
    response = numpy.zeros((2,2))
    efficiency= numpy.zeros(2)
    truth = numpy.zeros(2)
    measured = numpy.zeros(2)
    for igen in range(rebinnedHist.GetNbinsX()):
        for ireco in range(rebinnedHist.GetNbinsY()):
            response[igen,ireco]=rebinnedHist.GetBinContent(igen+1,ireco+1)/rebinnedHist.Integral(igen+1,igen+1,1,2)
        truth[igen]=rebinnedHist.Integral(igen+1,igen+1,0,2)
        measured[igen]=rebinnedHist.Integral(1,2,igen+1,igen+1)
        efficiency[igen]=1.0-rebinnedHist.GetBinContent(igen+1,0)/rebinnedHist.Integral(igen+1,igen+1,0,2)
    efficiencyMatrix=numpy.array([[efficiency[0],0.0],[0.0,efficiency[1]]])
    responseAndEfficiency=numpy.dot(efficiencyMatrix,response)
    invertedResponse=numpy.linalg.inv(responseAndEfficiency).T # not clear why it needs to be transposed but it works only this way
    '''
    #debug
    print "truth=",truth
    print "measured=",measured
    print "efficiency=",efficiency
    print "response=",response
    print "inverted=",invertedResponse
    print "unfolded=",numpy.dot(invertedResponse,measured)
    #'''
    #sanity check
    deltaMeasured = measured-numpy.dot(response,numpy.dot(efficiencyMatrix,truth))
    if (deltaMeasured[0]+deltaMeasured[1]>0.01):
        raise Exception("response matrix not properly defined")
    deltaTruth = truth-numpy.dot(invertedResponse,measured)
    if (deltaTruth[0]+deltaTruth[1]>0.01):
        raise Exception("inverted response matrix not properly defined")
    return invertedResponse    
    
    
if __name__=="__main__":
    '''
    
    #measured data with 1000 in first and 2000 in second bin, assume poisson uncertainties
    data = DataDistribution(1000,2000,True,0.0)
    
    #estimated background 1, 100 events in both bins, 10% yield uncertainty 
    bg1 = DataDistribution(100,100,False,0.1)
    
    #estimated background 2, 50 events in both bins, 20% yield uncertainty 
    bg2 = DataDistribution(50,50,False,0.2)
    
    #subtract background 1 from data
    sub1 = CompoundDistribution(Subtraction(),data,bg1)
    #subtract background 2
    measured = CompoundDistribution(Subtraction(),sub1,bg2)
    
    print "m1=",measured.getMean(0)," +- ",measured.getUncertainty(0)
    print "m1=",measured.getMeanSampled(0)," +- ",measured.getUncertaintySampled(0)," (toys)"
    print "m2=",measured.getMean(1)," +- ",measured.getUncertainty(1)
    print "m1=",measured.getMeanSampled(1)," +- ",measured.getUncertaintySampled(1)," (toys)"
    print "rho=",measured.getCorrelation(0,1)
    print "rho=",measured.getCorrelationSampled(0,1)," (toys)"
    print "-----------------------"
    
    #unfold the measured distribution
    testResponse=ROOT.TH2F("testresponse","",2,-1,1,2,-1,1)
    testResponse.SetBinContent(1 , 0 ,  208706.312561)
    testResponse.SetBinContent(1 , 1 ,  779.808402828)
    testResponse.SetBinContent(1 , 2 ,  343.157416447)
    testResponse.SetBinContent(2 , 0 ,  532897.071256)
    testResponse.SetBinContent(2 , 1 ,  305.399707367)
    testResponse.SetBinContent(2 , 2 ,  2683.5705404)
    invertedResponseMatrix=readResponseFromHistogramAndInvert(testResponse)
    unfolded= CompoundDistribution(Unfolding(invertedResponseMatrix),measured)
    print "u1=",unfolded.getMean(0)," +- ",unfolded.getUncertainty(0)
    print "u1=",unfolded.getMeanSampled(0)," +- ",unfolded.getUncertaintySampled(0)," (toys)"
    print "u2=",unfolded.getMean(1)," +- ",unfolded.getUncertainty(1)
    print "u2=",unfolded.getMeanSampled(1)," +- ",unfolded.getUncertaintySampled(1)," (toys)"
    print "rho=",unfolded.getCorrelation(0,1)
    print "rho=",unfolded.getCorrelationSampled(0,1)," (toys)"
    print "-----------------------"
    
    #calculate asymmetry
    asymmetry=CompoundDistribution(Asymmetry(),unfolded)
    print "A=",asymmetry.getMean(0)," +- ",asymmetry.getUncertainty(0)
    print "A=",asymmetry.getMeanSampled(0)," +- ",asymmetry.getUncertaintySampled(0)," (toys)"
    '''
