#include "IoT-02_pinout.h"
#include "IoT-02_oled.h"
#include "IoT-02_bme280.h"

void vSetupScreen() {
  display.init();
  display.flipScreenVertically();
}

void vScreenDemo(boolean bG, boolean bY, boolean bR, boolean bW, int nCmpt) {
  int nAra = millis() / 1000;
  static int nDarrerAra = nAra, nTx100, nPx100, nRHx100, nGr, nAx100;
  String sLeds = "G:";
  sLeds += bG ? "ON " : "OFF";
  sLeds += " Y:"; sLeds += bY ? "ON " : "OFF";
  sLeds += " R:"; sLeds += bR ? "ON " : "OFF";
  sLeds += " W:"; sLeds += bW ? "ON " : "OFF";
  display.clear();

  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Comptador: " + String(nAra));
  //display.drawString(0, 10, String("R:OFF Y:OFF G:OFF W:OFF    "));
  display.drawString(0, 10, sLeds);
  if (nCmpt % 2) {
    if (nDarrerAra != nAra) {
      vReadingBME280(&nTx100, &nPx100, &nRHx100, &nGr, &nAx100);
      nAx100 = (short int)nAx100_bme( SEALEVELPRESSURE_HPA );  
    }
    display.drawString(0, 20, "T: " + String(((float)nTx100) / 100) + " *C");
    display.drawString(0, 30, "RH: " + String(((float)nRHx100) / 100) + " %");
    display.drawString(0, 40, "P: " + String(((float)nPx100) / 100) + " hPa");
    display.drawString(0, 50, "A: " + String(((float)nAx100) / 100) + " m");
  } else {
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 22, "Llum: ");
    display.setFont(ArialMT_Plain_24);
    display.drawString(26, 42, String((100 * analogRead(LDR)) / 4095) + " %");
  }

  display.display();

  nDarrerAra = nAra;
}
