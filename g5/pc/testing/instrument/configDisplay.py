#### CONFIGURATION FOR THE LIVE DECODER
## DEFINE LABELS AND DECIMALS, DEFAULTS TO "" AND 1
## UPPER AND LOWER LIMITS FOR "WARNINGS"
##
## THE AVAILABLE OBJECTS ARE DEFINED IN "defineData.py"

from defineData import *

statusTime.label="Time"

waterTemp.label="CLT"
waterTemp.upperL=105
waterTemp.lowerL=75

roadSpeed.label="Speed"

rpm.label="RPM"
rpm.decimals=0
rpm.upperL=10000
rpm.lowerL=5000

statusLapTime.label="Lap T"
statusLapCount.label="Lap #"
statusLapCount.decimals=0

potmeter.label="TP"
potmeter.lowerL=-1
#gX.label="X"
#gX.upperL=1.5
#gX.lowerL=-1.5
#gX.decimals=3

#gY.label="Y"
#gY.upperL=1.5
#gY.lowerL=-1.5
#gY.decimals=3

#gZ.label="Z"
#gZ.upperL=1.5
#gZ.lowerL=0.5
#gZ.decimals=3

triggerErr.label="Synch"
triggerErr.decimals=0

airTemp.label="IAT"
mapSensor.label="MAP"
mapSensor.decimals=0
lambdaV.label="lambda"
lambdaV.decimals=0
load.label="Load"

batteryV.label="Bat"

x.label="X"
x.upperL=1.5
x.lowerL=-1.5
x.decimals=2

y.label="Y"
y.upperL=1.5
y.lowerL=-1.5
y.decimals=2

z.label="Z"
z.upperL=1.5
z.lowerL=0.5
z.decimals=2

gear.label="Gear"
gear.decimals=0

ratio.label="Ratio"
oilPressure.label="OilP"

time.label="Clock"
time.decimals=0

#### APPEND THE WANTED MODULES TO THE DISPLAY LIST

display=[]
display.append(statusTime)
display.append(waterTemp)
display.append(rpm)
display.append(roadSpeed)
#display.append(statusLapCount)
#display.append(statusLapTime)
display.append(potmeter)
display.append(x)
display.append(y)
display.append(z)
display.append(airTemp)
display.append(mapSensor)
display.append(triggerErr)
#display.append(lambdaV)
#display.append(load)
display.append(batteryV)
#display.append(ratio2)
display.append(gear)
display.append(time)
#display.append(oilPressure)

