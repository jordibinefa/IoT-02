from mp_modbus_master import modbus_rtu_master
from time import sleep_ms

while True:
    try:
        rtu_master = modbus_rtu_master(
          uart_no=2, baudrate=115200, parity=0, tx_pin=17, rx_pin=16, en_pin=33 , device_addr=9
          )

        frame = rtu_master.read_holding_registers(0, 1)
        szWord = ("{0:02X}".format(frame[0])) + ("{0:02X}".format(frame[1]))
        n = int(szWord,16)
        if n > 2**15:
            n = -1*(2**16-n)
        print("T: %.2f ºC" % (n / 100))
    except Exception as e:
        print("Is ModBus device connected? (Error: %s)"%e)
    sleep_ms(1000)
