// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// This code is showing what is happening when accessing I2C
// usings tasks without synchronization.
//
// Code written by Jordi Binefa. 20200524

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"

#include "IoT-02_bme280.h"
Adafruit_BME280 bme;

void vReadTemperature() {
  float fTc;

  fTc = ((float)nTx100_bme()) / 100;
  String szTemp = String(fTc);
  Serial.print("T: "); Serial.println(szTemp);
}

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);

  vSetupIO();
  vSetupBME280();

  delay(1000);

  xTaskCreate(
    taskOne,          /* Task function. */
    "TaskOne",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    taskTwo,          /* Task function. */
    "TaskTwo",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

}

void loop() {
  static int nCounter = 0;
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1 second

  Serial.print("Main loop (counter: ");
  Serial.print(++nCounter); Serial.print(") --> ");
  vReadTemperature();

  vTaskDelay(xDelay);
}

void taskOne( void * parameter ) {
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1 second

  for ( int i = 0; i < 10; i++ ) {
    Serial.print("Task 1--> ");
    vReadTemperature();
    vTaskDelay(xDelay);
  }
  Serial.println("Ending task 1");
  vTaskDelete( NULL );
}

void taskTwo( void * parameter) {
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1 second

  for ( int i = 0; i < 10; i++ ) {
    Serial.print("Task 2--> ");
    vReadTemperature();
    vTaskDelay(xDelay);
  }
  Serial.println("Ending task 2");
  vTaskDelete( NULL );
}

