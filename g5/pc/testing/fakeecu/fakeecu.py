#!/usr/bin/env python
# coding: utf-8

import sys
import serial
from string import strip
from binascii import hexlify, unhexlify
from time import sleep

rpmScale = 0.9408   # Faktor rpm fra ecu skal ganges med for at blive rigtig
rpmDataPos = 54     # Byte nummer i strengen hvor rmp står
rpmDataSize = 2     # Længden af rmp i byte

battScale = 1.0/210   # Faktor rpm fra ecu skal ganges med for at blive rigtig
battDataPos = 66     # Byte nummer i strengen hvor rmp står
battDataSize = 2     # Længden af rmp i byte

WTempScale = -150.0/3840   # Faktor rpm fra ecu skal ganges med for at blive rigtig
WTempOffset = 120
WTempDataPos = 46     # Byte nummer i strengen hvor rmp står
WTempDataSize = 2     # Længden af rmp i byte

port = "/dev/ttyUSB" + sys.argv[1]
tty=serial.Serial(port, 19200, timeout=1)
print "Port:", port
datalogfile=open("testdatalog.txt","r")

while 1:
	ttydata=hexlify(tty.read(10))
	#print ttydata
	if ttydata=="12345678170800000000":
		#print ttydata
		data=strip(datalogfile.readline(),"\n\r")
		if len(data)==0:
			print "EOF reached\n"
			datalogfile.close
			datalogfile=open("testdatalog.txt")
			print "File reopened\n"
			data=strip(datalogfile.readline(),"\n\r")
		tty.flushOutput()
		rpm = int((int(data[rpmDataPos*rpmDataSize:rpmDataPos*2+2*rpmDataSize],16)*rpmScale ))
		batt = float((int(data[battDataPos*battDataSize:battDataPos*2+2*battDataSize],16)*battScale ))
		WTemp = float((int(data[WTempDataPos*WTempDataSize:WTempDataPos*2+2*WTempDataSize],16)*WTempScale )+WTempOffset)
		print "rpm:", rpm, "batt:", batt, "WTemp:", WTemp  		
		tty.write(unhexlify(data))
		#print "Sent:"
		#print data
		tty.flushInput()

	else:
		#print "No data received\n"
		tty.flushInput()
		tty.flushOutput()

datalogfile.close
