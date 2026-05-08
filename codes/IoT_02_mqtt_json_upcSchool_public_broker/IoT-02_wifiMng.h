#ifndef IOT_02_WIFI_MNG
#define IOT_02_WIFI_MNG

#include "WiFiClientSecure.h"

#define N_TRIES_2_RST       24    /* 12 / 500ms -> 6 seconds */

void vDelayESP(unsigned long ulMilliseconds);
void vConnectToWiFi(const char *szSsid, const char *szPwd);
boolean bIsListed(String szSSID, int *pNwO);
boolean bTryWifiConnection();
void WiFiReset();
void vSetupWifi();
String szGetMac();

#endif // IOT_02_WIFI_MNG
