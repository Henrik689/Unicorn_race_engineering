#!/usr/bin/env python

#Gui for unicorn race engineering

import pygtk
pygtk.require('2.0')
import gtk, gobject
import threading, os

t1, t2 = 0, 0


from os import name
import socket

from binascii import hexlify, unhexlify
from fpformat import fix

from classMeas import *
from defineData import *

from time import sleep, time, strftime

nodemeas=0.0

#Create a socket to receive data
host = "192.168.2.219"
port = 21000
buf = 255
addr = (host,port)

UDPSock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
UDPSock.bind(addr)

dataLogPath="/mnt/lager/unicorn/datalog/"

fileName=dataLogPath + "dataLog " + strftime("%y-%m-%d %H%M") + ".txt"
print "Logging data to:\t" + fileName

class UpdateData (threading.Thread):
	def __init__(self, nUpobj):
		global upobj
		upobj=nUpobj
		threading.Thread.__init__ ( self )

	def run(self):
		logFile=open(fileName,"w")
		while 1:
			#Retreive data from the socket
			print "Retrieving data\n"
			data_buf = UDPSock.recv(buf)
			print "Data received"
			data=hexlify(data_buf)
			print data
			print data[1]+data[2]+data[3]+data[4]+data[5]+data[6]+data[7]+data[8]+data[9]+data[10]
			for i in range (1,8):
				if data[i]=='0':
					print "Part of data valid"
					data_valid=1
				else:
					data_valid=0
	
			#Decoding the received data set. 
			## meas(data,n=0,g=1,o=0,t="w",l=2)
			## data,byteOffset,gain,offset,type,length
			if data_valid == 1:
				waterTemp=meas(data,46,-150.0/3840,120).value()
				airTemp=meas(data,48,-150.0/3840,120).value()
				potmeter=meas(data,50,0.0510,-31.4).value()
				rpm=meas(data,54,0.9408).value()
				mapSensor=meas(data,64,1.0/800/5*3000).value()
				batteryV=meas(data,66,1.0/210).value()
				lambdaV=meas(data,68,-14.7*0.6/3840,0.7*14.7,"i").value()
				if data[237]=='0':
					if data[235]=='0':
						nodemeas=data[231]+data[233]
					else:
						nodemeas=data[231]+data[233]+data[235]
				else:
					nodemeas=data[231]+data[233]+data[235]+data[237]
				
				#nodemeas=float(nodemeas)*5.0
				#nodemeas=nodemeas/1.024	
				ecutime=meas(data,12,0.000001,0,"l",4).value()
				tp=meas(data,50,0.0510,-31.4).value()
	
				##Update the label in the GUI
				upobj.rpm_label.set_text(str(fix(rpm,0)))
				upobj.clt_label.set_text(str(fix(waterTemp,1)))
				upobj.iat_label.set_text(str(fix(airTemp,1)))
				upobj.time_label.set_text(str(fix(ecutime,1)))
				upobj.vbat_label.set_text(str(fix(batteryV,1)))
				upobj.afr_label.set_text(str(fix(lambdaV,1)))
				upobj.map_label.set_text(str(fix(mapSensor,1)))
				upobj.nodemeas_label.set_text(str(fix(nodemeas,1)))
				upobj.tp_label.set_text(str(fix(tp,1)))
				t=hex(int(strftime("%H%M")))[2:]
  				t="0000"[0:4-len(t)]+t
				#  t=t[2:]+t[:2]
  				data=data+t
  				logFile.write(data+"\n")

			else:
				print "Data invalid"

			#print "Runs: " + str(runs)
			#runs=runs+1
			sleep(0.1)

class MainWindow(threading.Thread):

	def __init__(self):
		gtk.gdk.threads_init()
		self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		self.window.connect("destroy", lambda w: gtk.main_quit())
					
		self.window.set_title("Unicorn Race Engineering")
	
		hbox=gtk.HBox(False,5)
		self.window.add(hbox)
		

		vbox=gtk.VBox(False, 5)
		vbox.set_border_width(10)
		hbox.pack_start(vbox, False, False, 0)
		
		#Create a 10x10 table, used to display engineinformation
		engine_table = gtk.Table(2, 11, False)
		engine_table.set_col_spacings(20)
    
    		
		engine_frame = gtk.Frame("Motor status")
		engine_frame.add(engine_table)

		label = gtk.Label("RPM")
		engine_table.attach(label,1,2,1,2)
		self.rpm_label = gtk.Label("0")
		engine_table.attach(self.rpm_label,2,3,1,2)
		
		label = gtk.Label("Coolant temperatur")
		engine_table.attach(label,1,2,2,3)
		self.clt_label = gtk.Label("0")
		engine_table.attach(self.clt_label,2,3,2,3)

		label = gtk.Label("Intake air temperature")
		engine_table.attach(label,1,2,3,4)
		self.iat_label = gtk.Label("0")
		engine_table.attach(self.iat_label,2,3,3,4)
		
		label = gtk.Label("Intake manifold pressure")
		engine_table.attach(label,1,2,4,5)
		self.map_label = gtk.Label("0")
		engine_table.attach(self.map_label,2,3,4,5)
		
		label = gtk.Label("Oil pressure")
		engine_table.attach(label,1,2,5,6)
		self.oilpres_label = gtk.Label("0")
		engine_table.attach(self.oilpres_label,2,3,5,6)
		
		label = gtk.Label("AFR")
		engine_table.attach(label,1,2,6,7)
		self.afr_label = gtk.Label("0")
		engine_table.attach(self.afr_label,2,3,6,7)

		label = gtk.Label("Speed (km/t)")
		engine_table.attach(label,1,2,7,8)
		self.speed_label = gtk.Label("0")
		engine_table.attach(self.speed_label,2,3,7,8)
		
		label = gtk.Label("Trigger error")
		engine_table.attach(label,1,2,8,9)
		self.trigerror_label = gtk.Label("0")
		engine_table.attach(self.trigerror_label,2,3,8,9)
		
		label = gtk.Label("ECU time:")
		engine_table.attach(label,1,2,9,10)
		self.time_label = gtk.Label("0")
		engine_table.attach(self.time_label,2,3,9,10)
		
		label = gtk.Label("Throttle position:")
		engine_table.attach(label,1,2,10,11)
		self.tp_label = gtk.Label("0")
		engine_table.attach(self.tp_label,2,3,10,11)

		#Pack the engine information in a frame
		vbox.pack_start(engine_frame, False, False, 0)
		
		
		#New vobx for other information
		vbox=gtk.VBox(False, 5)
		vbox.set_border_width(10)
		hbox.pack_start(vbox, False, False, 0)
		
		misc_table = gtk.Table(2, 10, False)
		misc_table.set_col_spacings(20)
    		
		misc_frame = gtk.Frame("Misc information")
		misc_frame.add(misc_table)
		
		
		label = gtk.Label("Battery voltage:")
		misc_table.attach(label,1,2,9,10)
		self.vbat_label = gtk.Label("0")
		misc_table.attach(self.vbat_label,2,3,9,10)
		
		label = gtk.Label("Node measurement:")
		misc_table.attach(label,1,2,11,12)
		self.nodemeas_label = gtk.Label("0")
		misc_table.attach(self.nodemeas_label,2,3,11,12)
	
		#Pack the misc information in a frame
		vbox.pack_start(misc_frame, False, False, 0)


		gtk.gdk.threads_enter()
		UpdateData(self).start()
		t2 = os.getpid()
		gtk.gdk.threads_leave()

		self.window.show_all()
		print t2	
		
		
def main():
	gtk.main()
	return 0


if __name__ == "__main__":
	MainWindow()
	main()
	t2 = os.getpid()
	print t2
	os.popen("kill -9 "+str(t2))
