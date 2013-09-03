#### DEFINE MEAS OBJECTS FROM BYTE OFFSET, SCALING, OFFSET, TYPE AND LENGTH
## DEFINE VIRTMEAS OBJECTS AND THEIR "SPECIAL FUNCTIONS"

from classMeas import *
from math import sin, cos

#### ERROR DETECTION

def correct():
  if rpm.value()>15000 or rpm.value()<0:
    return False
  if waterTemp.value()>125 or waterTemp.value()<-10:
    return False
  if potmeter.value()>115 or potmeter.value()<-15:
    return False
  if airTemp.value()>125 or airTemp.value()<-20:
    return False
  if load.value()>110 or load.value()<-10:
    return False
  if mapSensor.value()>2000 or mapSensor.value()<500:
    return False
  return True

#### ROTATION OF THE G-MAESUREMENTS

def rot(xyzi):

#### G2
  xx=0.0715
  yy=-0.351
  zz=0

#### G3  
  xx=0.1095
  yy=-0.205
  zz=-0.43
  
### Jeppe
#  xx=0.2014
#  yy=-0.2625  
#  zz=0.4429
  
  x1=xyzi[0].value()
  y1=xyzi[1].value()
  z1=xyzi[2].value()
  i=xyzi[3]
  
  ## Rot. about x
  x2=x1
  y2=cos(xx)*y1-sin(xx)*z1
  z2=sin(xx)*y1+cos(xx)*z1
  
  ## Rot. about y
  x3=cos(yy)*x2+sin(yy)*z2
  y3=y2
  z3=-sin(yy)*x2+cos(yy)*z2

  ## Rot. about z
  x=cos(zz)*x3-sin(zz)*y3
  y=sin(zz)*x3+cos(zz)*y3
  z=z3
  
  g=[x,y,z]
  return g[i]

#### DECODE THE FLAGS
## FIXME: seperat value() og s()

def decFlags(flags):
  flags=flags[0].value()
  string=""
  if flags & 0x04:
    string+="Fuel "
  if flags & 0x01:
    string+="Main "
  if flags & 0x02:
    string+="KL15 "
  if flags & 0x20:
    string+="Error "
  return string

#### CALCULATE THE GEAR RATIO

def calcRatio(data):
  rpm=data[0].value()
  speed=100*data[1].value()
  if rpm==0:
    return 0
  return speed/rpm

def calcRatio2(data):
  rpm=data[0].retWord()/64
  speed=data[1].retWord()
  if rpm==0:
    return 0
  return speed/rpm

def calcGear(data):
  ratio=data[0].value()
  if ratio < 42:
    return 0
  elif ratio < 60:
    return 1
  elif ratio < 77:
    return 2
  elif ratio < 90:
    return 3
  elif ratio < 102:
    return 4
  elif ratio < 111:
    return 5
  else:
    return 6

#### ACTUAL MEASUREMENTS
## meas(n=0,g=1,o=0,t="w",l=2)
## byteOffset,gain,offset,type,length

fuelPressure=meas(0)
statusLapCount=meas(2)
statusInjSum=meas(4)
lastGearShift=meas(6)
motorOilTemp=meas(8)
oilPressure=meas(10)
statusTime=meas(12,0.000001,0,"l",4)
statusLapTime=meas(16,0.000001,0,"l",4)
gearOilTemp=meas(20)
statusTraction=meas(22)
statusGas=meas(24)
statusLambdaV2=meas(26)
statusCamTrigP1=meas(28)
statusCamTrigP2=meas(30)
statusChokerAdd=meas(32)
statusLambdaPWM=meas(34)

waterTemp=meas(46,-150.0/3840,120)
airTemp=meas(48,-150.0/3840,120)
potmeter=meas(50,0.0510,-31.4)
rpm=meas(54,0.9408)
triggerErr=meas(56)
camAngle1=meas(58,0.75,0,"i")
camAngle2=meas(60,0.75,0,"i")
roadSpeed=meas(62,0.01)
mapSensor=meas(64,1.0/800/5*3000)
batteryV=meas(66,1.0/210)
lambdaV=meas(68,-14.7*0.6/3840,0.7*14.7,"i")

load=meas(74,100.0/3840)

injectorTime=meas(78)
ignitionTime=meas(82,-0.75,120)
dwellTime=meas(84)

gX=meas(96,1.0/16384,t="i")
gY=meas(98,1.0/16384,t="i")
gZ=meas(100,1.0/16384,t="i")

motorFlags=meas(111,l=1)
outBits=meas(113,l=1)

time=meas(114)

#### VIRTUAL MEASUREMENTS
## virtMeas(inputs,function)
## FIXME: Det er lidt et hack, lav evt. seperate funktioner til value() og s()

x=virtMeas([gX,gY,gZ,0],rot)
y=virtMeas([gX,gY,gZ,1],rot)
z=virtMeas([gX,gY,gZ,2],rot)

flags=virtMeas([outBits],decFlags)

ratio=virtMeas([rpm,roadSpeed],calcRatio)
ratio2=virtMeas([rpm,roadSpeed],calcRatio2)
gear=virtMeas([ratio2],calcGear)