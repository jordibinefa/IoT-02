#ifndef IOT_02_OLED_H
#define IOT_02_OLED_H

#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

extern SSD1306 display;

void vSetupScreen();
void vScreenDemo(boolean bG, boolean bY, boolean bR, boolean bW, int nCmpt);
void vScreen10pixelText(int x, int y, String szText);
void vScreen16pixelText(int x, int y, String szText);
void vScreen24pixelText(int x, int y, String szText);

#endif
