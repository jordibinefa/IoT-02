#include "Arduino.h"

#ifndef IOT_02_MODBUS_H
#define IOT_02_MODBUS_H

#include "IoT-02_pinout.h"
#include "modbus_ecat.h"

#define MODULE_ADDRESS 0x07
#define COIL_RELAY_ADDRESS 0x0004
#define COIL_INPUTS_ADDRESS 0x0000

#define ENOUGH_TIME_SENDING_ANSWER 15 /* in milliseconds */

extern HardwareSerial modbusData;

void vSetupModBus(int nBauds);
void vWriteCoil(byte* ucSt, int len);
void vReadInputs(byte* ucSt, int len);
void vProcessa(byte* ucSt, int len);
void vModBusReading();

byte byReadInputs();

#endif
