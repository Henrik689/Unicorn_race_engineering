#! /usr/bin/env python
# -*- coding: utf-8 -*-

# UDP server example
import socket
from binascii import hexlify, unhexlify

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_socket.bind(("192.168.2.219", 3000))

print"UDPServer Waiting for client on port 3000"

while 1:
	data, address = server_socket.recvfrom(256)
	print "( " ,address[0], " " , address[1] , " ) said : ", hexlify(data)
	#print data,
