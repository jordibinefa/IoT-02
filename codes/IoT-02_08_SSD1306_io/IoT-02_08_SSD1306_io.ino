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

void vScreenDemo(boolean bG,boolean bY,boolean bR,boolean bW) {
  String sLeds = "G:";
  sLeds += bG?"ON ":"OFF";
  sLeds += " Y:"; sLeds += bY?"ON ":"OFF";
  sLeds += " R:"; sLeds += bR?"ON ":"OFF";
  sLeds += " W:"; sLeds += bW?"ON ":"OFF";
  display.clear();

  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Comptador: " + String(millis() / 1000));
  //display.drawString(0, 10, String("R:OFF Y:OFF G:OFF W:OFF    "));
  display.drawString(0, 10, sLeds);
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
  static boolean bIO0wasPressed = false;
  static boolean bI35wasPressed = false;
  static boolean bI34wasPressed = false;
  boolean bLatchingRelayState = digitalRead(STATE_LATCHING_RELAY);
  boolean bIO0currentState = bPressedButton(BT_IO0);
  boolean bI34currentState = bPressedButton(BT_I34);
  boolean bI35currentState = bPressedButton(BT_I35);
  boolean bLedG,bLedY,bLedR,bLedW;

  if (bIO0wasPressed != bIO0currentState) {
    delay(2);
    if (bIO0currentState) {
      bLatchingRelayState = !bLatchingRelayState;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
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
  vScreenDemo(bLedG,bLedY,bLedR,bLedW);
  delay(10);
}
