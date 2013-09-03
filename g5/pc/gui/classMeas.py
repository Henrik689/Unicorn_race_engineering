from fpformat import fix

class meas:
  
  dataOffset=0
  dataLength=2
  dataType="w"
  gain=1
  offset=0
  label=""
  upperL=100000
  lowerL=0
  decimals=1
  
  def __init__(self,nData,n=0,g=1,o=0,t="w",l=2):
    global data
    data = nData
    self.dataOffset=n*2
    self.dataLength=l*2
    self.dataType=t
    self.gain=g
    self.offset=o
    
  def value(self):
    if self.dataType=="w":
      tmp=self.retWord()
    elif self.dataType=="l":
      tmp=self.retWord()
    elif self.dataType=="i":
      tmp=self.retInt()
    elif self.dataType=="b":
      tmp=self.retByte()
    return tmp*self.gain+self.offset
    
  def retHex(self):
    tmp=data[self.dataOffset:self.dataOffset+self.dataLength]
#    print len(tmp)
    if len(tmp)<2:
      tmp="0"
    return tmp
    
  def retWord(self):
    return int(self.retHex(),16)
    
  def retLong(self):
    return int(self.retHex(),16)
    
  def retInt(self):
    tmp=int(self.retHex(),16)
    if tmp < 32768:
      return tmp
    else:
      return -(0xffff-tmp)
  
  def s(self):
    return str(fix(self.value(),self.decimals))[:6]
    
#  def scrollData(self):
#    global d
#    global data
#    data=dataM[d]
#    d=(d+1)%len(dataM)
  
  def updateData(self,nData):
    global data
    data=nData

#d=0

class virtMeas:
  label=""
  upperL=1000
  lowerL=-1000
  decimals=1
  
  def __init__(self,inputs,function):
    self.inputs=inputs
    self.function=function
    
  def s(self):
    return str(fix(self.value(),self.decimals))
    
  def value(self):
    return self.function(self.inputs)
