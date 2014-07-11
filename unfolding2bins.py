import ROOT
import numpy
import math

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
        return varianceMatrix[index1,index2]/math.sqrt(varianceMatrix[index1,index1]*varianceMatrix[index2,index2])
    
    def getCorrelationSampled(self,index1,index2,ntoys=10000):
        if self._ntoys!=ntoys:
            self.generateSample(ntoys)
        return self._sampledCovariance[index1,index2]/math.sqrt(self._sampledCovariance[index1,index1]*self._sampledCovariance[index2,index2])
    
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
        
# concrete distribution initialized with measured values and uncertainties
class DataDistribution(Distribution):
    
    def __init__(self,bin1,bin2,hasPoissonUncertainty=False,yieldUncertainty=0.0):
        Distribution.__init__(self)
        self._content=numpy.zeros(2)
        self._content[0]=bin1
        self._content[1]=bin2
        self.setPoissonUncertainty(hasPoissonUncertainty)
        self.setYieldUncertainty(yieldUncertainty)
        
    def setYieldUncertainty(self, yieldUncertainty):
        self._yieldUncertainty=yieldUncertainty
        varBin1=(self._content[0]*yieldUncertainty)**2
        varBin2=(self._content[1]*yieldUncertainty)**2
        self._yieldCovarianceMatrix=numpy.array([[varBin1,math.sqrt(varBin1*varBin2)],[math.sqrt(varBin1*varBin2),varBin2]],dtype=numpy.float32)
       
    def setPoissonUncertainty(self,hasPoissonUncertainty):
        self._hasPoissonUncertainty=hasPoissonUncertainty
        if (hasPoissonUncertainty):
            self._poissonCovarianceMatrix=numpy.array([[self._content[0],0.0],[0.0,self._content[1]]],dtype=numpy.float32)
        else:
            self._poissonCovarianceMatrix=numpy.array([[0.0,0.0],[0.0,0.0]],dtype=numpy.float32)
            
    def setBin(self,index,binContent):
        self._content[index]=binContent  
        
    def getMeans(self):
        return self._content
      
    def getVarianceMatrix(self):
        return self._poissonCovarianceMatrix+self._yieldCovarianceMatrix
        
    def sample(self):
        result = numpy.zeros(2)
        y=ROOT.gRandom.Gaus(1.0,self._yieldUncertainty)
        result[0] = self._content[0]
        result[1] = self._content[1]
        if self._hasPoissonUncertainty:
            result[0]=ROOT.gRandom.Poisson(result[0])
            result[1]=ROOT.gRandom.Poisson(result[1])
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
    def __init__(self,responseMatrix):
        TransformationFunction.__init__(self)
        self._responseMatrix=responseMatrix
        
    def getJacobian(self,*args):
        return self._responseMatrix

    def transform(self,*args):
        if (len(args)!=1):
            raise NotImplemented
        return numpy.dot(self._responseMatrix,args[0])
        
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
        
        
if __name__=="__main__":
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
    responseMatrix=numpy.array([[283.2637427,-32.23640403],[-92.50766353,210.2191294]])
    unfolded= CompoundDistribution(Unfolding(responseMatrix),measured)
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
    
