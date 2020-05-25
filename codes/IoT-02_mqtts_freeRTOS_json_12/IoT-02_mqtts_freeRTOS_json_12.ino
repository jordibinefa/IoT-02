// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
//
// Code written by Jordi Binefa. 20200524

#define USING_WIFI

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"

#include <Arduino_JSON.h>

#ifdef USING_WIFI
#include <WiFi.h>
#include "IoT-02_wifiMng.h"
#include "IoT-02_mqttCredentials.h"
#include "IoT-02_mqttTopics.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>
WiFiClientSecure espClient;
PubSubClient client(espClient);
#define MAC_SIZE 15
char sMac[MAC_SIZE];
#endif         /* USING_WIFI */

#define MINIMUM_DELAY_ALLOWING_MULTITASKING 50

#include "IoT-02_bme280.h"
Adafruit_BME280 bme;

#include "IoT-02_oled.h"
SSD1306  display(0x3c, I2C_SDA, I2C_SCL);

SemaphoreHandle_t xMutex;

JSONVar jsonBME;

void vReadTemperature() {
  float fTc;

  fTc = ((float)nTx100_bme()) / 100;
  String szTemp = String(fTc);
  Serial.print("T: "); Serial.println(szTemp);
}

#ifdef USING_WIFI

void vSupervisingButtons() {
  static boolean bIO0wasPressed = false;
  boolean bIO0currentState = bPressedButton(BT_IO0);
  static boolean bI34wasPressed = false;
  boolean bI34currentState = bPressedButton(BT_I34);
  static boolean bI35wasPressed = false;
  boolean bI35currentState = bPressedButton(BT_I35);

  if (bIO0wasPressed != bIO0currentState) {
    delay(2);
    Serial.print("Publishing topic: "); Serial.println(String("/" + String(sMac) + TOPIC_BT_IO0).c_str());
    if (bIO0currentState) {
      client.publish(String("/" + String(sMac) + TOPIC_BT_IO0).c_str(), "IO0 pressed");
      Serial.println("Button IO0 pressed");
    } else {
      client.publish(String("/" + String(sMac) + TOPIC_BT_IO0).c_str(), "IO0 released");
      Serial.println("Button IO0 released");
    }
    bIO0wasPressed = bIO0currentState;
  }
  if (bI34wasPressed != bI34currentState) {
    delay(2);
    Serial.print("Publishing topic: "); Serial.println(String("/" + String(sMac) + TOPIC_BT_I34).c_str());
    if (bI34currentState) {
      client.publish(String("/" + String(sMac) + TOPIC_BT_I34).c_str(), "I34 pressed");
      Serial.println("Button I34 pressed");
    } else {
      client.publish(String("/" + String(sMac) + TOPIC_BT_I34).c_str(), "I34 released");
      Serial.println("Button I34 released");
    }
    bI34wasPressed = bI34currentState;
  }
  if (bI35wasPressed != bI35currentState) {
    delay(2);
    Serial.print("Publishing topic: "); Serial.println(String("/" + String(sMac) + TOPIC_BT_I35).c_str());
    if (bI35currentState) {
      client.publish(String("/" + String(sMac) + TOPIC_BT_I35).c_str(), "I35 pressed");
      Serial.println("Button I35 pressed");
    } else {
      client.publish(String("/" + String(sMac) + TOPIC_BT_I35).c_str(), "I35 released");
      Serial.println("Button I35 released");
    }
    bI35wasPressed = bI35currentState;
  }
}

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  float fTc, fRH, fP, fAlt;

  String szTopic = String(topic), szPayload = "";
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    szPayload += (char)payload[i];
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
  if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_LATCHING_RELAY_STATE).c_str()) {
    String szRelay = digitalRead(STATE_LATCHING_RELAY) ? "SET" : "RESET";
    Serial.print("Relay "); Serial.println(szRelay);
    client.publish( String("/" + String(sMac) + TOPIC_LATCHING_RELAY_STATE).c_str(), szRelay.c_str());
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_LDR).c_str()) {
    String szLdr = String(analogRead(LDR));
    Serial.print("LDR: "); Serial.println(szLdr);
    client.publish( String("/" + String(sMac) + TOPIC_LDR).c_str(), szLdr.c_str());
  }
  // jsonBME["T"] = fT; jsonBME["RH"] = fRH; jsonBME["P"] = fP; jsonBME["G"] = fGr; jsonBME["A"] = fA;
  if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_T).c_str()) {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    //fTc = ((float)nTx100_bme()) / 100;
    //xSemaphoreGive(xMutex);
    if (jsonBME.hasOwnProperty("Tx100")) {
      fTc = ((float)(int)jsonBME["Tx100"]) / 100;
      String szTemp = String(fTc);
      Serial.print("T: "); Serial.println(szTemp);
      client.publish( String("/" + String(sMac) + TOPIC_T).c_str(), szTemp.c_str());
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_RH).c_str()) {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    //fRH = ((float) nRHx100_bme()) / 100;
    //xSemaphoreGive(xMutex);
    if (jsonBME.hasOwnProperty("RHx100")) {
      fRH = ((float)(int)jsonBME["RHx100"]) / 100;
      String szRH = String(fRH);
      Serial.print("RH: "); Serial.print(szRH); Serial.println(" %");
      client.publish( String("/" + String(sMac) + TOPIC_RH).c_str(), szRH.c_str());
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_P).c_str()) {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    //fP = ((float) nPx100_bme()) / 100;
    //xSemaphoreGive(xMutex);
    if (jsonBME.hasOwnProperty("Px100")) {
      fP = ((float)(int)jsonBME["Px100"]) / 100;
      String szP = String(fP);
      Serial.print("P: "); Serial.print(szP); Serial.println(" hPa");
      if (fP == 0) {
        Serial.println("Starting I2C again!");
        vSetupScreen();
        vSetupBME280();
      }
      if (fP > 869 && fP < 1100)
        client.publish( String("/" + String(sMac) + TOPIC_P).c_str(), szP.c_str());
      else
        Serial.println("Pressure out of range!");
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_ALT).c_str()) {
    //xSemaphoreTake(xMutex, portMAX_DELAY);
    //fAlt = ((float) nAx100_bme(SEALEVELPRESSURE_HPA)) / 100;
    //xSemaphoreGive(xMutex);
    if (jsonBME.hasOwnProperty("Ax100")) {
      fAlt = ((float)(int)jsonBME["Ax100"]) / 100;
      String szAlt = String(fAlt);
      Serial.print("A: "); Serial.print(szAlt); Serial.println(" m");
      client.publish( String("/" + String(sMac) + TOPIC_ALT).c_str(), szAlt.c_str());
    }
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_BIG_TEXT).c_str()) {
    Serial.print("Big text: "); Serial.println(szPayload);
    xSemaphoreTake(xMutex, portMAX_DELAY);
    vScreen24pixelText(0, 30, szPayload);
    xSemaphoreGive(xMutex);
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_MEDIUM_TEXT).c_str()) {
    Serial.print("Medium text: "); Serial.println(szPayload);
    xSemaphoreTake(xMutex, portMAX_DELAY);
    vScreen16pixelText(0, 30, szPayload);
    xSemaphoreGive(xMutex);
  }
  if (szTopic == String("/" + String(sMac) + TOPIC_SMALL_TEXT).c_str()) {
    Serial.print("Small text: "); Serial.println(szPayload);
    xSemaphoreTake(xMutex, portMAX_DELAY);
    vScreen10pixelText(0, 30, szPayload);
    xSemaphoreGive(xMutex);
  }
}

void mqttconnect() {
  const TickType_t xDelay = 5000 / portTICK_PERIOD_MS; // 5 seconds

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
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_LATCHING_RELAY_STATE).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_LDR).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_T).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_RH).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_P).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_ALT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_SMALL_TEXT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_MEDIUM_TEXT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_BIG_TEXT).c_str());
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      //delay(5000);
      vTaskDelay(xDelay);
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

void vReconnectWifiMqtt() {
  vSetupWifi();
  szGetMac().toCharArray(sMac, MAC_SIZE);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  vSetupMqtt();
  mqttconnect();
}

void vConnectingMqttsTask(void *parameter) {
  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();

  for (;;) {
    if (!client.connected()) {
      vReconnectWifiMqtt();
    }
    client.loop();
    vSupervisingButtons();
    vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
  }
  vTaskDelete(NULL); // There is an infinite loop before. This line will never be reached.
}

#endif         /* USING_WIFI */

void vReadingSensorBME(void *parameter) {
  int nTx100, nPx100, nRHx100, nGr, nAx100;
  float fT, fP, fRH, fGr, fA;
  uint32_t now;
  static uint32_t lastNow = 0, nTimes = 0;

  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();

  for (;;) {
    now = millis();
    if (now > lastNow + 1000) // Every second
    {
      lastNow = now;
      xSemaphoreTake(xMutex, portMAX_DELAY);
      vReadingBME280(&nTx100, &nPx100, &nRHx100, &nGr, &nAx100);
      xSemaphoreGive(xMutex);
      fT = ((float)nTx100) / 100; fP = ((float)nPx100) / 100; fRH = ((float)nRHx100) / 100; fGr = (float)nGr; fA = ((float)nAx100) / 100;
      if (fT > -100.0) {
        jsonBME["Tx100"] = nTx100; jsonBME["RHx100"] = nRHx100; jsonBME["Px100"] = nPx100; jsonBME["G"] = nGr; jsonBME["Ax100"] = nAx100;
        /*
          String jsonString = JSON.stringify(jsonBME);
          Serial.print("JSON.stringify(jsonBME) = ");
          Serial.println(jsonString);
        */
      } else {
        nTimes++; Serial.print(nTimes); Serial.println(") Setting I2C up again.");
        vSetupScreen();
        vSetupBME280();
      }
    }
    vTaskDelay(10); // To avoid: Task watchdog got triggered. The following tasks did not reset the watchdog in time
  }
  vTaskDelete(NULL); // There is an infinite loop before. This line will never be reached.
}

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);

  vSetupIO();
  vSetupScreen();
  vSetupBME280();

  delay(1000);
  xMutex = NULL;

#ifdef USING_WIFI
  xTaskCreate(
    vConnectingMqttsTask,    /* Task function. */
    "Connecting MQTTS Task", /* name of task. */
    //10000,                   /* Stack size of task */
    8192,  /* Stack size of task */
    NULL,  /* parameter of the task */
    1,     /* priority of the task */
    NULL); /* Task handle to keep track of created task */
#endif         /* USING_WIFI */

  xTaskCreate(
    vReadingSensorBME,    /* Task function. */
    "Reading Sensonr BME280 or BME680", /* name of task. */
    //10000,                   /* Stack size of task */
    8192,  /* Stack size of task */
    NULL,  /* parameter of the task */
    1,     /* priority of the task */
    NULL); /* Task handle to keep track of created task */
}

void loop() {
  vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
}

