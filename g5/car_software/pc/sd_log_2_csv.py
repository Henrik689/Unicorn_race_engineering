#!/usr/bin/env python

import sys
import numpy
import csv
from pylab import *
from scipy import signal

fil = sys.argv[1]

f = open(fil, 'r')
rawData = numpy.fromfile(f, dtype=numpy.uint8)
#rawData = rawData[169065+121869+264+17442+1620+11001:]#+25647+41466:]
#rawData = rawData[291188+17440:]

class measurement:
    def __init__(self, HumanReadAbleName, idNum, dataLength=2, gain=0, offset=0, Min=0, Max=0, checkValues = False, signedConv=False, func=lambda x: x):
        self.name = HumanReadAbleName
        self.id = idNum
        self.length = dataLength
        self.gain = gain
        self.offset = offset
        self.min = Min
        self.max = Max
        self.checkValues = checkValues
        self.signedConv = signedConv
        self.func = func
        self.values = []

    def add_val(self, val):
        self.values.append(val)

    def convert_data(self, data):
        tmp = 0
        for val, shift in zip(data, range( (len(data)-1)*8,-1,-8) ):
            tmp += val<<shift

        if self.signedConv:
            if tmp > 2**15:
                tmp = -1*(2**16-1-tmp)

        tmp = self.func(tmp)

        return tmp*self.gain+self.offset

dataDefs = []
dataDefs.append( measurement('Fuel Press.', 1, 2, 1, 0) )
dataDefs.append( measurement('StatusLapCount', 2, 2, 1, 0) )
dataDefs.append( measurement('StatusInjSum', 3, 2, 1, 0) )
dataDefs.append( measurement('LastGearShift', 4, 2, 1, 0) )
dataDefs.append( measurement('MotorOilTemp', 5, 2, 1, 0) )
dataDefs.append( measurement('OilPressure', 6, 2, 1, 0) )
dataDefs.append( measurement('StatusTime', 7, 2, 1, 0) )
dataDefs.append( measurement('StatusLapTime', 8, 2, 1, 0) )
dataDefs.append( measurement('GearOilTemp', 9, 2, 1, 0) )
dataDefs.append( measurement('StatusTraction', 10, 2, 1, 0) )
dataDefs.append( measurement('StatusGas', 11, 2, 1, 0) )
dataDefs.append( measurement('StatusLambdaV2', 12, 2, 1, 0) )
dataDefs.append( measurement('StatusCamTrigP1', 13, 2, 1, 0) )
dataDefs.append( measurement('StatusCamTrigP2', 14, 2, 1, 0) )
dataDefs.append( measurement('StatusChokerAdd', 15, 2, 1, 0) )
dataDefs.append( measurement('StatusLambdaPWM', 16, 2, 1, 0) )
dataDefs.append( measurement('Motor water temp.', 17, 2, -150.0/3840, 120) )
dataDefs.append( measurement('Manifold air temp', 18, 2, -150.0/3840, 120) )
dataDefs.append( measurement('Potmeter', 19, 2, 1/26.9, -336) )
dataDefs.append( measurement('RPM', 20, 2, 0.9408, 0, 
    Min=0, Max = 15000, checkValues=False, signedConv=False) )
dataDefs.append( measurement('Trigger Error', 21, 2, 1, 0) )
dataDefs.append( measurement('Cam Angle 1', 22, 2, 1, 0) )
dataDefs.append( measurement('Cam Angle 2', 23, 2, 1, 0) )
dataDefs.append( measurement('RoadSpeed', 24, 2, 0.01, 0) )
dataDefs.append( measurement('mapSensor', 25, 2, 0.75, 0) )
dataDefs.append( measurement('battery', 26, 2, 1.0/210, 0,
    Min=0, Max = 20, checkValues=False, signedConv=False) )
dataDefs.append( measurement('lambda', 27, 2, 1, 0 ,
    signedConv=True, func=lambda x: ((70-x/64.)/100.) ) ) 
dataDefs.append( measurement('GX', 32, 2, 1/16384., 0., signedConv=True) )
dataDefs.append( measurement('GY', 33, 2, 1/16384., 0., signedConv=True) )
dataDefs.append( measurement('GZ', 34, 2, 1/16384., 0., signedConv=True) )
dataDefs.append( measurement('Time', 37, 1, 1., 0.) )
dataDefs.append( measurement('GearChange', 38, 1, 1, 0) )
dataDefs.append( measurement('FWheelL', 39, 2, 1, 0) )
dataDefs.append( measurement('FWheelR', 40, 2, 1, 0) )
dataDefs.append( measurement('BWheelL', 41, 2, 1, 0) )
dataDefs.append( measurement('BWheelR', 42, 2, 1, 0) )
dataDefs.append( measurement('GearBoard Temp', 52, 2, 1, 0) )
dataDefs.append( measurement('OilPress', 56, 2, 1, 0) )
dataDefs.append( measurement('WaterInlet temp.', 57, 2, 1, 0, 
    func=lambda x: 127.5*exp(-0.003286*x) ) )
dataDefs.append( measurement('WaterOutlet temp.', 58, 2, 1, 0, 
    func=lambda x: 127.5*exp(-0.003286*x)) )
dataDefs.append( measurement('GPS-sattelites', 59, 1, 1, 0) )
dataDefs.append( measurement('GPS-speed', 60, 2, 1, 0) )
dataDefs.append( measurement('GPS-time-h', 61, 2, 1, 0) )
dataDefs.append( measurement('GPS-time-m', 62, 2, 1, 0) )
dataDefs.append( measurement('GPS-time-s', 63, 2, 1, 0) )
dataDefs.append( measurement('GearNeutral', 64, 2, 1, 0) )
dataDefs.append( measurement('GearEst', 65, 2, 1, 0) )
dataDefs.append( measurement('Debug', 66, 2, 1, 0) )

i = 0
done = False
foundId = False
j = 0

filelen = len(rawData)

while not done:
    if j > len(dataDefs):
        print "File error"

    j = 0
    foundIf = False

    while j < len(dataDefs) and (not foundId) and (not done):
        datadef = dataDefs[j]
        j = j +1 
        #print rawData[i]
        #print "i: ", i
        if rawData[i] == datadef.id:
            foundIf = True
            if i%100 == 0:
                print "Parsed {0} %".format((i*100)/filelen)
            value = datadef.convert_data( numpy.array([rawData[i+1], rawData[i+2]]) )
            i = i + datadef.length+1
            if i > len(rawData)-2:
                done = True
            if datadef.checkValues == True:
                if value < datadef.min or value > datadef.max:
                    datadef.add_val(numpy.nan)
                else:
                    datadef.add_val( value )
            else:
                datadef.add_val( value )


parsedValues = {}
for item in dataDefs:
    parsedValues[item.name] = item

print "-----------------------------"
for key in parsedValues.keys():
    data = parsedValues[key]
    print "Name: " + data.name + "\nNumber of values: ", len(data.values)
    print "-----------------------------"

startTime = 0
ecuReqFreq = float(6.4)
endTime = len(parsedValues['battery'].values)/ecuReqFreq

figure(1)
plot( linspace(startTime, endTime, len(parsedValues['battery'].values)), 
        parsedValues['battery'].values, label=parsedValues['battery'].name)
b = signal.firwin(21, 0.1)
plot( linspace(startTime, endTime, len(parsedValues['battery'].values)), 
        signal.filtfilt(b, [1], parsedValues['battery'].values), 
        label=parsedValues['battery'].name + " filtered")

grid()
xlabel('Time [s]')
ylabel('Voltage [V]')
legend(loc=0)

figure(2)
title('Engine temperatures')
plot( linspace(startTime, endTime, len(parsedValues['Motor water temp.'].values)), 
        parsedValues['Motor water temp.'].values, label=parsedValues['Motor water temp.'].name)
plot( linspace(startTime, endTime, len(parsedValues['Manifold air temp'].values)), 
        parsedValues['Manifold air temp'].values, label=parsedValues['Manifold air temp'].name)
plot( linspace(startTime, endTime, len(parsedValues['WaterInlet temp.'].values)), 
        parsedValues['WaterInlet temp.'].values, label=parsedValues['WaterInlet temp.'].name)
plot( linspace(startTime, endTime, len(parsedValues['WaterOutlet temp.'].values)), 
        parsedValues['WaterOutlet temp.'].values, label=parsedValues['WaterOutlet temp.'].name)

grid()
xlabel('Time [s]')
ylabel('Temperature [Degrees C]')
legend(loc=0)

figure(3)
plot( linspace(startTime, endTime, len(parsedValues['RPM'].values)), 
        parsedValues['RPM'].values, label=parsedValues['RPM'].name)
b = signal.firwin(5, 0.5)
plot( linspace(startTime, endTime, len(parsedValues['RPM'].values)), 
        signal.filtfilt(b, [1], parsedValues['RPM'].values), label=parsedValues['RPM'].name+" filtered")

grid()
xlabel('Time [s]')
ylabel('Engine speed [RPM]')
legend(loc=0)

figure(4)
plot( linspace(startTime, endTime, len(parsedValues['lambda'].values)), 
        parsedValues['lambda'].values, label=parsedValues['lambda'].name)
b = signal.firwin(21, 0.1)
plot( linspace(startTime, endTime, len(parsedValues['lambda'].values)), 
        signal.filtfilt(b, [1], parsedValues['lambda'].values), 
        label=parsedValues['lambda'].name+" filtered")

grid()
xlabel('Time [s]')
ylabel('Value')
legend(loc=0)

figure(5)
title('Car acceleration')
plot( linspace(startTime, endTime, len(parsedValues['GX'].values)), 
        parsedValues['GX'].values, label=parsedValues['GX'].name)
plot( linspace(startTime, endTime, len(parsedValues['GY'].values)), 
        parsedValues['GY'].values, label=parsedValues['GY'].name)
plot( linspace(startTime, endTime, len(parsedValues['GZ'].values)), 
        parsedValues['GZ'].values, label=parsedValues['GZ'].name)

grid()
xlabel('Time [s]')
ylabel('Value')
legend(loc=0)

figure(6)
plot( linspace(startTime, endTime, len(parsedValues['GPS-speed'].values)), 
        parsedValues['GPS-speed'].values, label=parsedValues['GPS-speed'].name)

grid()
xlabel('Time [s]')
ylabel('Speed [km/h]')
legend(loc=0)

figure(7)
plot( linspace(startTime, endTime, len(parsedValues['OilPress'].values)), 
        parsedValues['OilPress'].values, label=parsedValues['OilPress'].name)

grid()
xlabel('Time [s]')
ylabel('Oil press [0 = No press, 1024 = Full press]')
legend(loc=0)


show()

"""short = 2**64
for datadef in dataDefs:
    if len(datadef.values) < short:
        short = len(datadef.values)
"""
"""
for datadef in dataDefs:
    datadef.values = datadef.values[:short]

res = numpy.reshape(dataDefs[0].values, (len(dataDefs[0].values), 1) )
names = []
names.append(dataDefs[0].name)

for i in range(1,len(dataDefs)):
    res = numpy.hstack( (res, numpy.reshape(dataDefs[i].values, (len(dataDefs[i].values), 1)) ))
    names.append(dataDefs[i].name)


with open(fil.split('.')[0] + '.csv', 'w+') as f:
    writer = csv.writer(f)
    writer.writerow(names)
    writer.writerows(res)
"""
