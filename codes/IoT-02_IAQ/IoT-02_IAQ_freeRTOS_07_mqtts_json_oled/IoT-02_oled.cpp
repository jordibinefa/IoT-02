#include "IoT-02_pinout.h"
#include "IoT-02_oled.h"



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
      nDarrerAra = nAra;
      //vReadingBME680(&nTx100, &nPx100, &nRHx100, &nGr, &nAx100);
    }
    display.drawString(0, 20, "T: " + String(((float)nTx100) / 100) + " *C");
    display.drawString(0, 30, "RH: " + String(((float)nRHx100) / 100) + " %");
    display.drawString(0, 40, "P: " + String(((float)nPx100) / 100) + " hPa");
    display.drawString(0, 50, "G: " + String(nGr) + "  ");
  } else {
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 22, "Llum: ");
    display.setFont(ArialMT_Plain_24);
    display.drawString(26, 42, String((100 * analogRead(LDR)) / 4095) + " %");
  }

  display.display();
}

void vScreen10pixelText(int x, int y, String szText) {
  display.clear();
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(x, y, szText);
  display.display();
}

void vScreen16pixelText(int x, int y, String szText) {
  display.clear();
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(x, y, szText);
  display.display();
}

void vScreen24pixelText(int x, int y, String szText) {
  display.clear();
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(x, y, szText);
  display.display();
}

void vScreenIAQ(int nEstat, int nIAQ) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(50, 0, "IAQ: " + String(nIAQ));
  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 10, "Qualitat de l'aire");
  display.setFont(ArialMT_Plain_24);
  switch (nEstat) {
    case 0:
      display.drawString(0, 28, "EVAQUEU!");
      display.setFont(ArialMT_Plain_10);
      display.drawString(22, 52, "CAL VENTILAR");
      break;
    case 1:
      display.drawString(0, 28, "MOLT DOLENTA");
      display.setFont(ArialMT_Plain_10);
      display.drawString(22, 52, "CAL VENTILAR");
      break;
    case 2:
      display.drawString(10, 28, "DOLENTA");
      display.setFont(ArialMT_Plain_10);
      display.drawString(22, 52, "CAL VENTILAR");
      break;
    case 3:
      display.drawString(15, 28, "ENRARIT");
      display.setFont(ArialMT_Plain_10);
      display.drawString(22, 52, "CAL VENTILAR");
      break;
    case 4:
      display.drawString(10, 28, "REGULAR");
      display.setFont(ArialMT_Plain_10);
      display.drawString(17, 52, "NO CAL VENTILAR");
      break;
    case 5:
      display.drawString(30, 28, "BONA");
      display.setFont(ArialMT_Plain_10);
      display.drawString(17, 52, "NO CAL VENTILAR");
      break;
  }
  display.display();
}

void vPantallaInicial(String sVer) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 20, "Qualit AIRE");
  display.setFont(ArialMT_Plain_10);
  display.drawString(50, 52, "v " + sVer);
  display.display();
}

void vScreenMacIp(String sMAC, String sIP) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "MAC: ");
  display.drawString(0, 15, sMAC);
  display.drawString(0, 30, "IP: ");
  display.drawString(0, 45, sIP);
  display.display();
}
