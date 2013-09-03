#! /usr/bin/env python

#### GENERATE CSV FILE FROM DATALOG
## JUST DRAG A "dataLog yy-mm-dd hhmm.txt" TO THIS FILES ICON

from sys import argv
from configCSV import *
from time import sleep

###################################

def writeLabels(csvFile):
  for item in display:
#    csvFile.write(item.label + ",\t")
    csvFile.write(item.label + ", ")
#    print item.label
#    sleep(0.5)

  csvFile.write("\n")

###################################

def writeData(logFile,csvFile):
  c=0
  eof=False
  while not eof:
    data=logFile.readline()
#    print data
#    print len(data)

    if len(data)==0:
      eof=True
      continue
    if not (len(data)==229 or len(data)==230 or len(data)==233 or len(data)==232) :
      continue
    
    rpm.updateData(data)
    if not correct():
      continue
    for item in display:
#      csvFile.write(str(fix(item.value(),item.decimals)) + ",\t")
      csvFile.write(str(fix(item.value(),item.decimals)) + ", ")
      
    csvFile.write("\n")
#    sleep(0.1)
#    c+=1
    if c==10:
      eof=True

###################################

if len(argv)==1:
  exit()

argv=argv[1:]
####
#argv=["C:\Documents and Settings\\Jeppe Holm\\Desktop\\unicorn\\datalogs\\fartstrup\\fsg-acce.dat.txt"]

for log in argv:
  print log
  logFile=open(log,"r")
  csvFile=open(log + ".csv","w")
  writeLabels(csvFile)
  writeData(logFile,csvFile)
  logFile.close()
  csvFile.close()
