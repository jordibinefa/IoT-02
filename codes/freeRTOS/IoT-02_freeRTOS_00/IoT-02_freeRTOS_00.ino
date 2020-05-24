// https://techtutorialsx.com/2017/05/06/esp32-arduino-creating-a-task/

void setup() {
  Serial.begin(112500);
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
  
  Serial.print("Hello from main loop (counter: ");
  Serial.print(++nCounter); Serial.println(")");
  vTaskDelay(xDelay);
}
 
void taskOne( void * parameter ){
    for( int i = 0;i<10;i++ ){ 
        Serial.println("Hello from task 1");
        delay(1000);
    }
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
}
 
void taskTwo( void * parameter){ 
    for( int i = 0;i<10;i++ ){
        Serial.println("Hello from task 2");
        delay(1000);
    }
    Serial.println("Ending task 2");
    vTaskDelete( NULL );
}

