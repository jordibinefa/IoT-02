#ifndef IOT_02_WIFI_CREDENTIALS_H
#define IOT_02_WIFI_CREDENTIALS_H

#define N_WIFIS 9
#define MAX_STRING_SIZE 15

struct stWifiList {
  const char* szSSID;
  const char* szPWD;
};

/*struct*/ stWifiList stWiFi[N_WIFIS] = {
  {"MiFibra-213B" , "JdkZYbkx"},
  {"IoT-eCat_RPi" , "clotClot"},
  {"WIFI_SIARQ","GuestsWLAN@Siarq"},
  {"VodafoneMobileWiFi-6D9883","7770797201"},
  {"IoT-eCat" , "clotClot"},
  {"ONO1BD3","YeneA9pwQx81"},
  {"Androidalexis","12345678"},
  {"JESUITESFP" , "internetcoses"},
  {"iotcat" , "1234567890a"}
};

#endif // IOT_02_WIFI_CREDENTIALS_H
