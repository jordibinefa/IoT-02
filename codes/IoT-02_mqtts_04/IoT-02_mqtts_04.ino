// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// Code written by Jordi Binefa. 20200511

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"
#include <WiFi.h>
#include "IoT-02_wifiMng.h"
#include "IoT-02_mqttCredentials.h"
#include "IoT-02_mqttTopics.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>

/* create an instance of WiFiClientSecure */
WiFiClientSecure espClient;
PubSubClient client(espClient);

#define MAC_SIZE 15
char sMac[MAC_SIZE];

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  String szTopic = String(topic);
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  //Serial.print("Topic: "); Serial.println(szTopic);
  if (szTopic == TOPIC_REQUEST_MAC) {
    if (bPressedButton(BT_IO0)) {
      //szGetMac().toCharArray(sMac, MAC_SIZE);
      Serial.print(TOPIC_MAC); Serial.print(" : "); Serial.println(sMac);
      client.publish(TOPIC_MAC, sMac);
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_LED_W).c_str()) {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_W, HIGH);
    } else {
      digitalWrite(LED_W, LOW);
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_LED_R).c_str()) {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_R, HIGH);
    } else {
      digitalWrite(LED_R, LOW);
    }
  }  
  if (szTopic == String("/" + String(sMac) + TOPIC_LED_Y).c_str()) {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_Y, HIGH);
    } else {
      digitalWrite(LED_Y, LOW);
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_LED_G).c_str()) {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_G, HIGH);
    } else {
      digitalWrite(LED_G, LOW);
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_LATCHING_RELAY).c_str()) {
    if ((char)payload[0] == '1') {
      vLatchingRelay(true);
    } else {
      vLatchingRelay(false);
    }
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "IoT-02_" + String(sMac); // <-------   Unique name in every device
    /* connect now */
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      //if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic */
      client.subscribe(TOPIC_REQUEST_MAC); // <-------   Subscription to MQTT(S) topic
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_W).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_R).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_Y).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_G).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LATCHING_RELAY).c_str());
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

void vSetupMqtt() {
  /* set SSL/TLS certificate */
  espClient.setCACert(ca_cert);
  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, mqtt_port);
  /* this receivedCallback function will be invoked
    when client received subscribed topic */
  client.setCallback(receivedCallback);
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
  vSetupMqtt();
}

void loop() {
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming
    subscribed topic-process-invoke receivedCallback */
  client.loop();
}
