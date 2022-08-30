'''
Based on: https://bhave.sh/micropython-mqtt/
https://github.com/jordibinefa/IoT-02/tree/master/micropython/prj_mqtt_00
20220830
'''
from umqttsimple import MQTTClient
from machine import Pin
from time import sleep
from random import choice
import ubinascii,network

def cadenaAleatoria(longitud):
    lletres = ['a','b','c','d','e','f','0','1','2','3','4','5','6','7','8','9']
    return 'IoT-02_'+''.join(choice(lletres) for i in range(longitud))

def controlLedVerd(topic,msg):
    print("tema: %s, msg: %s"%(topic,msg))
    if msg.decode() == '1':
        ledG.value(1)
    else:
        ledG.value(0)

btn = Pin(35,Pin.IN)
ledW = Pin(19,Pin.OUT)
ledG = Pin(32,Pin.OUT)
wlan_sta = network.WLAN(network.STA_IF)
wlan_mac = wlan_sta.config('mac')
MAC = ubinascii.hexlify(wlan_mac).decode().upper()
print("MAC: %s"%MAC)
TEMA_BOTO = b'/' + MAC.encode() + b'/btn/35'
TEMA_LED_VERD = b'/' + MAC.encode() + b'/ledG'
print("TEMA_LED_VERD: %s"%TEMA_LED_VERD)
CLIENT_NAME = cadenaAleatoria(10)
BROKER_ADDR = 'formacio.things.cat'
BROKER_PORT = 1883
BROKER_USER = 'ecat'
BROKER_PWD = 'clotClot'
mqttc = MQTTClient(CLIENT_NAME, BROKER_ADDR, port=BROKER_PORT, user=BROKER_USER,password=BROKER_PWD,keepalive=60)
mqttc.connect()
mqttc.set_callback(controlLedVerd)
mqttc.subscribe(TEMA_LED_VERD)

currentBtn = btn()
lastBtn = currentBtn
while True:
    currentBtn = btn()
    if currentBtn != lastBtn:
        lastBtn = currentBtn
        if currentBtn == 1:
            print("Button released. ",end='')
        else:
            print("Button pressed. ",end='')
        missatge = str(currentBtn).encode()
        try:
            mqttc.publish( TEMA_BOTO, missatge )
            print("Tema: %s, missatge: %s"%(TEMA_BOTO,missatge))
        except:
            print("It looks like there is not MQTT connection")

    mqttc.check_msg()
    sleep(0.05)
