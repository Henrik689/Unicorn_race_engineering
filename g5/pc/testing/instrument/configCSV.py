#### CONFIGURATION FOR THE CSV GENERATOR
## DEFINE LABELS AND DECIMALS, DEFAULTS TO "" AND 1
##
## THE AVAILABLE OBJECTS ARE DEFINED IN "defineData.py"

from defineData import *

statusTime.label="Time"

waterTemp.label="CLT"
roadSpeed.label="Speed"
rpm.label="RPM"
rpm.decimals=0
statusLapTime.label="Lap T"
statusLapCount.label="Lap #"
statusLapCount.decimals=0

potmeter.label="TP"

triggerErr.label="Synch"
triggerErr.decimals=0

airTemp.label="IAT"
mapSensor.label="MAP"
mapSensor.decimals=0
lambdaV.label="lambda"
lambdaV.decimals=2
load.label="Load"

batteryV.label="Bat"

x.label="X"
x.decimals=2
y.label="Y"
y.decimals=2
z.label="Z"
z.decimals=2

flags.label="Flags"

ratio.label="Ratio"
ratio.decimals=3

ratio2.label="Ratio2"

gear.label="Gear"

oilPressure.label="OilP"

ignitionTime.label="ign"
ignitionTime.decimals=2

camAngle1.label="cam1"
camAngle2.label="cam2"

time.label="Clock"
time.decimals=0

#### APPEND THE WANTED MODULES TO THE DISPLAY LIST

display=[]
display.append(statusTime)
display.append(waterTemp)
display.append(rpm)
display.append(roadSpeed)
display.append(statusLapCount)
display.append(statusLapTime)
display.append(potmeter)
display.append(x)
display.append(y)
display.append(z)
display.append(airTemp)
display.append(mapSensor)
display.append(triggerErr)
display.append(lambdaV)
display.append(load)
display.append(batteryV)
display.append(ratio2)
display.append(gear)
display.append(oilPressure)
display.append(ignitionTime)
display.append(time)
#display.append(camAngle1)
#display.append(camAngle2)
#display.append(flags)
