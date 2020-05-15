// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// Code written by Jordi Binefa. 20200515

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"

#include "IoT-02_bme280.h"
Adafruit_BME280 bme;

#include "IoT-02_oled.h"
SSD1306  display(0x3c, I2C_SDA, I2C_SCL);

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);

  vSetupIO();
  vSetupScreen();
  vSetupBME280();
}

void loop() {
  int nTx100, nPx100, nRHx100, nGr, nAx100;
  float fT,fP,fRH,fA;

  vReadingBME280(&nTx100, &nPx100, &nRHx100, &nGr, &nAx100);
  fT = ((float)nTx100)/100; fP = ((float)nPx100)/100; fRH = ((float)nRHx100)/100; fA = ((float)nAx100)/100;
  vScreenBME280(fT, fP, fRH, fA);
  
  delay(500);
}
