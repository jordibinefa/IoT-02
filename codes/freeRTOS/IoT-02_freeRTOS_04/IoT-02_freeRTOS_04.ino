// https://github.com/jordibinefa/IoT-02/tree/master/codes
// things.cat
//
// Task synchronization while accessing I2C. Using Mutex.
//
// Code written by Jordi Binefa. 20200524

#include "IoT-02_pinout.h"
#include "IoT-02_common.h"

#define MINIMUM_DELAY_ALLOWING_MULTITASKING 50

#include "IoT-02_bme280.h"
Adafruit_BME280 bme;

SemaphoreHandle_t xMutex;

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
  xMutex = NULL;

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
  vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
}

void taskOne( void * parameter ) {
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1 second

  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();

  for ( int i = 0; i < 10; i++ ) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    Serial.print("Task 1 ("); Serial.print(i); Serial.print(") --> ");
    vReadTemperature();
    xSemaphoreGive(xMutex);

    vTaskDelay(xDelay);
  }
  Serial.println("Ending task 1");
  vTaskDelete( NULL );
}

void taskTwo( void * parameter) {
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1 second
  int nC = 0;

  if (xMutex == NULL)
    xMutex = xSemaphoreCreateMutex();

  for ( int n = 0; n < 5; n++) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    for ( int i = 0; i < 2; i++ ) {
      Serial.print("Task 2 ("); Serial.print(nC); Serial.print(") --> ");
      vReadTemperature();
      vTaskDelay(xDelay);
      nC++;
    }
    xSemaphoreGive(xMutex);
  }
  Serial.println("Ending task 2");
  vTaskDelete( NULL );
}

