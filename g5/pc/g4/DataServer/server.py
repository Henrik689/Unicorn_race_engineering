# -*- coding: utf8 -*-

from threading import Thread
from time import sleep
import serial
import random
import time
import math

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

        print "Serial"

    def run(self):

        sold = 0
        while True:
            tmp = self.ser.read(1)
            if ord(tmp) == 123:
                tmp = self.ser.read(1)
                if ord(tmp) == 255:
                    tmp = self.ser.read(1)
                    if ord(tmp) == 10:
                        #print "data modtaget"
                        tmp = self.ser.read(35)
                if ord(tmp[0]) == 0:
                    Thetime = int(time.time())
                    rpm = int((ord(tmp[1])<<8)+ord(tmp[2])*0.9408)
                    speed = int(((ord(tmp[3])<<8)+ord(tmp[4]))*0.01*0.2+sold*0.8)
                    sold = speed
                    temp = int( ((ord(tmp[5])<<8)+ord(tmp[6]))*(-150.0/3840)+120)
                    air = int( ((ord(tmp[7])<<8)+ord(tmp[8]))*(-150.0/3840)+120)
                    batt = int( ((ord(tmp[9])<<8)+ord(tmp[10]))*(10./210))
                    pot = int((((ord(tmp[11])<<8)+ord(tmp[12])) -628)*100/2280)
                    Map = int( ((ord(tmp[13])<<8)+ord(tmp[14]))*(1.0/800/5*3000))
                    gearneautral = int(ord(tmp[15]))
                    olietryk = int(ord(tmp[16]))
                    water2_temp = int(197.6*math.exp(-0.015*ord(tmp[17])))
                    olie_temp = int(197.6*math.exp(-0.015*ord(tmp[18])))

                    gx = int((ord(tmp[19])<<8)+ord(tmp[20]))
                    if(gx>32768):
                        gx=-(0xffff-gx)
                    gx = int(gx / 1638.4)

                    gy = int((ord(tmp[21])<<8)+ord(tmp[22]))
                    if(gy>32768):
                        gy=-(0xffff-gy)
                    gy = int(gy / 1638.4)

                    gz= int((ord(tmp[23])<<8)+ord(tmp[24]))
                    if(gz>32768):
                        gz=-(0xffff-gz)
                    gz = int(gz / 1638.4)

                    lambda0 = (ord(tmp[25])<<8)+ord(tmp[26])
                    if(lambda0>32768):
                        lambda0=-(0xffff-lambda0)
                    lambda0 = int(70 - lambda0/64)

                    injectorTime = int((ord(tmp[27])<<8)+ord(tmp[28]))
                    ignitionTime = int( ((ord(tmp[29])<<8)+ord(tmp[30]))*(-0.75)+120)
                    loadeng = int( ((ord(tmp[31])<<8)+ord(tmp[32]))*(100.0/3840))
                    triggerErr = int((ord(tmp[33])<<8)+ord(tmp[34]))

                    #print "Rpm: {0:d} Speed: {1:d} Vand temp: {2:d} Luft temp: {3:d} Batt: {4:d} Pot: {5:d} Map: {6:d} Lambda: {7:f}, Gear Neutral: {8:d}, Olie Tryk: {9:d}, Køler Vand: {10:d}, Olie temp: {11:d}, g:{12:d},{13:d},{14:d}, L: {15:d}".format(int((ord(tmp[1])<<8)+ord(tmp[2])*0.9408), speed, temp, air, batt, pot, Map, Lambda, gearneautral, olietryk, water2_temp, olie_temp, gx,gy,gz,lambda2)
                    # Datastreng der sendes

                    streng = '{0:d},{1:d},{2:d},{3:d},{4:d},{5:d},{6:d},{7:d},{8:d},{9:d},{10:d},{11:d},{12:d},{13:d},{14:d},{15:d},{16:d},{17:d},{18:d}\n'.format(rpm, speed, temp, air, batt, pot, Map, gearneautral, olietryk, water2_temp, olie_temp, Thetime,gx,gy,lambda0,injectorTime,ignitionTime,loadeng,triggerErr)

                    print streng,                    

                    self.sender.serialData = streng
                    self.cv.set()

                elif ord(tmp[0]) == 1:
                    print "ECU communication error"
                else:
                    print "data error"
            else:
                print "no data"
