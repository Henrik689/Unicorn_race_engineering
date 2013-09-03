#! /usr/bin/python

from binascii import hexlify, unhexlify
from re import findall
from sys import argv

if len(argv)==1:
  exit()

argv=argv[1:]

for log in argv:
  binFile=open(log,"rb")
  hexFile=open(log+".txt","w")
  
  hest=hexlify(binFile.read())
#  print hest
#  print
  #giraf=hest.split("00000000007")
  #                  00000000xxxx00000000xxxxxxxxxxxx0000000000000000000000000
  #                  00000000007000000000001f00c103040000000000000000000000000
  elefant=findall("00000000.{4}00000000.{12}0000000000000000000000000.{171}",hest)
  for mus in elefant:
    hexFile.write(mus + "\n")
#  print elefant
  
  #for mus in giraf:
  #  print "00000000000" + mus
  #  hexFile.write("00000000000" + mus + "\n")
