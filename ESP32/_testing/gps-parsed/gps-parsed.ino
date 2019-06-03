//
// Adafruit Ultimate GPS module
//

#include <Adafruit_BME280.h>
#include <Adafruit_GPS.h>
#include <ArduinoJson.h>

Adafruit_BME280 bme;

// HW-Serial for GPS module
HardwareSerial mySerial = Serial2;
Adafruit_GPS GPS(&mySerial);

// Define Json Buffer
const size_t bufferSizeGps = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1);
DynamicJsonBuffer jsonBuffer(bufferSizeGps);

// Print raw GPS readings.
#define GPSECHO false

bool bmeReady = false;
bool gpsReady = false;

bool setupGPS() {
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // TODO: check if GPS was initialized
  return true;
}

// Read GPS Sensor location and return as Json
JsonObject& readGPS() {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);

  JsonObject& root = jsonBuffer.createObject(); 
  JsonArray& location = root.createNestedArray("gps");
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return root;  // we can fail to parse a sentence in which case we should just wait for another
  }
  
  if (GPS.fix) {
    location.add(GPS.latitude);
    location.add(GPS.longitude);
  } else {
    location.add(0.0);
    location.add(0.0);
  }

  return root;
}

void setup()  
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);

  gpsReady = setupGPS();
  
  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);


  if (!bme.begin()) {
    Serial.println("BME not found");
  } else {
    bmeReady = true;
  }
}

void loop()
{
  float temp;
  float pres;
  float hum;
  
  if (bmeReady) {
    temp = bme.readTemperature();
    pres = bme.readPressure() / 100F;
    hum = bme.readHumidity();
  }
  JsonObject& gpsData = readGPS();
  gpsData.printTo(Serial);
  Serial.println();
  delay(2000);
}
