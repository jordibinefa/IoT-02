# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()

def connect():
    import network
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        sta_if.active(True)
        sta_if.connect('CASTELLA40_2G', '1A2B3C4D5E')
        while not sta_if.isconnected():
            pass # wait till connection
    print('network config:', sta_if.ifconfig())

print("Benvinguts a MicroPython per a la placa IoT-02")    
connect()