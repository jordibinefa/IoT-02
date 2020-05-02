#include "Arduino.h"

#ifndef IOT_02_MODBUS_H
#define IOT_02_MODBUS_H

#include "IoT-02_pinout.h"
#include "modbus_ecat.h"

#define MODULE_ADDRESS 0x07
#define COIL_RELAY_ADDRESS 0x0004
#define COIL_INPUTS_ADDRESS 0x0000
#define HOLDING_REGISTER_TEMEPRATURE_ADDRESS 0x0000
#define HOLDING_REGISTER_REL_HUMIDITY_ADDRESS 0x0001
#define HOLDING_REGISTER_PRESSURE_ADDRESS 0x0002
#define HOLDING_REGISTER_ALTITUDE_ADDRESS 0x0003
#define HOLDING_REGISTER_VOC_ADDRESS 0x0004

#define ENOUGH_TIME_SENDING_ANSWER 15 /* in milliseconds */

extern HardwareSerial modbusData;

void vSetupModBus(int nBauds);
void vWriteCoil(byte* ucSt, int len);
void vReadInputs(byte* ucSt, int len);
void vReadHoldingRegisters(byte* ucSt, int len);
void vProcessa(byte* ucSt, int len);
void vModBusReading();

byte byReadInputs();

#endif
