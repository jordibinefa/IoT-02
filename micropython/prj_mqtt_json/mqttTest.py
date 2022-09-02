from wifiManagement import keepWifiConnected
from microWorkers   import MicroWorkers
from time           import sleep
from mqttManagement import mqttConnect
from machine        import Pin,ADC,SoftI2C
import ubinascii,network
import heapq
import _thread
import BME280
from ssd1306 import SSD1306_I2C
from font import Font
import ujson

bt0 = Pin(0,Pin.IN)
bt34 = Pin(34,Pin.IN)
bt35 = Pin(35,Pin.IN)
ledW = Pin(19,Pin.OUT)
ledR = Pin(23,Pin.OUT)
ledY = Pin(27,Pin.OUT)
ledG = Pin(32,Pin.OUT)
setLatchingRelay = Pin(2,Pin.OUT)
rstLatchingRelay = Pin(4,Pin.OUT)
stateLatchingRelay = Pin(39,Pin.IN)
ldr = ADC(Pin(36))
i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
queue = []
mutex = _thread.allocate_lock()
mqttc=None

wifiRunning = False;
mqttRunning = False;
previousWifiState = wifiRunning
sta_if = network.WLAN(network.STA_IF)
wlan_mac = sta_if.config('mac')
MAC = ubinascii.hexlify(wlan_mac).decode().upper()
print("MAC: %s"%MAC)
# Topics for subscription
TOPIC_LED_W = b'/' + MAC.encode() + b'/ledW'
TOPIC_LED_R = b'/' + MAC.encode() + b'/ledR'
TOPIC_LED_Y = b'/' + MAC.encode() + b'/ledY'
TOPIC_LED_G = b'/' + MAC.encode() + b'/ledG'
TOPIC_LATCHING_RELAY = b'/' + MAC.encode() + b'/latchingRelay'
TOPIC_REQUEST_LATCHING_RELAY_STATE = b'/' + MAC.encode() + b'/latchingRelayStateReq'
TOPIC_REQUEST_LDR = b'/' + MAC.encode() + b'/ldrReq'
TOPIC_REQUEST_MAC = b'/macReq'
TOPIC_REQUEST_T = b'/' + MAC.encode() + b"/tempReq"
TOPIC_REQUEST_RH = b'/' + MAC.encode() + b"/rhReq"
TOPIC_REQUEST_P = b'/' + MAC.encode() + b"/pressureReq"
TOPIC_SMALL_TEXT = b'/' + MAC.encode() + b"/10pxTxt"
TOPIC_MEDIUM_TEXT = b'/' + MAC.encode() + b"/16pxTxt"
TOPIC_BIG_TEXT = b'/' + MAC.encode() + b"/24pxTxt"
TOPIC_XTRABIG_TEXT = b'/' + MAC.encode() + b"/32pxTxt"
TOPIC_JSON_INPUT_REQ = b'/' + MAC.encode() + b"/jsonInReq"
# Topics for publishing
TOPIC_MAC = b'/mac'
TOPIC_LATCHING_RELAY_STATE = b'/' + MAC.encode() + b'/latchingRelayState'
TOPIC_LDR = b'/' + MAC.encode() + b'/ldr'
TOPIC_T = b'/' + MAC.encode() + b"/temp"
TOPIC_RH = b'/' + MAC.encode() + b"/rh"
TOPIC_P = b'/' + MAC.encode() + b"/pressure"
TOPIC_JSON_INPUT = b'/' + MAC.encode() + b"/jsonIn"

display= SSD1306_I2C(128, 32, i2c)
f=Font(display)

def clrscr():
    display.fill(0)

def vLatchingRelay(bSet):
    setLatchingRelay.value(1) if bSet else rstLatchingRelay.value(1)
    sleep(0.03)
    setLatchingRelay.value(0) if bSet else rstLatchingRelay.value(0)    

def topicManagement(topic,msg):
    global mqttc,MAC
    print("tema: %s, msg: %s"%(topic,msg))
    if topic == TOPIC_REQUEST_MAC:
        if not Pin(0).value():
            mqttc.publish( TOPIC_MAC, MAC )
            print("Tema: %s, missatge: %s"%(TOPIC_MAC,MAC))           
    elif topic == TOPIC_LED_W:
        ledW.value(msg.decode() == '1')
    elif topic == TOPIC_LED_R:
        ledR.value(msg.decode() == '1')
    elif topic == TOPIC_LED_Y:
        ledY.value(msg.decode() == '1')
    elif topic == TOPIC_LED_G:
        ledG.value(msg.decode() == '1')
    elif topic == TOPIC_LATCHING_RELAY:
        vLatchingRelay(msg.decode() == '1')
    elif topic == TOPIC_REQUEST_LATCHING_RELAY_STATE:
        msgAnswer = '1' if stateLatchingRelay.value() else '0'
        mqttc.publish( TOPIC_LATCHING_RELAY_STATE, msgAnswer )
        print("Tema: %s, missatge: %s"%(TOPIC_LATCHING_RELAY_STATE, msgAnswer))
    elif topic == TOPIC_REQUEST_LDR:
        msgAnswer = str((100*ldr.read())//4095)
        mqttc.publish( TOPIC_LDR, msgAnswer )
        print("Tema: %s, missatge: %s"%(TOPIC_LDR, msgAnswer))
    elif topic == TOPIC_REQUEST_T:
        heapq.heappush(queue,['T'])
    elif topic == TOPIC_REQUEST_RH:
        heapq.heappush(queue,['RH'])
    elif topic == TOPIC_REQUEST_P:
        heapq.heappush(queue,['P'])
    elif topic == TOPIC_SMALL_TEXT:
        heapq.heappush(queue,['OledS',msg.decode()])
    elif topic == TOPIC_MEDIUM_TEXT:
        heapq.heappush(queue,['OledM',msg.decode()])
    elif topic == TOPIC_BIG_TEXT:
        heapq.heappush(queue,['OledB',msg.decode()])
    elif topic == TOPIC_XTRABIG_TEXT:
        heapq.heappush(queue,['OledXB',msg.decode()])
    elif topic == TOPIC_JSON_INPUT_REQ:
        heapq.heappush(queue,['JSON'])

def queueSupervisor(jobName,queue):
    while True:
        if len(queue):
            todo = heapq.heappop(queue)
            print("To do: {}".format(todo[0]))
            if todo[0] == 'T' or todo[0] == 'RH' or todo[0] == 'P':
                mutex.acquire()
                bme = BME280.BME280(i2c=i2c)
                if todo[0] == 'T':
                    temp = bme.temperature
                elif todo[0] == 'RH':
                    hum = bme.humidity
                elif todo[0] == 'P':
                    pres = bme.pressure
                mutex.release()
                if todo[0] == 'T':
                    msgAnswer = str(temp); topic = TOPIC_T
                    mqttc.publish( topic, msgAnswer )
                    print("Tema: %s, missatge: %s"%(topic, msgAnswer))
                elif todo[0] == 'RH':
                    msgAnswer = str(hum); topic = TOPIC_RH
                    mqttc.publish( topic, msgAnswer )
                    print("Tema: %s, missatge: %s"%(topic, msgAnswer))
                elif todo[0] == 'P':
                    msgAnswer = str(pres); topic = TOPIC_P
                    mqttc.publish( topic, msgAnswer )
                    print("Tema: %s, missatge: %s"%(topic, msgAnswer))
            if todo[0] == 'OledS' or todo[0] == 'OledM' or todo[0] == 'OledB' or todo[0] == 'OledXB':
                clrscr()
                if todo[0] == 'OledS':
                    f.text(todo[1],0,12,8)
                if todo[0] == 'OledM':
                    f.text(todo[1],0,8,16)
                if todo[0] == 'OledB':
                    f.text(todo[1],0,3,24)
                if todo[0] == 'OledXB':
                    f.text(todo[1],0,0,32)
                f.show()
            if todo[0] == 'JSON':
                mutex.acquire()
                bme = BME280.BME280(i2c=i2c)
                temp = bme.temperature
                hum = bme.humidity
                pres = bme.pressure
                mutex.release()
                iot02 = {}
                iot02['bIO0'] = bt0.value(); iot02['bI34'] = bt34.value(); iot02['bI35'] = bt35.value()
                iot02['LDR'] = str((100*ldr.read())//4095)
                iot02['T'] = temp; iot02['HR'] = hum; iot02['P'] = pres
                msgAnswer = ujson.dumps(iot02); topic = TOPIC_JSON_INPUT
                mqttc.publish( topic, msgAnswer )
                print("Tema: %s, missatge: %s"%(topic, msgAnswer))                
        sleep(0.05)

def buttonSupervisor(jobName,btPin):
    global mqttRunning,MAC
    print("Supervising button {}".format(btPin))
    if btPin:
        TEMA_BOTO = b'/' + MAC.encode() + b'/btI%d'%btPin
    else:
        TEMA_BOTO = b'/' + MAC.encode() + b'/btIO0'
    btn = Pin(btPin,Pin.IN)
    currentBtn = btn()
    lastBtn = currentBtn
    while True:
        currentBtn = btn()
        if currentBtn != lastBtn:
            lastBtn = currentBtn
            if currentBtn == 1:
                print("Button %d released. "%btPin,end='')
            else:
                print("Button %d pressed. "%btPin,end='')
            missatge = str(currentBtn).encode()
            if mqttRunning:
                try:
                    mqttc.publish( TEMA_BOTO, missatge )
                    print("Tema: %s, missatge: %s"%(TEMA_BOTO,missatge))
                except:
                    print("It looks like there is not MQTT connection")
                    print('network config:', sta_if.ifconfig())                    
            else:
                print("There is not MQTT connection")
        sleep(0.05)

def pingMqtt(jobName):
    global mqttc
    n = 0
    while True:
        try:
            mqttc.ping()
            print("ping: {}".format(n))
            n += 1
        except:
            print("trouble with mqttc")
        sleep(2)
        
workers = MicroWorkers(workersCount=5)
workers.AddJob('keepWifiConnected',keepWifiConnected,arg=sta_if,onFinished=None)
workers.AddJob('button35Supervisor',buttonSupervisor,arg=35,onFinished=None)
workers.AddJob('button34Supervisor',buttonSupervisor,arg=34,onFinished=None)
workers.AddJob('button0Supervisor',buttonSupervisor,arg=0,onFinished=None)
workers.AddJob('queueSupervisor',queueSupervisor,arg=queue,onFinished=None)

clrscr()
f.text("IoT-02's MAC:",0,0,8)
f.text(MAC,4,12,16)
f.show()

n = 0
while workers.IsWorking:
    wifiRunning = sta_if.isconnected()
    if previousWifiState != wifiRunning:
        previousWifiState = wifiRunning
        if wifiRunning:
            try:
                mqttc = mqttConnect()
                mqttc.set_callback(topicManagement)
                mqttc.subscribe(TOPIC_LED_G)
                mqttc.subscribe(TOPIC_LED_Y)
                mqttc.subscribe(TOPIC_LED_R)
                mqttc.subscribe(TOPIC_LED_W)
                mqttc.subscribe(TOPIC_LATCHING_RELAY)
                mqttc.subscribe(TOPIC_REQUEST_LATCHING_RELAY_STATE)
                mqttc.subscribe(TOPIC_REQUEST_LDR)
                mqttc.subscribe(TOPIC_REQUEST_MAC)
                mqttc.subscribe(TOPIC_REQUEST_T)
                mqttc.subscribe(TOPIC_REQUEST_RH)
                mqttc.subscribe(TOPIC_REQUEST_P)
                mqttc.subscribe(TOPIC_SMALL_TEXT)
                mqttc.subscribe(TOPIC_MEDIUM_TEXT)
                mqttc.subscribe(TOPIC_BIG_TEXT)
                mqttc.subscribe(TOPIC_XTRABIG_TEXT)
                mqttc.subscribe(TOPIC_JSON_INPUT_REQ)
                mqttRunning = True
            except:
                mqttRunning = False
                print("trouble with MQTT's broker")
        else:
            mqttRunning = False
    else:
        if wifiRunning:
            if mqttRunning:
                mqttc.check_msg()
                n += 1
                if n > 20:
                    mqttc.ping() # this ping every two seconds keeps MQTT connection reliable
                    # print("mqttc.ping()")
                    n = 0
    sleep(0.1)
