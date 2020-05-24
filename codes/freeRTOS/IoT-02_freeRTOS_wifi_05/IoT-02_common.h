#include "Arduino.h"

#ifndef IOT_02_COMMON_H
#define IOT_02_COMMON_H

#include "IoT-02_pinout.h"

void vSetupIO();
void vLatchingRelay(bool bSet);
boolean bPressedButton(int nWhichOne);

#endif
