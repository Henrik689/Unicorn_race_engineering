# UDP client example
import socket
from string import strip
from binascii import hexlify, unhexlify
from time import sleep

req_status="12345678170800000000"
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
data=1234

while 1:
	print "sending data\n"
	client_socket.sendto(unhexlify(req_status)+"endoffile", ("192.168.2.219",21000))
	sleep(1);

client_socket.close()

