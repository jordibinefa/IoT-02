from mqttCredentials import brokerMqtt
from umqttsimple import MQTTClient
from time import sleep
from random import choice

def cadenaAleatoria(longitud):
    lletres = ['a','b','c','d','e','f','0','1','2','3','4','5','6','7','8','9']
    return 'IoT-02_'+''.join(choice(lletres) for i in range(longitud))

def mqttConnect():
    CLIENT_NAME = cadenaAleatoria(10)
    BROKER_ADDR = brokerMqtt["addr"]
    BROKER_PORT = brokerMqtt["port"]
    BROKER_USER = brokerMqtt["user"]
    BROKER_PWD = brokerMqtt["pwd"]
    mqttc = MQTTClient(CLIENT_NAME, BROKER_ADDR, port=BROKER_PORT, user=BROKER_USER,password=BROKER_PWD,keepalive=30)
    mqttc.connect()
    return mqttc
