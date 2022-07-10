#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
mb_IoT_slave_00.py
www.things.cat
Code written by Jordi Binefa. 20220115

python mb_IoT_slave_00.py COM4 4800
"""

import sys
import serial
import random
from time import sleep
from crc16c import calcByte,calcString,hexCRC,checkModbusWithCrc,readHex
from threading import Timer
from key_getter import KeyGetter # Tan sols funciona amb entorns amb GUI. Està basat en curses

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
				szPort = "COM4"
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
			
			k = KeyGetter()
			self.bLoop = True
			while self.bLoop == True:
				if k.kbhit(False):  # A Windows s'ha afegit False
					cKey = repr(k.getch(False))
					self.processaTecla(cKey[1])
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

	def processaTecla(self,c):
		if c == 'q' or c == 'Q':
			self.bLoop = False
			return

	def bAskingTemperature(self,sz,nQ):
		listExpectedTemperatureFrame = [0x07,0x03,0x00,0x00,0x00,0x01]
		if nQ == 8:
			for by in range(nQ-2):
				if "{0:02X}".format(sz[by]) != "{0:02X}".format(listExpectedTemperatureFrame[by]):
					# print("It is not expected temperature frame")
					return False
		else:
			return False
		return True

	def processa(self,sz,nQ):
		#self.bConn = 0
		#print ("Rebut: %s"%sz)
		print ("{\nHe rebut %d bytes" % nQ)
		#readHex(sz)
		for b in sz:
			hex_byte = ("{0:02X}".format(b))
			print (hex_byte,end='')		
		print("")
		if self.bAskingTemperature(sz,nQ):
			nN = random.randint(2000, 3000)
			print("Receiving: Reading Temperature. Answering: %.2fºC"%(float(nN)/100))
			stMB = "%c%c%c%c%c"%(0x07,0x03,0x00,nN//256,nN%256)
			self.writeCommand(stMB)            

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
	print("Emulating temperature sensor slave (expecting frame: 070300000001846C). q: quit")

	mb = ModBus()
