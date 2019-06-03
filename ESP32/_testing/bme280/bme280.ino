/***************************************************************************
  This is a sample for reading the BME280 temperature, humidity & pressure sensor 
  data live. These sensors use I2C or SPI to communicate,

  Hint: the default I2C address used by the library is (0x77), as in
  Adafruit_BME280.h. If your sensor board uses a different address you
  can set your own via the .begin(...) method.
  
  Wiring BME280 & Arduino Uno R3 => Connect BME280 to Arduino Uno R3 as follows:
  ===========================================================
  Connect GND to GND
  Connect VIN to 5VV
  Connect SCK to A5 (SCL)
  Connect SDI to A4 (SDA)
 ***************************************************************************/

#include <Adafruit_Sensor.h>
#include "Adafruit_BME280.h"

#define BME_SCK D4
#define BME_MISO D3
#define BME_MOSI D2
#define BME_CS D5
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void setup() {
  Serial.begin(9600);
  Serial.println("Reading BME280 TEMP, HUMIDITY, PRESSURE DATA LIVE:");

  // if (!bme.begin(0x76)) {
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, verify wiring!");
    while (1);
  }
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
    delay(2000);
}
