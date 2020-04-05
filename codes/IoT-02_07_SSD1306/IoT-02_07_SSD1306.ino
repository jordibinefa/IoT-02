// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// Code written by Jordi Binefa. 20200405
// Based on: https://playground.arduino.cc/Main/I2cScanner?action=sourceblock&num=1

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"

#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

SSD1306  display(0x3c, I2C_SDA, I2C_SCL);

void vSetupScreen() {
  display.init();
  display.flipScreenVertically();
}

void vScreenDemo() {
  display.clear();

  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Comptador: " + String(millis() / 1000));
  display.drawString(0, 10, String("R:OFF Y:OFF G:OFF W:OFF    "));
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 22, "Llum: ");
  display.setFont(ArialMT_Plain_24);
  display.drawString(26, 42, String((100 * analogRead(LDR)) / 4095) + " %");

  display.display();
}

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  vSetupIO();
  vSetupScreen();
  Serial.begin(115200);
  Serial.println(__FILE__);
}

void loop() {
  vScreenDemo();
  delay(10);
}
