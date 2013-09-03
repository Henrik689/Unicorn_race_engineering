# -*- coding: utf8 -*-

from threading import Thread
from time import sleep
import serial
import random
import time

# Tråd der holder styr paa indkommende data. Der oprettes en tråd for hver
# klient der er tilsluttet.
class server(Thread):
    def __init__(self,args):
            Thread.__init__(self)
            print args
            self.conn, self.addr, self.clients  = args
    def run(self):
        # Modtager slip og status for launch control
        #self.car.reqStat()
        while True:
            try:
                # Modtager data fra klient
                self.data = self.conn.recv(1024)
                if not self.data: break
                # Data splittes ved ',' for at kunne sende det videre til bilen
                self.tmp = self.data.split(',')
                print "-------------------------------------"
                print "Modtaget pakke {0} fra: {1}".format(self.tmp[0],\
                                                           self.addr[0])
                if self.tmp[0] == '2':
                    print "Slip: {0}, Controller: {1}".format(self.tmp[1],\
                                                              self.tmp[2])
                    # Data sendes til bilen
                    #self.car.setStat(int(self.tmp[1]), int(self.tmp[2]))
                print "-------------------------------------\n"
            except:
                break

        # Når klienten lukker forbindelsen fjernes denne fra listen og
        # forbindelsen fra serveren lukkes
        self.clients.removeClient(self.conn, self.addr)
        self.conn.close()
        
    def exit(self):
        self.conn.close()

# Denne klasse holder styr på de tilsluttede klienter
class clients():
    def __init__(self):
        self._clients = []

    # En klient tilføjes til listen over klienter
    def addClient(self, conn, addr):
        self._clients.append((conn, addr))
        print "Added client:", addr 
        print "New number of clients: {0}\n".format(len(self._clients))

    # En klient fjernes fra listen over klienter
    def removeClient(self, conn, addr):
        self._clients.remove((conn, addr))
        print "removed client:", addr 
        print "New number of clients: %d\n" % len(self._clients)

    # Returnerer listen over klienter
    def getClients(self):
        return self._clients

# En tråd der sender data fra serial-klassen videre til alle klienterne
class serialSendToClients(Thread):
    def __init__(self, cv, clients, data):
        Thread.__init__(self)
        self.clients = clients
        self.data = data
        self.cv = cv
        print "Serial Send"

    def run(self):
        while True:
            # Der ventes på set som kaldes i 'serial'
            self.cv.wait()
            self.cv.clear()
            # Data sendes til alle klienter
            for i in range(len(self.clients.getClients())):
                    self.clients.getClients()[i][0].send(self.serData)

    # Setter for serialData
    def setSerialData(self, serData):
        self.serData = serData

    # Getter for serialData
    def getSerialData(self):
        return self.serData
        
    serialData = property(getSerialData, setSerialData)

# Denne klasse oprettes som en tråd. Den henter data fra serielporten og gemmer
# det i 'serialData'
class serial(Thread):
    def __init__(self, cv, ser, senderThread):
        Thread.__init__(self)
        self.cv = cv
        self.ser = ser
        self.sender = senderThread
        print "Data"

    def run(self):
        #while True:
            # Der læses en byte ind fra serielporten
            #tmp = self.ser.read()
            #if 255 == ord(tmp):
            #    var1 = ord(self.ser.read())
            #    if var1 == 1:
            #        # 7 byte læses fra serielporten
            #        var2 = self.ser.read(7)
            #        self.sender.serialData = "1,{0},{1},{2},{3},{4},{5},{6}\n".\
            #                                format(ord(var2[0]),ord(var2[1]),\
            #                                        ord(var2[2]),ord(var2[3]),\
            #                                        ord(var2[4]),ord(var2[5]),\
            #                                        ord(var2[6]))
            #        # set kaldes for at indikere at den serielle data er klar
            #        # til at sende
            #        self.cv.set()
            #    elif var1 == 2:
		    # var2 = self.ser.read(2)
		    #print "fra bil {0},{1}".format(var2[0], var2[1])
            #        self.car.setSelfStat(ord(var2[0]), ord(var2[1]))
            #        self.sender.serialData = self.car.pack2()
            #        self.cv.set()
            #    else:
            #        print "forkert type"
            #self.sender.serialData = "1,-45,0,4,15,1,7,10\n"
            #self.cv.set()
            #time.sleep(0.1)
            #self.sender.serialData = "25,-20,3,5,56,5,8,11\n"
            #self.cv.set()
            #time.sleep(0.1)
            #self.sender.serialData = "50,0,7,6,67,55,9,12\n"
            #self.cv.set()
            #time.sleep(0.1)
            #self.sender.serialData = "75,5,11,7,115,80,10,13\n"
            #self.cv.set()
            #time.sleep(0.1)
            #self.sender.serialData = "100,45,0,8,120,120,11,8\n"
            #self.cv.set()
            #time.sleep(0.1)
            
            #else:
            #    print "no valid data"     

        f = open('data/LarsSlalom5.txt', 'r')

        j = 0
        for line in f:
                if j>1:
                    print line,
                    self.sender.serialData = line
                    self.cv.set()
                    time.sleep(0.1)
                j = j +1

        f.close()   
