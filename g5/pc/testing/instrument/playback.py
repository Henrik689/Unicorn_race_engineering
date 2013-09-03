#! /usr/bin/python

from sys import argv
import socket
from time import sleep

host = "localhost"
port = 21567
buf = 1024
addr = (host,port)

UDPSock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

logPath="C:\\Documents and Settings\\Jeppe Holm\\Desktop\\unicorn\\datalogs\\"
logFile=open(logPath+"dataLog 08-08-08 2109.txt","r")
c=0
eof=False
while not eof:
  data=logFile.readline()
  if len(data)==0:
    eof=True
    continue
  if not (len(data)==229 or len(data)==230 or len(data)==233):
    continue

  UDPSock.sendto(data,addr)
  sleep(0.5)
