from wifiCredentials import ssidWifi
import network
from time import sleep

# print("ssid: {}, pwd: {}".format(ssidWifi["name"],ssidWifi["pwd"]))

def connect(sta_if):
    # print("Inside connect()")
    if not sta_if.isconnected():
        sta_if.active(True)
        sta_if.connect(ssidWifi["name"], ssidWifi["pwd"])
        while not sta_if.isconnected():
            pass
    return sta_if

def keepWifiConnected(jobName,sta_if):
    # print("Inside ",jobName)
    while True:
        if not sta_if.isconnected():
            try:
                sta_if.disconnect()
            except:
                print("trouble disconnecting WiFi")
            connect(sta_if)
            print('network config:', sta_if.ifconfig())
        sleep(0.1)    
                