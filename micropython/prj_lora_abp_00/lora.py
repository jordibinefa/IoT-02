# Based on https://github.com/ElektorLabs/210047-LoRa-with-the-Raspberry-Pi-Pico/tree/main/Software

import machine
import time
import utime
import ubinascii
import os

from micropython import const
from ulora import TTN, uLoRa

#Set the pinout for the RP2040
'''
LORA_SCK = const(6)
LORA_MOSI = const(7)
LORA_MISO = const(4)
LORA_CS = const(5)
LORA_IRQ = const(8)
LORA_RST = const(9)
'''
#Set the pinout for the IoT-02
LORA_SCK = const(5)
LORA_MOSI = const(25)
LORA_MISO = const(15)
LORA_CS = const(18)
LORA_IRQ = const(26) # <---- IRQ -> DIO0
LORA_RST = const(14)

#LoRa configuration
LORA_DATARATE = "SF9BW125"  # Choose from several available

#Enter the Data for the TTN access here
DEVADDR = bytearray([0x26, 0x0B, 0x71, 0x90])
# DEVADDR = bytearray([INSERT DATA HERE])
# static const u4_t DEVADDR = 0x260B7190 ;
NWKEY = bytearray([0x27, 0x92, 0xAA, 0x1E, 0x8A, 0xC5, 0x8D, 0x7F, 0x47, 0xBE, 0xBF, 0x3A, 0x5D, 0xAC, 0x3D, 0xC7])
# NWKEY = bytearray([INSERT DATA HERE])
# static const PROGMEM u1_t NWKSKEY[16] = { 0x27, 0x92, 0xAA, 0x1E, 0x8A, 0xC5, 0x8D, 0x7F, 0x47, 0xBE, 0xBF, 0x3A, 0x5D, 0xAC, 0x3D, 0xC7 };
APP = bytearray([0xE7, 0x76, 0xF4, 0x8E, 0xBB, 0xA1, 0xE3, 0xD7, 0xBF, 0xE8, 0xCD, 0xF2, 0x5D, 0x1A, 0x2C, 0xBA])
# APP = bytearray([INSERT DATA HERE])
# static const u1_t PROGMEM APPSKEY[16] = { 0xE7, 0x76, 0xF4, 0x8E, 0xBB, 0xA1, 0xE3, 0xD7, 0xBF, 0xE8, 0xCD, 0xF2, 0x5D, 0x1A, 0x2C, 0xBA };
#Configure your contry accodtingly for Europe you may use ="EU"
TTN_CONFIG = TTN(DEVADDR, NWKEY, APP, country="EU")
FPORT = 1


#Initialize the RFM95
lora = uLoRa(
    cs=LORA_CS,
    sck=LORA_SCK,
    mosi=LORA_MOSI,
    miso=LORA_MISO,
    irq=LORA_IRQ,
    rst=LORA_RST,
    ttn_config=TTN_CONFIG,
    datarate=LORA_DATARATE,
    fport=FPORT
)
  
try:
    f = open('counter.txt','r')
    strval = f.read()
    f.close()
except OSError:  # open failed
   # handle the file open case
    f = open('counter.txt','w')
    f.write('0')
    f.close()
    print("Write new counter.txt with value=0")
    f = open('counter.txt','r')
    strval = f.read()
    f.close()
print(strval)
lora.frame_counter= int(strval)
print("Start with Framecounter="+str(lora.frame_counter))

while True:
    data = b""
    data += "Hola univers!"
    print("Sending packet...", lora.frame_counter, ubinascii.hexlify(data))
    lora.send_data(data, len(data), lora.frame_counter)
    print(len(data), "bytes sent!")
    lora.frame_counter += 1
    f = open('counter.txt','w')
    f.write(str(lora.frame_counter))
    f.close()
    print("Sleep for 900 seconds")
    time.sleep(900)

