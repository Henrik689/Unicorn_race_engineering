#! /usr/bin/python

import serial
from time import sleep

ttyName="COM5"
tty=serial.Serial(ttyName, 19200, timeout=0.35)

print "SENDING: AT^SETUP"
tty.write("\r\nAT^SETUP\r\n")
sleep(0.5)
tmp=tty.inWaiting()
#  print "RECEIVING: " + tty.read(tmp)
sleep(0.5)
print "SENDING: ATO"
tty.write("\r\nATO\r\n")