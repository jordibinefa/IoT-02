#include <Arduino.h> // OUTPUT and INPUT
#include "IoT-02_common.h"

void vSetupIO() {
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(SET_LATCHING_RELAY, OUTPUT);
  pinMode(RST_LATCHING_RELAY, OUTPUT);
  pinMode(BT_IO0, INPUT);
  pinMode(BT_I34, INPUT);
  pinMode(BT_I35, INPUT);
  pinMode(STATE_LATCHING_RELAY, INPUT);
}

void vLatchingRelay(bool bSet) {
  digitalWrite(bSet ? SET_LATCHING_RELAY : RST_LATCHING_RELAY, HIGH);
  delay(3);
  digitalWrite(bSet ? SET_LATCHING_RELAY : RST_LATCHING_RELAY, LOW);
}

boolean bPressedButton(int nWhichOne) {
  if (digitalRead(nWhichOne))
    return false;
  return true;
}



