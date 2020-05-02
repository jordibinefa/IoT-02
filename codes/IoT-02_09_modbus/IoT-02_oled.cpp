#include "IoT-02_pinout.h"
#include "IoT-02_oled.h"

void vSetupScreen() {
  display.init();
  display.flipScreenVertically();
}

void vScreenDemo(boolean bG, boolean bY, boolean bR, boolean bW) {
  String sLeds = "G:";
  sLeds += bG ? "ON " : "OFF";
  sLeds += " Y:"; sLeds += bY ? "ON " : "OFF";
  sLeds += " R:"; sLeds += bR ? "ON " : "OFF";
  sLeds += " W:"; sLeds += bW ? "ON " : "OFF";
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
