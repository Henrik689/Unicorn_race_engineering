import threading
import time
import serial
import Queue
import struct
import data


'''
The objective for this class is to run a Thread which handles the serial communication.
This object should parse the different types of data and write these to a mysql database.
The communication should be robust to lost packages, by attempting to find start sequences 
in the datastring, if it is incomplete.

255, 123, 10, datatype,
datalength defined in datatype definition

'''



class parser(threading.Thread):
	def __init__(self,connectionname = None,speed=None,queue=None):
		threading.Thread.__init__(self)
		if(speed == None):
			speed = 115200
		if connectionname == None:
			connectionname = "/dev/tty.usbserial-A700eCo8"
		self.conn = serial.Serial(connectionname,speed,timeout=1)
		self.q = queue
		
	def close(self):
		#print self.conn
		self.conn.close()
		#print self.conn

	def getDataLength(self,dataType):
		length = data.datatype(ord(dataType)).datalength
		#print data.datatype(ord(dataType)).name
		return length
		
	def parse(self,dataType,readData):
		#print len(readData)
		x = 0.0
		#print readData
		if(len(readData) == 1):
			x = ord(readData[0])
		elif(len(readData) == 2):
			x = ord(readData[0])*(2**8)+ord(readData[1])
		converted = data.datatype(ord(dataType)).conv(x)
		if(self.q != None):
			self.q.put([ord(dataType),converted])
		else:
			print [ord(dataType),converted]
	
		
	def run(self):
		dataType = 0
		while self.conn.isOpen():
			
			if self.conn.inWaiting() > 0:
				tmp = self.conn.read()
				if ord(tmp) == 255:
					tmp = self.conn.read()
					if ord(tmp) == 123:
						tmp = self.conn.read()
						if ord(tmp) == 10:
							dataType = self.conn.read()
							length = self.getDataLength(dataType)/8
							data = []
							for i in range(length):
								data.append(self.conn.read())
							#print data
							self.parse(dataType,data)

			time.sleep(0.001)
	
		
		

if __name__ == '__main__':
	print "Starting thread"
	try:
		t = parser()
		t.daemon =True
		t.start()
		while True: time.sleep(1)
	except (KeyboardInterrupt, SystemExit):
		print "Interrupt"
		
	t.close()
	print "Quitting thread"
	