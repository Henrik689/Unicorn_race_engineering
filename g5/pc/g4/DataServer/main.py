#! /usr/bin/env python
# -*- coding: utf8 -*-

# Dette er et serverprogram der bruges som mellemled mellem en seriel enhed
# og en eller flere klienter. Forbindelsen til klienterne foregår over TCP.
#
# Programmet er udviklet af gruppe 505 på Aalborg Universitet år 2009


import socket
import server
import threading
from time import sleep
import serial


# Opsætning af serielporten. ser.port er afhængig af det styresystem
# programmet køres i. I Linux systemer gives porten som '/dev/ttyUSB0' og i
# Windows systemer angives porten blot med et nummer. Nummer 5 angiver at
# COM6 bruges.

ser = serial.Serial()
ser.port = '/dev/ttyUSB0'
#ser.port = 5
ser.baudrate = 115200
ser.open()


# Der oprettes en socket server som tillader klienter at oprette forbindelse
HOST = ''
PORT = 9999
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)


# Der oprettes henvisninger til klasser så de kan bruges som argumenter til
# andre metoder
clients = server.clients()
cv = threading.Event()


# En tråd til at sende seriel data til klienterne oprettes
sender = server.serialSendToClients(cv, clients, "Consumer",)
sender.setDaemon(True)
sender.start()


# En tråd til at modtage seriel data fra bilen oprettes
serial = server.serial(cv, ser, sender)
serial.setDaemon(True)
serial.start()


print "Server started (Port: %d)," %PORT


while True:
    # Der oprettes en tråd for hver klient der tilslutter
    conn, addr = s.accept()
    clients.addClient(conn, addr)
    th = server.server(args=(conn, addr, clients))
    th.setDaemon(True)
    th.start()

