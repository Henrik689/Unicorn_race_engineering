#! /usr/bin/python

#### FARTSTRUP ECU "EMULATOR"
## ONLY EMULATES VARYING RPM
## THE DATA PACKAGE IS NOT "AUTHENTIC"

import serial
from binascii import hexlify, unhexlify
from os import name

#### SERIAL PORT CONFIGURATION
linTTYs="/dev/ttyS0"
winTTYs="COM1"
##############################

print "Simulating Fartstrup ECU."

if name=="posix":
  print "Linux (I think), using " + linTTYs
  tty=serial.Serial(linTTYs, 19200)
else:
  print "Windows (or what?), using " + winTTYs
  tty=serial.Serial(winTTYs, 19200)

replyA=""

rpmH="aa"
rpmL="55"

int("0352",16)

rpm=0;

for i in range(0,54):
  replyA+="00"

logPath="C:\\Documents and Settings\\Jeppe Holm\\Desktop\\unicorn\\datalogs\\"
logFile=open(logPath+"dataLog 08-08-20 1353.txt","r")
c=0
eof=False
while not eof:
  data=logFile.readline()
#  print len(data)
  if len(data)==0:
    eof=True
    continue
  if not (len(data)==229 or len(data)==230 or len(data)==233):
    continue

#  while 1:
  
#    rpm=(rpm+100)%8000
#    tmp=hex(rpm+4000)[2:]
#    rpmHEX="0000"[0:4-len(tmp)]+tmp
  
#    dat=unhexlify(replyA+rpmHEX+replyA+"0000FFFF")
  dat=unhexlify(data[0:228])
  reqRAW=tty.read(10)
  reqHEX=hexlify(reqRAW)
#  print reqHEX
  tty.flushInput()
  if reqHEX[17:19]=="00":
    tty.write(dat)
  else:
    tty.flushInput()
