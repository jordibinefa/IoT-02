#ifndef IOT_02_WIFI_CREDENTIALS_H
#define IOT_02_WIFI_CREDENTIALS_H

#define N_WIFIS 9
#define MAX_STRING_SIZE 15

struct stWifiList {
  const char* szSSID;
  const char* szPWD;
};

/*struct*/ stWifiList stWiFi[N_WIFIS] = {
  {"SSID_xarxa_01", "contrasenya_01"},
  {"SSID_xarxa_02", "contrasenya_02"},
  {"SSID_xarxa_03", "contrasenya_03"},
  {"SSID_xarxa_04", "contrasenya_04"},
  {"SSID_xarxa_05", "contrasenya_05"},
  {"SSID_xarxa_06", "contrasenya_06"},
  {"SSID_xarxa_07", "contrasenya_07"},
  {"SSID_xarxa_08", "contrasenya_08"},
  {"SSID_xarxa_09", "contrasenya_09"}
};

#endif // IOT_02_WIFI_CREDENTIALS_H
