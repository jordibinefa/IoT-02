// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// Code written by Jordi Binefa. 20200514

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"
#include <WiFi.h>
#include "IoT-02_wifiMng.h"

#define MAC_SIZE 15
char sMac[MAC_SIZE];

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);

  vSetupIO();
  vSetupWifi();
  szGetMac().toCharArray(sMac, MAC_SIZE);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}
