/***************************************************
  By Group 5 @ HSLU I.BSCI.HS18.IOT
 ****************************************************/

// ######################
// ## Import Libraries ##
// ######################

#include <U8x8lib.h>
#include <LoRa.h>

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ###############################
// ## Defining Global Variables ##
// ###############################

/************************* Heltec LORA Setup *********************************/

String receivedText;
String receivedRSSI;
String receivedSNR;

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS 18
#define RST 14
#define DI0 26

//Defining The Frequency 868.100000 - used in europe
#define BAND 8681E5

//Defining Spreading Factor from 6 (highest Bandwith, least Distance) to 12 (lowest Bandwith, highest Distance)
#define spreadingFactor 12

//Define txPower (from 2 to 14)
#define txPower 14

//Define Signal Bandwith
#define signalBandwidth 125E3

// Use the OLED
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/15, /* data=*/4, /* reset=*/16);

/************************* WiFi Access Point *********************************/

const char *WLAN_SSID = "Inis iPhone";
const char *WLAN_PASS = "---";

WiFiClient client;

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // use 8883 for SSL -> nope
#define AIO_USERNAME "hslu_iot_hs18"
#define AIO_KEY "fb1e2ed9d91848ea9f0f43d85e960fb0"

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Define all the Used Feeds here (T/H/P/GPS)
Adafruit_MQTT_Publish FeedTemperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature-rpi");
Adafruit_MQTT_Publish FeedHumidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity-rpi");
Adafruit_MQTT_Publish FeedPressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pressure-rpi");
Adafruit_MQTT_Publish FeedGPS = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gps-rpi");
Adafruit_MQTT_Publish FeedCO2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/co2-rpi");



// #######################
// ##     Functions     ##
// #######################

  //needs to be defined empty, some kind of Arduino Bug
  void MQTT_connect();

  //connect to the wifi
  void setup_WiFi()
  {
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    WiFi.setHostname("esp32gw001");

    Serial.print("Connecting to: ");
    Serial.println(WLAN_SSID);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(2000);
      Serial.println("Connecting to WiFi..");
    }
    delay(10);

    Serial.println();
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  //Printing nice headers
  void print_header()
  {
    Serial.println("****************************************");
    Serial.println("****************************************");
    Serial.println("**   Heltec ESP LORA Gateway Kit v2   **");
    Serial.println("**       Serial: OK                   **");
    Serial.println("**       WiFi:   OK                   **");
    Serial.println("**       LoRa:   OK                   **");
    Serial.println("****************************************");
    Serial.println("****************************************");
  }

  //Parse Messages and send them to Adafruit.IO
  void parsing_lora_packets_and_send_to_AIO(String localreceivedText)
  {
    Serial.println("");
    Serial.println(localreceivedText);
    
    int doppelpunktIndex = localreceivedText.indexOf(':');
    
    String type = localreceivedText.substring(0, doppelpunktIndex);
    String value = localreceivedText.substring(doppelpunktIndex + 1);
    
    Serial.println("type: ");
    Serial.print(type);
    
    Serial.println("value: ");
    Serial.print(value);

    //Gotta Convert the string Ltype to a char array first, to compare it in th if-else-ladder
    const char* Ltype_ca = type.c_str();
    const char* Lvalue_ca = value.c_str();

    Serial.println(Ltype_ca);
    Serial.println(Lvalue_ca);
    
    //If Else Ladder, Kudos to dylnmc
    if (strcmp(Ltype_ca, "temperature") == 0) 
    {
      if (! FeedTemperature.publish(Lvalue_ca)) {
        Serial.println("Failed to send Temperature");
      }
      else {
        Serial.println("Sent Temperature to Adafruit.IO: ");
        Serial.print(value);
      }
    } 
    else if (strcmp(Ltype_ca, "humidity") == 0)
    {
      if (! FeedHumidity.publish(Lvalue_ca)) {
        Serial.println("Failed to send Humidity");
      }
      else {
        Serial.println("Sent Humidity to Adafruit.IO: ");
        Serial.print(value);
      }
    }
    else if (strcmp(Ltype_ca, "pressure") == 0)
    {
      if (! FeedPressure.publish(Lvalue_ca)) {
        Serial.println("Failed to send Pressure");
      }
      else {
        Serial.println("Sent Pressure to Adafruit.IO: ");
        Serial.print(value);
      }
    }
    else if (strcmp(Ltype_ca, "co2") == 0)
    {
      if (! FeedCO2.publish(Lvalue_ca)) {
        Serial.println("Failed to send CO2");
      }
      else {
        Serial.println("Sent CO2 to Adafruit.IO: ");
        Serial.print(value);
      }
    }
     else if (strcmp(Ltype_ca, "gps") == 0)
    {
      if (! FeedGPS.publish(Lvalue_ca)) {
        Serial.println("Failed to send GPS");
      }
      else {
        Serial.println("Sent GPS Data to Adafruit.IO: ");
        Serial.print(value);
      }
    }
    else /* default: */
    {
      Serial.print("not a valid LORA type: ");
      Serial.println(type);
      Serial.print("Message: ");
      Serial.println(value);
    }
  }

  // Function to connect and reconnect as necessary to the MQTT server.
  // Should be called in the loop function and it will take care if connecting.
  void MQTT_connect() {
    
    int8_t ret;
    
    // Stop if already connected.
    if (mqtt.connected()) {
      return;
    }
    
    Serial.println("Connecting to MQTT... ");

    uint8_t retries = 3;
    
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
          // basically die and wait for WDT to reset me
          while (1);
        }
    }
    Serial.println("MQTT Connected!");
  }


// #######################
// ##       Setup       ##
// #######################

void setup()
{
  delay(10); 
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  Serial.begin(115200);
  while (!Serial);
  
  //if just the the basic function, must connect to a computer
  delay(1000);
  
  //Print Header
  print_header();
  
  //setup WiFi
  setup_WiFi();
 
  // Starting OLED
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, "LoRa Receiver");

  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(1, 0, "Starting LoRa failed!");
    while (1);
  }
}

// #######################
// ##       Loop        ##
// #######################

void loop()
{
  // Establish MQTT Connection
  MQTT_connect();

  // ** Try to parse packet **
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // ** Receiving LORA Package **
    Serial.print("Received packet: ");
    u8x8.drawString(0, 2, "P-ID");

    String komplett;

    // ** Reading Lora Package **
    while (LoRa.available())
    {
      receivedText = (char)LoRa.read();
      Serial.print(receivedText);
      char currentid[64];
      receivedText.toCharArray(currentid, 64);
      //print packet on OLED
      u8x8.drawString(0, 3, currentid);

      komplett = komplett + receivedText;
    }

    // ** Parsing the Received receivedText ** | ** Send to Adafruit.IO **
    // If it contains any of these --> send to Adafruit
    // temperature: | humidity: | pressure: | gps:
    //parsing anhand der globalen Variable receivedText
    parsing_lora_packets_and_send_to_AIO(komplett);
    

    // ** Print RSSI of packet**
    receivedRSSI = LoRa.packetRssi();
    Serial.print(" with RSSI ");
    Serial.println(receivedRSSI);
    
    u8x8.drawString(0, 4, "RSSI");
    
    char currentRSSI[64];
    receivedRSSI.toCharArray(currentRSSI, 64);
    
    u8x8.drawString(0, 5, currentRSSI);
    
    // ** Print SNR of packet **
    receivedSNR = LoRa.packetSnr();
    Serial.print(" with SNR ");
    Serial.println(receivedSNR);

    u8x8.drawString(0, 6, "SNR");

    char currentSNR[64];
    receivedSNR.toCharArray(currentSNR, 64);
    
    u8x8.drawString(0, 7, currentSNR);

    Serial.println("=====================================");
  }
}
