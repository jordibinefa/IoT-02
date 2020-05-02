#include "Arduino.h"

#ifndef MODBUS_H
#define MODBUS_H

#ifndef N_MAX
	#define N_MAX 99
#endif

#ifndef TIMEOUT
    #define TIMEOUT 500
#endif

#ifndef SSerialTxControl
#define SSerialTxControl 33 /*RS485 Direction control*/
#define RS485Transmit HIGH
#define RS485Receive LOW
#endif

union uSI {
    byte by[2];
    short int nV;
};

union uLI {
    byte by[4];
    long int nV;
};

void vModeTxRxRS485(boolean bMode);
unsigned int uiModRTU_CRC(byte *buf, int len);
void vModbusTx(unsigned char *uc, int nLen);
bool bReadingModBus(char *);
int str2hex(char *s);
void vPeticioModBus(String sz);
extern void modbusFlush();
short int siBytes2shortInt(byte *b);
long int liBytes2longInt(byte *b);

#endif // MODBUS_H
