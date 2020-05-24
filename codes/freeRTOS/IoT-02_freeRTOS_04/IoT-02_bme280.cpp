#include "IoT-02_common.h"
#include "IoT-02_bme280.h"

void vSetupBME280() {  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

int nTx100_bme(){
  return (int)(100*bme.readTemperature());
}

int nPx100_bme(){
  return (int)bme.readPressure();
}

int nRHx100_bme(){
  return (int)(100*bme.readHumidity());
}

int nGr_bme(){
  return 0; // It only has sense on BME680
}

int nAx100_bme(int nSeaLevelPressure_hPa){
  return (int)(100*bme.readAltitude(nSeaLevelPressure_hPa));
}

void vReadingBME280(int* nTx100,int* nPx100,int* nRHx100,int* nGr,int* nAx100){
  //Serial.print("Temperature = ");
  //Serial.print(bme.temperature);
  //Serial.println(" *C");
  *nTx100 = (int)(100*bme.readTemperature());

  //Serial.print("Pressure = ");
  //Serial.print(bme.pressure / 100.0);
  //Serial.println(" hPa");
  *nPx100 = (int)bme.readPressure();

  //Serial.print("Humidity = ");
  //Serial.print(bme.humidity);
  //Serial.println(" %");
  *nRHx100 = (int)(100*bme.readHumidity());

  //Serial.print("Gas = ");
  //Serial.print(bme.gas_resistance / 1000.0);
  //Serial.println(" KOhms");
  *nGr = 0; // It only has sense on BME680
  
  //Serial.print("Approx. Altitude = ");
  //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  //Serial.println(" m");
  *nAx100 = (int)(100*bme.readAltitude(SEALEVELPRESSURE_HPA));

  //Serial.println();
  //delay(200);
}
