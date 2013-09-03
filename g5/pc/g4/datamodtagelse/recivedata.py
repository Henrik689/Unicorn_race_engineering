#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial
import math
import time
ser=serial.Serial("/dev/ttyUSB0",115200)

f = open('/home/css/Desktop/UnicornLogData/logdata2.txt','w+')

sold = 0
speed = 0
temp = 0
air = 0
batt = 0
pot = 0
Map = 0
Lambda = 0
Lambda2 = 0
water2_temp = 0
gearneatral = 0
olietryk = 0
olie_temp = 0

streng = 'RPM, Speed, Water-temp (engine), Air-temp manifold, Batt voltage, Pot, MAP, Lambda, Gear Neautral, Oil-press, Water-temp, Oil-temp\n'
f.write(streng)
print "startet"

while 1:
    tmp = ser.read(1)
    if ord(tmp) == 123:
        tmp = ser.read(1)
        if ord(tmp) == 255:
            tmp = ser.read(1)
            if ord(tmp) == 10:
                #print "data modtaget"
                tmp = ser.read(20)
        if ord(tmp[0]) == 0:
            speed = int(((ord(tmp[3])<<8)+ord(tmp[4]))*0.01*0.2+sold*0.8)
            sold = speed
            temp = int( ((ord(tmp[5])<<8)+ord(tmp[6]))*(-150.0/3840)+120)
            air = int( ((ord(tmp[7])<<8)+ord(tmp[8]))*(-150.0/3840)+120)
            batt = int( ((ord(tmp[9])<<8)+ord(tmp[10]))*(1./210))
            #pot = int( ((ord(tmp[11])<<8)+ord(tmp[12]))*(0.0510)-31.4)
            pot = int((((ord(tmp[11])<<8)+ord(tmp[12])) -628)*100/2280)
            Map = int( ((ord(tmp[13])<<8)+ord(tmp[14]))*(1.0/800/5*3000))
            Lambda = float((ord(tmp[15])/2400.0 ) )
            gearneautral = int(ord(tmp[16]))
            olietryk = int(ord(tmp[17]))
            water2_temp = int(197.6*math.exp(-0.015*ord(tmp[18])))
            olie_temp = int(197.6*math.exp(-0.015*ord(tmp[19])))

            print "Rpm: {0:5d} Speed: {1:3d} Vand temp: {2:3d} Luft temp: {3:3d} Batt: {4:2d} Pot: {5:3d} Map: {6:4d} Lambda: {7:f}, Gear Neutral: {8:3d}, Olie Tryk: {9:3d}, Køler Vand: {10:3d}, Olie temp: {11:3d}".format(int((ord(tmp[1])<<8)+ord(tmp[2])*0.9408), speed, temp, air, batt, pot, Map, Lambda, gearneautral, olietryk, water2_temp, olie_temp)
            # Datastreng der skal gemmes
            streng = '{0:5d},{1:3d},{2:3d},{3:3d},{4:2d},{5:3d},{6:4d},{7:f},{8:3d},{9:3d},{10:3d},{11:3d},{12:f}\n'.format(int((ord(tmp[1])<<8)+ord(tmp[2])*0.9408), speed, temp, air, batt, pot, Map, Lambda, gearneautral, olietryk, water2_temp, olie_temp, time.time())

            f.write(streng)
        elif ord(tmp[0]) == 1:
            print "ECU communication error"
        else:
            print "data error"
    else:
        print "no data"
