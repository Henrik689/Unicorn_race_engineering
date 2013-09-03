#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import serial
port = "/dev/ttyUSB" + sys.argv[1]
ser=serial.Serial(port,115200)
print "Port:", port

package_start_counter = 0
package_start = False
test = [123,255,10,4,51,32,25,32,123,255,10,25,123,10,123]
i = 0

valueID = list(enumerate ([
	('empty', 0, 0), 
	('fuelPressure', 16, 0),
	('statusLapCount', 16, 0),
	('statusInjSum', 16, 0),
	('lastGearShift', 16, 0),
	('motorOilTemp', 16, 0),
	('oilPressure', 16, 0),
	('statusTime', 16, 0),
	('statusLapTime', 16, 0),
	('gearOilTemp', 16, 0),
	('statusTraction', 16, 0),
	('statusGas', 16, 0),
	('statusLambdaV2', 16, 0),
	('statusCamTrigP1', 16, 0),
	('statusCamTrigP2', 16, 0),
	('statusChokerAdd', 16, 0),
	('statusLambdaPWM', 16, 0),
	('waterTemp', 16, 6),
	('airTemp', 16, 6),
	('potmeter', 16, 6),
	('rpm', 16, 0),
	('triggerErr', 16, 0),
	('camAngle1', 16, 0),
	('camAngle2', 16, 0),
	('roadSpeed', 16, 6),
	('mapSensor', 16, 6),
	('batteryV', 16, 6),
	('lambdaV', 16, 0),
	('load', 16, 0),
	('injectorTime', 16, 0),
	('ignitionTime', 16, 0),
	('dwellTime', 16, 0),
	('gX', 16, 0),
	('gY', 16, 0),
	('gZ', 16, 0),
	('motorFlags', 8, 0),
	('outBits', 8, 0),
	('time', 8, 0),
	('GearUp', 8, 0),
	('FWheelL', 16, 0),
	('FWheelR', 16, 0),
	('BWheelL', 16, 0),
	('BWheelR', 16, 0),
        ('N5ADC0', 16, 0),
        ('N5ADC1', 16, 0),
        ('N5ADC2', 16, 0),
        ('N5ADC3', 16, 0),
        ('N5ADC4', 16, 0),
        ('N5ADC5', 16, 0),
        ('N5ADC6', 16, 0),
        ('N5ADC7', 16, 0),
        ('debug', 16, 0),
	('ValueIdLength', 16, 0)]))

nextID = -1
IDErrors = 0
j = 0
while 1:
    tmp = ord(ser.read(1))
    #tmp = test[i]
    i += 1

    if (package_start == True):
        j += 1
        print "Package", j, ": IDErrors", IDErrors
        if (tmp < 0 or tmp >=valueID[-1][0]):
            IDErrors += 1
            nextID = -1
        else:
            print "ID:", "(", tmp,",",valueID[tmp][0],",",valueID[tmp][1][0],")"
            nextID = valueID[tmp][1][1]/8
    elif (nextID > 0):
        print "Value:", tmp
        nextID -= 1
    elif nextID == 0:
        if (tmp < 0 or tmp >=valueID[-1][0]):
            nextID = -1
        else:
            print "ID:", "(", tmp,",",valueID[tmp][0],",",valueID[tmp][1][0],")"
            nextID = valueID[tmp][1][1]/8

    package_start = False

#    print "Data: ", tmp
    if (package_start_counter == 0 and tmp == 255):
        print "Start 0"
        package_start_counter = 1
    elif (package_start_counter == 1 and tmp == 123):
        print "Start 1"
        package_start_counter = 2
    elif (package_start_counter == 2 and tmp == 10):
        print "Start 2"
        package_start_counter = 0
        package_start = True

        
