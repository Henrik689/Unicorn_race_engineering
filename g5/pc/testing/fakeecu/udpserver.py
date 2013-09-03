# UDP server example
from binascii import hexlify, unhexlify
import socket
import threading
from time import sleep


class SocketListen(threading.Thread):
	def __init__(self, value):
		threading.Thread.__init__(self)
		self.value=value
	def run(self):
		while 1:
			print "Waiting for data"
			data, address = server_socket.recvfrom(256)
			print "( " ,address[0], " " , address[1] , " ) said : ", hexlify(data)


class SocketSend(threading.Thread):
	def __init__(self, value):
	    threading.Thread.__init__(self)
	    self.value=value
	def run(self):
		while 1:
			print "Sent request string"
			server_socket.sendto(unhexlify(req_status)+"endoffile", ("192.168.1.1",31337))
			sleep(0.5)




req_status="12345678170800000000"

print"UDPServer Waiting for client on port 5000"
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(("192.168.2.219", 21000))

##while 1:
#	print "Sending request string"
	#server_socket.sendto(unhexlify(req_status),("192.168.1.1",31337))

#SocketSend(1).start();
SocketListen(2).start();


