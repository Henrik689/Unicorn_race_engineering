#!/usr/bin/env python
# coding: utf-8
import serial
from binascii import hexlify, unhexlify

leds = 16
maxRpm = 12000
rpmScale = 0.9408   # Faktor rpm fra ecu skal ganges med for at blive rigtig
rpmDataPos = 54     # Byte nummer i strengen hvor rmp står
rpmDataSize = 2     # Længden af rmp i byte

ledScale = maxRpm/(leds+1) # +1 da led 16 ellers først vil lyse ved rpm > maxRpm

ser = serial.Serial("/dev/ttyUSB0", 38400)

for i in range(1000):
    ser.write(unhexlify("12345678"+"17"+"0800000000"))  # Streng der skal sender til ecu for at få data.
                                                        # Som byte er det 18-52-86-120-23-8-0-0-0-0
    data = hexlify(ser.read(114))                       # Der sendes 114 byte fra ecu
    rpm = int((int(data[rpmDataPos*rpmDataSize:rpmDataPos*2+2*rpmDataSize],16)*rpmScale ))
    print "rmp = ", rpm
    print rpm/ledScale
