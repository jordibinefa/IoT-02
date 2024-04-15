#include "IoT-02_wifiMng.h"
#include "IoT-02_wifiCredentials.h"

void vDelayESP(unsigned long ulMilliseconds)
{
    unsigned long ulPreviousMillis = millis();

    do
    {
        yield();
    } while (millis() - ulPreviousMillis <= ulMilliseconds);
}

void vConnectToWiFi(const char *szSsid, const char *szPwd)
{
    int nCmpt = 0;
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS; // 0.5 seconds

    Serial.println("Connecting to WiFi network: " + String(szSsid) + ", pwd: " + String(szPwd));

    WiFi.begin(szSsid, szPwd);

    Serial.println("Waiting for WIFI connection...");
    while (WiFi.status() != WL_CONNECTED)
    {
        if (nCmpt > N_TRIES_2_RST)
        {
            ESP.restart();
            Serial.print("*");
            nCmpt = 0;
        }
        else
        {
           Serial.print(".");
        }
        nCmpt++;
        vDelayESP(500);
        //vTaskDelay( xDelay );
    }
    Serial.println();
}

boolean bIsListed(String szSSID, int *pNwO)
{
    for (int i = 0; i < N_WIFIS; i++)
    {
        if (String(stWiFi[i].szSSID) == szSSID)
        {
            *pNwO = i;
            return true;
        }
    }
    return false;
}

boolean bTryWifiConnection()
{
    int n = WiFi.scanNetworks(), nWhichOne;
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1 second

    //Serial.print("*");
    if (n == 0)
    {
        Serial.println("\nNo networks found");
        vDelayESP(1000);
        //vTaskDelay( xDelay );
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            if (bIsListed(WiFi.SSID(i), &nWhichOne))
            {
                vConnectToWiFi(stWiFi[nWhichOne].szSSID, stWiFi[nWhichOne].szPWD);
                return true;
            }
        }
    }
    return false;
}

void WiFiReset() {
   WiFi.persistent(false);
   WiFi.disconnect();
   WiFi.mode(WIFI_OFF);
   WiFi.mode(WIFI_STA);
}

void vSetupWifi()
{
    const TickType_t xDelay = 100 / portTICK_PERIOD_MS; // 0.1 seconds

    do
    {
        //WiFi.mode(WIFI_STA);
        //WiFi.disconnect();
        WiFiReset();
        vDelayESP(100);
        //vTaskDelay( xDelay );
    } while (!bTryWifiConnection());
}

String szGetMac()
{
    byte mac[6];
    String szMAC = "";
    char szMac[3];

    WiFi.macAddress(mac);
    //for (int i = 5; i >= 0; i--)
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] > 0x0F)
            sprintf(szMac, "%2X", mac[i]);
        else
            sprintf(szMac, "0%X", mac[i]);
        szMAC += szMac;
    }

    return szMAC;
}
