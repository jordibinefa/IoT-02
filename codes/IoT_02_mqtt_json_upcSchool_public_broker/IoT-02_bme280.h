#include "Arduino.h"

#ifndef IOT_02_BME280_H
#define IOT_02_BME280_H

#include "IoT-02_pinout.h"

#include <Adafruit_Sensor.h>
#include "Adafruit_BME280.h"
#define SEALEVELPRESSURE_HPA (1013.25)

extern Adafruit_BME280 bme; // I2C

void vSetupBME280();
void vReadingBME280(int* nTx100,int* nPx100,int* nRHx100,int* nGr,int* nAx100);
int nTx100_bme();
int nPx100_bme();
int nRHx100_bme();
int nGr_bme();
int nAx100_bme(int nSeaLevelPressure_hPa);

#endif
