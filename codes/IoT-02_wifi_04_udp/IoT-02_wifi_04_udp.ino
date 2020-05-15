// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// Code written by Jordi Binefa. 20200514

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"
#include <WiFi.h>
#include "IoT-02_wifiMng.h"
#include <WiFiUdp.h>

#define LED_1 0x01
#define LED_2 0x02
#define LED_3 0x04
#define LED_4 0x08

#define MAC_SIZE 15
char sMac[MAC_SIZE];

unsigned int udpPortRx = 3334;
unsigned int udpPortTx = 3333;

static byte byState;
byte packetBuffer[512]; //buffer to hold incoming and outgoing packets

WiFiUDP Udp;

void vSendUdp(String sz, unsigned int uiLocalPort) {
  Udp.beginPacket(Udp.remoteIP(), uiLocalPort);
  Udp.print(sz);
  Udp.endPacket();
}

bool bManageMsg(String szMsg) {
  bool bUnderstood = false;

  if (szMsg == "i" || szMsg == "I") {
    vSendUdp(bPressedButton(BT_IO0)?"L":"H", udpPortTx);
    bUnderstood = true;
  }
  if (szMsg == "i34" || szMsg == "I34") {
    vSendUdp(bPressedButton(BT_I34)?"L":"H", udpPortTx);
    bUnderstood = true;
  }
  if (szMsg == "i35" || szMsg == "I35") {
    vSendUdp(bPressedButton(BT_I35)?"L":"H", udpPortTx);
    bUnderstood = true;
  }
  if ( szMsg == "W" ) {
    byState |= LED_1;
    Serial.println("WHITE HIGH");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }
  if (szMsg == "w") {
    byState &= ~LED_1;
    Serial.println("WHITE LOW");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }  
  if ( szMsg == "R" ) {
    byState |= LED_2;
    Serial.println("RED HIGH");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }
  if (szMsg == "r") {
    byState &= ~LED_2;
    Serial.println("RED LOW");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }
  if ( szMsg == "Y" ) {
    byState |= LED_3;
    Serial.println("YELLOW HIGH");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }
  if (szMsg == "y") {
    byState &= ~LED_3;
    Serial.println("YELLOW LOW");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }  
  if ( szMsg == "G" ) {
    byState |= LED_4;
    Serial.println("GREEN HIGH");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }
  if (szMsg == "g") {
    byState &= ~LED_4;
    Serial.println("GREEN LOW");
    bUnderstood = true;
    vSendUdp("OK", udpPortTx);
  }
  if (szMsg == "L" || szMsg == "l") {
    String sLdr = String(analogRead(LDR));
    Serial.println(sLdr);
    bUnderstood = true;
    vSendUdp(sLdr.c_str(), udpPortTx);
  }

  return bUnderstood;
}

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

  byState = 0x00;
  Serial.println("Connected to wifi");
  Serial.print("Udp server started at port ");
  Serial.println(udpPortRx);
  Udp.begin(udpPortRx);
}

void vWriteLeds(byte by) {
  digitalWrite(LED_W, by & LED_1);
  digitalWrite(LED_R, by & LED_2);
  digitalWrite(LED_Y, by & LED_3);
  digitalWrite(LED_G, by & LED_4);
}

void loop() {
  int noBytes = Udp.parsePacket();
  String received_command = "";

  if ( noBytes ) {
    Serial.print(millis() / 1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, noBytes); // read the packet into the buffer

    // display the packet contents in HEX
    for (int i = 1; i <= noBytes; i++) {
      Serial.print(packetBuffer[i - 1], HEX);
      received_command = received_command + char(packetBuffer[i - 1]);
      if (i % 32 == 0) {
        Serial.println();
      }
      else Serial.print(' ');
    } // end for
    Serial.println();

    if (! bManageMsg(received_command)) {
      vSendUdp("I don't understand this command: ", udpPortTx);
      vSendUdp(received_command, udpPortTx);
    }
    Serial.println(received_command);
    Serial.println();
    vWriteLeds(byState);
  }
}
