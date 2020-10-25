// Based on https://github.com/G6EJD/BME680-Example/blob/master/ESP32_bme680_CC_demo_03.ino
// Adapted code to IoT-02 by Jordi Binefa - 20201018
// things.cat / electronics.cat / wiki.binefa.cat / binefa.com
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define VERSIO "1.0"
#define USING_WIFI

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"
#include <Wire.h>
#include "IoT-02_oled.h"
SSD1306  display(0x3c, I2C_SDA, I2C_SCL);

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

int   humidity_score, gas_score;
int   getgasreference_count = 0;
int   gas_lower_limit = 10000;  // Bad air quality limit
int   gas_upper_limit = 300000; // Good air quality limit
float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score
float gas_reference = 2500;
float hum_reference = 40;
double lfT, lfP , lfRH;
float air_quality_score;
bool bPrimeraLectura, bMqttsConnectat;

enum {hazardous, veryUnhealthy, unhealthy, unhealthySensitiveGroups, moderate, good, noReadings} enState;
String sJson;

SemaphoreHandle_t xMutex;

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
String sIP;
#endif         /* USING_WIFI */

#define MINIMUM_DELAY_ALLOWING_MULTITASKING 50

void TaskBlink( void *pvParameters );
void TaskBME680( void *pvParameters );
void TaskReadButton_i34( void *pvParameters );
#ifdef USING_WIFI
void vConnectingMqttsTask( void *pvParameters );


String ip2Str(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
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
  /*
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
    if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_T).c_str()) {
    fTc = ((float)nTx100_bme()) / 100;
    String szTemp = String(fTc);
    Serial.print("T: "); Serial.println(szTemp);
    client.publish( String("/" + String(sMac) + TOPIC_T).c_str(), szTemp.c_str());
    }
    if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_RH).c_str()) {
    fRH = ((float) nRHx100_bme()) / 100;
    String szRH = String(fRH);
    Serial.print("RH: "); Serial.print(szRH); Serial.println(" %");
    client.publish( String("/" + String(sMac) + TOPIC_RH).c_str(), szRH.c_str());
    }
    if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_P).c_str()) {
    fP = ((float) nPx100_bme()) / 100;
    String szP = String(fP);
    Serial.print("P: "); Serial.print(szP); Serial.println(" hPa");
    client.publish( String("/" + String(sMac) + TOPIC_P).c_str(), szP.c_str());
    }
    if (szTopic == String("/" + String(sMac) + TOPIC_REQUEST_ALT).c_str()) {
    fAlt = ((float) nAx100_bme(SEALEVELPRESSURE_HPA)) / 100;
    String szAlt = String(fAlt);
    Serial.print("A: "); Serial.print(szAlt); Serial.println(" m");
    client.publish( String("/" + String(sMac) + TOPIC_ALT).c_str(), szAlt.c_str());
    }
    if (szTopic == String("/" + String(sMac) + TOPIC_BIG_TEXT).c_str()) {
    Serial.print("Big text: "); Serial.println(szPayload);
    vScreen24pixelText(0, 30, szPayload);
    }
    if (szTopic == String("/" + String(sMac) + TOPIC_MEDIUM_TEXT).c_str()) {
    Serial.print("Medium text: "); Serial.println(szPayload);
    vScreen16pixelText(0, 30, szPayload);
    }
    if (szTopic == String("/" + String(sMac) + TOPIC_SMALL_TEXT).c_str()) {
    Serial.print("Small text: "); Serial.println(szPayload);
    vScreen10pixelText(0, 30, szPayload);
    }*/
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
  /*
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  */
  vSetupMqtt();
  mqttconnect();

}

void vConnectingMqttsTask(void *parameter) {

  //Serial.println("0) vConnectingMqttsTask");

  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      //Serial.println("1) vConnectingMqttsTask");
      bMqttsConnectat = false;
      sIP = "0.0.0.0";
      vReconnectWifiMqtt();
      if (WiFi.status() == WL_CONNECTED) {
        sIP = ip2Str(WiFi.localIP());
      }
      Serial.println("IP address: ");
      Serial.println(sIP);
    } else {
      if (client.connected()) {
        bMqttsConnectat = true;
        client.loop();
      } else {
        bMqttsConnectat = false;
        mqttconnect();
      }
    }

    /*
      if (WiFi.status() == WL_CONNECTED) {
       sIP = ip2Str(WiFi.localIP());
       szGetMac().toCharArray(sMac, MAC_SIZE);
      }
    */

    /*
      if (!client.connected())
      vReconnectWifiMqtt();
      else
      client.loop();
    */
    vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
  }
  vTaskDelete(NULL); // There is an infinite loop before. This line will never be reached.
}

#endif         /* USING_WIFI */

// Starting BME680 functions {
void GetGasReference() {
  // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
  //Serial.println("Getting a new gas reference value");
  int readings = 10;
  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();

  for (int i = 1; i <= readings; i++) { // read gas for 10 x 0.150mS = 1.5secs
    xSemaphoreTake(xMutex, portMAX_DELAY);
    gas_reference += bme.readGas();
    xSemaphoreGive(xMutex);
  }
  gas_reference = gas_reference / readings;
  //Serial.println("Gas Reference = "+String(gas_reference,3));
}

String CalculateIAQ(int score) {
  String IAQ_text = "air quality is ";
  score = (100 - score) * 5;
  if      (score >= 301)                  {
    IAQ_text += "Hazardous";
    enState = hazardous;
  }
  else if (score >= 201 && score <= 300 ) {
    IAQ_text += "Very Unhealthy";
    enState = veryUnhealthy;
  }
  else if (score >= 176 && score <= 200 ) {
    IAQ_text += "Unhealthy";
    enState = unhealthy;
  }
  else if (score >= 151 && score <= 175 ) {
    IAQ_text += "Unhealthy for Sensitive Groups";
    enState = unhealthySensitiveGroups;
  }
  else if (score >=  51 && score <= 150 ) {
    IAQ_text += "Moderate";
    enState = moderate;
  }
  else if (score >=  00 && score <=  50 ) {
    IAQ_text += "Good";
    enState = good;
  }
  //Serial.print("IAQ Score = " + String(score) + ", ");
  return IAQ_text;
}

int GetHumidityScore() {  //Calculate humidity contribution to IAQ index
  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();
  xSemaphoreTake(xMutex, portMAX_DELAY);
  float current_humidity = bme.readHumidity();
  xSemaphoreGive(xMutex);

  if (current_humidity >= 38 && current_humidity <= 42) // Humidity +/-5% around optimum
    humidity_score = 0.25 * 100;
  else
  { // Humidity is sub-optimal
    if (current_humidity < 38)
      humidity_score = 0.25 / hum_reference * current_humidity * 100;
    else
    {
      humidity_score = ((-0.25 / (100 - hum_reference) * current_humidity) + 0.416666) * 100;
    }
  }
  return humidity_score;
}

int GetGasScore() {
  //Calculate gas contribution to IAQ index
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100.00;
  if (gas_score > 75) gas_score = 75; // Sometimes gas readings can go outside of expected scale maximum
  if (gas_score <  0) gas_score = 0;  // Sometimes gas readings can go outside of expected scale minimum
  return gas_score;
}

void vSetupBme680() {
  xMutex = NULL;
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    vScreen24pixelText(0, 20, "NO BME680!");
    while (1);
  } else {
    //vScreen24pixelText(0, 20, "Qualit AIRE");
    vPantallaInicial(VERSIO);
    Serial.println("BME680 trobat!");
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms
  // Now run the sensor to normalise the readings, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
  // The sensor takes ~30-mins to fully stabilise
  GetGasReference();
}

// } BME680 functions

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  pinMode(BT_I35, INPUT);
  bMqttsConnectat = false;
  bPrimeraLectura = false;
  enState = noReadings;
  vSetupScreen();
  vSetupBme680();
  sIP = "0.0.0.0";

#ifdef USING_WIFI
  xTaskCreatePinnedToCore(
    vConnectingMqttsTask,    /* Task function. */
    "Connecting MQTTS Task", /* name of task. */
    //10000,                   /* Stack size of task */
    8192,  /* Stack size of task */
    NULL,  /* parameter of the task */
    2,     /* priority of the task */
    NULL,
    ARDUINO_RUNNING_CORE);
#endif         /* USING_WIFI */

  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskBlink
    ,  "TaskBlink"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskBME680
    ,  "TaskBME680"
    ,  8192  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskReadButton_i34
    ,  "TaskReadButton_i34"
    ,  8192  // Stack size
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  Serial.println(__FILE__);
}

void loop() {
  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();
  xSemaphoreTake(xMutex, portMAX_DELAY);
  double lfT = bme.readTemperature();
  double lfP = bme.readPressure();
  double lfRH = bme.readHumidity();
  xSemaphoreGive(xMutex);
  /*
    Serial.println("Sensor Readings:");
    Serial.println("  Temperature = " + String(lfT, 2)     + "°C");
    Serial.println("     Pressure = " + String(lfP / 100.0F) + " hPa");
    Serial.println("     Humidity = " + String(lfRH, 1)        + "%");
    Serial.println("          Gas = " + String(gas_reference)               + " ohms\n");
    Serial.print("Qualitative Air Quality Index ");
  */
  humidity_score = GetHumidityScore();
  gas_score      = GetGasScore();

  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  air_quality_score = humidity_score + gas_score;
  //Serial.println(" comprised of " + String(humidity_score) + "% Humidity and " + String(gas_score) + "% Gas");
  if ((getgasreference_count++) % 5 == 0) GetGasReference();
  //Serial.println(CalculateIAQ(air_quality_score));
  CalculateIAQ(air_quality_score);
  int nIAQ = (100 - air_quality_score) * 5;
  sJson = "{ \"LDR\": "    + String(analogRead(LDR))
          + ", \"T\": "    + String(lfT, 2)
          + ", \"RH\": "   + String(lfRH, 1)
          + ", \"P\": "    + String(lfP / 100.0F)
          + ", \"IAQ\": "  + String(nIAQ)
          + ", \"G\": "    +  String(gas_reference)
          + ", \"Gs\": "    +  String(gas_score)
          + ", \"RHs\": "    +  String(humidity_score)
          + ", \"Estat\": "    + String(int(enState))
          + ", \"Llengua\": "  + "\"ca\""
          + "}";
  if (digitalRead(BT_I35))
    vScreenIAQ(int(enState), nIAQ);
  else
    vScreenMacIp(sMac, sIP);
  //Serial.println(sJson);

  bPrimeraLectura = true;
  vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskReadButton_i34(void *pvParameters) {
  (void) pvParameters;
  boolean bI34wasPressed = false;
  boolean bI34currentState;
  boolean bLedG = LOW;

  pinMode(LED_G, OUTPUT);
  pinMode(BT_I34, INPUT);

  //Serial.println("TaskReadButton_i34");
  for (;;) {
    bI34currentState = bPressedButton(BT_I34);
    if (bI34wasPressed != bI34currentState) {
      vTaskDelay(2);
      if (bI34currentState) {
        bLedG = !bLedG;
        Serial.println( (bLedG) ? "ON" : "OFF");
      }
      bI34wasPressed = bI34currentState;
    }
    //digitalWrite(LED_G, bLedG);
    vTaskDelay(10);
  }
}

void TaskBlink(void *pvParameters) {
  (void) pvParameters;

  pinMode(LED_W, OUTPUT);

  for (;;) {
    if (sIP == "0.0.0.0")
      digitalWrite(LED_W, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    digitalWrite(LED_W, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskBME680(void *pvParameters) {
  (void) pvParameters;
  // enum {hazardous, veryUnhealthy, unhealthy, unhealthySensitiveGroups, moderate, good, noReadings} enState;
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(SET_LATCHING_RELAY, OUTPUT);
  pinMode(RST_LATCHING_RELAY, OUTPUT);
  pinMode(STATE_LATCHING_RELAY, INPUT);

  boolean bLatchingRelayState;
  int n = 0, nMillis = millis();
  for (;;) {
    bLatchingRelayState = digitalRead(STATE_LATCHING_RELAY);
    switch (enState) {
      case hazardous:
      case veryUnhealthy: digitalWrite(LED_R, (n % 2) ? HIGH : LOW); digitalWrite(LED_Y, LOW); digitalWrite(LED_G, LOW);
        if (!bLatchingRelayState) vLatchingRelay(HIGH);
        break;
      case unhealthy: digitalWrite(LED_R, HIGH); digitalWrite(LED_Y, LOW); digitalWrite(LED_G, LOW);
        if (!bLatchingRelayState) vLatchingRelay(HIGH);
        break;
      case unhealthySensitiveGroups: digitalWrite(LED_Y, (n % 2) ? HIGH : LOW); digitalWrite(LED_R, LOW); digitalWrite(LED_G, LOW);
        if (!bLatchingRelayState) vLatchingRelay(HIGH);
        break;
      case moderate: digitalWrite(LED_R, LOW); digitalWrite(LED_Y, HIGH); digitalWrite(LED_G, LOW);
        if (bLatchingRelayState) vLatchingRelay(LOW);
        break;
      case good: digitalWrite(LED_R, LOW); digitalWrite(LED_Y, LOW); digitalWrite(LED_G, HIGH);
        if (bLatchingRelayState) vLatchingRelay(LOW);
        break;
      default: digitalWrite(LED_R, LOW); digitalWrite(LED_Y, LOW); digitalWrite(LED_G, LOW);
        //vLatchingRelay(LOW);
    }
    //(bLatchingRelayState)?Serial.println("Relay SET"):Serial.println("Relay RESET");
    n++;
    // Serial.println(millis()-nMillis);
    // nMillis = millis();
    if (bPrimeraLectura) {
      if (n % 75 == 0) {
        //Serial.println(sJson);
        if (bMqttsConnectat) {
          Serial.print("Tema: "); Serial.println(String("/" + String(sMac) + TOPIC_JSON_DATA).c_str());
          Serial.print("Contingut: "); Serial.println(sJson.c_str());
          client.publish( String("/" + String(sMac) + TOPIC_JSON_DATA).c_str(), sJson.c_str());
          //Serial.println("PUB");
        }
      }
    }
    vTaskDelay(200);
  }
}
