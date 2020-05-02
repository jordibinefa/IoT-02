#include "IoT-02_common.h"
#include "IoT-02_bme680.h"

void vSetupBME680() {  
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms;
}

int nTx100_bme(){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return 0;
  }
  return (int)(100*bme.temperature);
}

int nPx100_bme(){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return 0;
  }
  return (int)bme.pressure;
}

int nRHx100_bme(){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return 0;
  }
  return (int)(100*bme.humidity);
}

int nGr_bme(){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return 0;
  }
  return (int)bme.gas_resistance;
}

int nAx100_bme(int nSeaLevelPressure_hPa){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return 0;
  }
  // return (int)(100*bme.readAltitude(SEALEVELPRESSURE_HPA));
  return (int)(100*bme.readAltitude(nSeaLevelPressure_hPa));
}

void vReadingBME680(int* nTx100,int* nPx100,int* nRHx100,int* nGr,int* nAx100){
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  //Serial.print("Temperature = ");
  //Serial.print(bme.temperature);
  //Serial.println(" *C");
  *nTx100 = (int)(100*bme.temperature);

  //Serial.print("Pressure = ");
  //Serial.print(bme.pressure / 100.0);
  //Serial.println(" hPa");
  *nPx100 = (int)bme.pressure;

  //Serial.print("Humidity = ");
  //Serial.print(bme.humidity);
  //Serial.println(" %");
  *nRHx100 = (int)(100*bme.humidity);

  //Serial.print("Gas = ");
  //Serial.print(bme.gas_resistance / 1000.0);
  //Serial.println(" KOhms");
  *nGr = (int)bme.gas_resistance;
  
  //Serial.print("Approx. Altitude = ");
  //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  //Serial.println(" m");
  *nAx100 = (int)(100*bme.readAltitude(SEALEVELPRESSURE_HPA));

  //Serial.println();
  //delay(200);
}
