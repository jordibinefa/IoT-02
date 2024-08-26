#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
mb_IoT_00.py
www.things.cat
Version without curses (avoiding graphical environment dependency). 20240822
Code written by Jordi Binefa. 20200502

./mb_IoT_00.py /dev/ttyUSB1 4800
"""

import sys
import serial
from time import sleep
from crc16c import calcByte,calcString,hexCRC,checkModbusWithCrc,readHex
from threading import Timer
# from key_getter import KeyGetter # Tan sols funciona amb entorns amb GUI. Està basat en curses
from kbhit import KBHit

class ModBus():
	def __init__(self, parent=None):
		self.tipus = 0
		if len( sys.argv ) == 2 :
			szPort = sys.argv[1]
			nBauds = 115200
		else :
			if len( sys.argv ) == 3 :
				szPort = sys.argv[1]
				nBauds = int(sys.argv[2])
			else :
				szPort = "/dev/ttyUSB0"
				nBauds = 4800

		port = szPort
		baudrate = nBauds
		
		print("Baudrate: %d at %s"%(baudrate,port))
		self.ser = 0
		self.t = 0
		
		try: 	
			self.ser = serial.Serial(
				port,
				baudrate,
				timeout=0,
				parity=serial.PARITY_NONE,
				stopbits=serial.STOPBITS_TWO,
				bytesize=serial.EIGHTBITS
			)
			
			self.nSegonsTimer = 0.2
			self.t = Timer(self.nSegonsTimer,self.temporitzador)
			self.t.start()
			
			# k = KeyGetter()
			k = KBHit()
			self.bLoop = True
			while self.bLoop == True:
				if k.kbhit():
					# cKey = repr(k.getch(False))
					# self.processaTecla(cKey[1])
					cK = k.getch()
					self.processaTecla(cK)
				sleep(0.1)
			self.bye()

		except serial.SerialException as e:
			print ("%s port is not connected" % szPort)
			sys.exit(-1)

		except KeyboardInterrupt:
			print ("\nAdéu!")
			self.t.cancel()		
			sys.exit(0)

	def writeCommand(self,command):
		hCRC = hexCRC(command)
		addedCRC="%c%c"%(hCRC>>8,hCRC&0xFF)
		szAddedCRC="%2X%2X"%(hCRC>>8,hCRC&0xFF)
		command += addedCRC
		#readHex(stMB)
		#print("")

		for cmd_byte in command:
			hex_byte = ("{0:02X}".format(ord(cmd_byte)))
			print (hex_byte,end='')
			self.ser.write(bytearray.fromhex(hex_byte))
		print("")		

	def lecturaMB(self,q):
		# print("%s" % q)
		if q == 'relaySet':
			print("{\nSending: Relay SET")
			stMB = "%c%c%c%c%c%c"%(0x07,0x05,0x00,0x04,0xFF,0x00)
			self.writeCommand(stMB)
		if q == 'relayReset':
			print("{\nSending: Relay RESET")
			stMB = "%c%c%c%c%c%c"%(0x07,0x05,0x00,0x04,0x00,0x00)
			self.writeCommand(stMB)
		if q == 'input':
			print("{\nSending: Reading 4 coils")
			stMB = "%c%c%c%c%c%c"%(0x07,0x02,0x00,0x00,0x00,0x04)
			self.writeCommand(stMB)
		if q == 'temperature':
			print("{\nSending: Reading temperature")
			stMB = "%c%c%c%c%c%c"%(0x07,0x03,0x00,0x00,0x00,0x01)
			self.writeCommand(stMB)
		if q == 'relativeHumidity':
			print("{\nSending: Reading relative humidity")
			stMB = "%c%c%c%c%c%c"%(0x07,0x03,0x00,0x01,0x00,0x01)
			self.writeCommand(stMB)
		if q == 'pressure':
			print("{\nSending: Reading pressure")
			stMB = "%c%c%c%c%c%c"%(0x07,0x03,0x00,0x02,0x00,0x01)
			self.writeCommand(stMB)
		if q == 'altitude':
			print("{\nSending: Reading altitude")
			stMB = "%c%c%c%c%c%c"%(0x07,0x03,0x00,0x03,0x00,0x01)
			self.writeCommand(stMB)
		if q == 'voc':
			print("{\nSending: Reading VOC")
			stMB = "%c%c%c%c%c%c"%(0x07,0x03,0x00,0x04,0x00,0x01)
			self.writeCommand(stMB)

						
	def processaTecla(self,c):
		if c == 'q' or c == 'Q':
			self.bLoop = False
			return
		if c == 's' or c == 'S':
			self.tipus = 10
			self.lecturaMB('relaySet')
		if c == 'r' or c == 'R':
			self.tipus = 10
			self.lecturaMB('relayReset')
		if c == 'i' or c == 'I':
			self.tipus = 11
			self.lecturaMB('input')
		if c == 't' or c == 'T':
			self.tipus = 12
			self.lecturaMB('temperature')
		if c == 'h' or c == 'H':
			self.tipus = 13
			self.lecturaMB('relativeHumidity')
		if c == 'p' or c == 'P':
			self.tipus = 14
			self.lecturaMB('pressure')
		if c == 'a' or c == 'A':
			self.tipus = 15
			self.lecturaMB('altitude')		
		if c == 'v' or c == 'V':
			self.tipus = 16
			self.lecturaMB('voc')


	def processa(self,sz,nQ):
		#self.bConn = 0
		#print ("Rebut: %s"%sz)
		print ("He rebut %d bytes" % nQ)
		#readHex(sz)
		for b in sz:
			hex_byte = ("{0:02X}".format(b))
			print (hex_byte,end='')		
		print("")
		szByH = ("{0:02X}".format(sz[3]))
		szByL = ("{0:02X}".format(sz[4]))
		szWord = szByH + szByL
		nV = int(szWord,16)
		if self.tipus == 10:
			szByH = ("{0:02X}".format(sz[4]))
			szByL = ("{0:02X}".format(sz[5]))
			szWord = szByH + szByL
			print("Received: Relay %s" % ("SET" if szWord == "FF00" else "RESET") )
		elif self.tipus == 11:
			szBy = ("{0:02X}".format(sz[3]))
			print("Received: Input %s" % szBy )
			print("IO0%spressed, I34%spressed, I35%spressed and relay is%sset" % ((" not " if sz[3] & 0x01 else " "),(" not " if sz[3] & 0x02 else " "),(" not " if sz[3] & 0x04 else " "),(" " if sz[3] & 0x08 else " re")))
		elif self.tipus == 12:
			szByH = ("{0:02X}".format(sz[3]))
			szByL = ("{0:02X}".format(sz[4]))
			szWord = szByH + szByL
			n = int(szWord,16)
			if n > 2**15:
				n = -1*(2**16-n)			
			print("T: %.2f ºC" % (n / 100))
		elif self.tipus == 13:
			szByH = ("{0:02X}".format(sz[3]))
			szByL = ("{0:02X}".format(sz[4]))
			szWord = szByH + szByL
			print("RH: %.2f %%" % (int(szWord,16) / 100))
		elif self.tipus == 14:
			szByH = ("{0:02X}".format(sz[3]))
			szByL = ("{0:02X}".format(sz[4]))
			szWord = szByH + szByL
			print("P: %d hPa" % (int(szWord,16)))
		elif self.tipus == 15:
			szByH = ("{0:02X}".format(sz[3]))
			szByL = ("{0:02X}".format(sz[4]))
			szWord = szByH + szByL
			mask = ( 1 << 16 ) - 1
			n = int(szWord,16)
			if n > 2**15:
				n = -1*(2**16-n)
			print("A: %.2f m" % (n / 100))	
		elif self.tipus == 16:
			szByH = ("{0:02X}".format(sz[3]))
			szByL = ("{0:02X}".format(sz[4]))
			szWord = szByH + szByL
			print("VOC: %.3f KiloOhms" % (int(szWord,16) / 1000))
		else:
			print("Dada: %s: %d"%(szWord,nV))

	def bye(self):
		if self.t:
			self.t.cancel()	
		if self.ser:
			self.ser.close()
		
	def __del__(self):
		self.bye()
					
	def temporitzador(self):
		data = self.ser.read(1)
		n = self.ser.inWaiting()
		if n:
			n = 1 + n
			data = data + self.ser.read(n)
			# print("n: %d" % n)
		if len(data):
			self.processa(data,n)
			print("} ----")
			
		self.t = Timer(self.nSegonsTimer,self.temporitzador)
		self.t.start()
			
if __name__ == '__main__':
	print("s: Relay SET, r: Relay RESET, i: Read inputs, t: temperature, h: relative humidity, p: pressure, a: altitude, v: VOC and q: quit")

	mb = ModBus()
