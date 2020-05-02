// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// BME680 is able to read temperature, relative humidity and pressure
// Using https://github.com/adafruit/Adafruit_BME280_Library
//
// In computer side: ./mb_IoT_00.py /dev/ttyUSB1 4800
// /dev/ttyUSB1 USB is serial port name and 4800 is baud rate
//
// Code written by Jordi Binefa. 20200502

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"

#include "IoT-02_oled.h"
SSD1306  display(0x3c, I2C_SDA, I2C_SCL);

#include "IoT-02_modbus.h"
HardwareSerial modbusData(2);
#define MODBUS_BAUD_RATE 4800

#include "IoT-02_bme280.h"
Adafruit_BME280 bme; 

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  vSetupIO();
  vSetupScreen();
  vSetupModBus(MODBUS_BAUD_RATE);
  Serial.begin(115200);
  Serial.println(__FILE__);
  vSetupBME280();
}

void loop() {
  static boolean bIO0wasPressed = false;
  static boolean bI35wasPressed = false;
  static boolean bI34wasPressed = false;
  static int nCmptIO0 = 0;
  boolean bLatchingRelayState = digitalRead(STATE_LATCHING_RELAY);
  boolean bIO0currentState = bPressedButton(BT_IO0);
  boolean bI34currentState = bPressedButton(BT_I34);
  boolean bI35currentState = bPressedButton(BT_I35);
  boolean bLedG, bLedY, bLedR, bLedW;

  if (bIO0wasPressed != bIO0currentState) {
    delay(2);
    if (bIO0currentState) {
      bLatchingRelayState = !bLatchingRelayState;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
      nCmptIO0++;
    }
    bIO0wasPressed = bIO0currentState;
  }
  if (bI34wasPressed != bI34currentState) {
    delay(2);
    if (bI34currentState) {
      bLatchingRelayState = HIGH;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
    }
    bI34wasPressed = bI34currentState;
  }
  if (bI35wasPressed != bI35currentState) {
    delay(2);
    if (bI35currentState) {
      bLatchingRelayState = LOW;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
    }
    bI35wasPressed = bI35currentState;
  }
  bLedG = bPressedButton(BT_I35);
  bLedY = bLatchingRelayState;
  bLedR = bPressedButton(BT_I34);
  bLedW = bPressedButton(BT_IO0);
  digitalWrite(LED_G, bLedG);
  digitalWrite(LED_Y, bLedY);
  digitalWrite(LED_R, bLedR);
  digitalWrite(LED_W, bLedW);
  vScreenDemo(bLedG, bLedY, bLedR, bLedW,nCmptIO0);
  vModBusReading();
  
  delay(10);
}
