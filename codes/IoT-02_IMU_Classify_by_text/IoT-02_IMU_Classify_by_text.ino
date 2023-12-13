// IoT-02_IMU_Classify_by_text.ino
// https://eloquentarduino.com/gesture-classification/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "Pipeline.h"
#include "Classifier.h"

#define FREQUENCY_HZ        60
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

// objeto da classe Adafruit_MPU6050
Adafruit_MPU6050 mpu;

static unsigned long last_interval_ms = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);

  // Try to initialize!
  if (!mpu.begin()) { // if (!mpu.begin(0x68))
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");
  delay(100);}

void loop() {
  float ax, ay, az, gx, gy, gz;

  sensors_event_t a, g, temp;

  if (millis() > last_interval_ms + INTERVAL_MS) {
    last_interval_ms = millis();

    mpu.getEvent(&a, &g, &temp);
    
    ax = a.acceleration.x;  
    ay = a.acceleration.y;  
    az = a.acceleration.z;
    gx = g.gyro.x;  
    gy = g.gyro.y;  
    gz = g.gyro.z;  
  
    // perform feature extraction
    float features[] = {ax, ay, az, gx, gy, gz};
      
    if (!pipeline.transform(features))
      return;
      
    // perform classification
    Serial.print("Predicted gesture: ");
    Serial.print(forest.predictLabel(pipeline.X));
    Serial.print(" (DSP: ");
    Serial.print(pipeline.latencyInMicros());
    Serial.print(" micros, Classifier: ");
    Serial.print(forest.latencyInMicros());
    Serial.println(" micros)");
    Serial.println();
  }
}
