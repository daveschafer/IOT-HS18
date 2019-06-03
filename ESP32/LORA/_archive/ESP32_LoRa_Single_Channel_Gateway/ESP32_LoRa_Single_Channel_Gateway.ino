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
String receivedRssi;

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

// Use the OLED
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/15, /* data=*/4, /* reset=*/16);

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


/************************* WiFi Access Point *********************************/

const char *WLAN_SSID = "SSID-HERE";
const char *WLAN_PASS = "PASSWORD-HERE";

WiFiClient client;

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
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
    delay(10);

    Serial.println();
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }


  void print_header()
  {
    Serial.println("****************************************")
    Serial.println("****************************************")
    Serial.println("**   Heltec ESP LORA Gateway Kit v2   **"));
    Serial.println("**       Serial: OK                   **"));
    Serial.println("**       WiFi: OK                     **"));
    Serial.println("**       LORA: OK                     **"));
    Serial.println("****************************************")
    Serial.println("****************************************")
  }

  //Parse Messages and send them to Adafruit.IO
  Adafruit_MQTT_Publish parsing_lora_packets_and_send_to_AIO()
  {
    //Parse and return the reference feed (temp, hum, gps...)
    String Ltype = String yval = getValue(receivedText, ':', 0);
    String Lvalue = String yval = getValue(receivedText, ':', 1);

    //If Else Ladder, Kudos to dylnmc
    if (strcmp(string, "temperature") == 0) 
    {
      if (! FeedTemperature.publish(Lvalue)) {
        Serial.println(F("Failed to send Temperature"));
      }
      else {
        Serial.print(F("Sent Temperature to Adafruit.IO: "))
        Serial.print(F(Lvalue))
      }
    } 
    else if (strcmp(string, "humidity") == 0)
    {
      if (! FeedHumidity.publish(Lvalue)) {
        Serial.println(F("Failed to send Humidity"));
      }
      else {
        Serial.print(F("Sent Humidity to Adafruit.IO: "))
        Serial.print(F(Lvalue))
      }
    }
    else if (strcmp(string, "pressure") == 0)
    {
      if (! FeedPressure.publish(Lvalue)) {
        Serial.println(F("Failed to send Pressure"));
      }
      else {
        Serial.print(F("Sent Pressure to Adafruit.IO: "))
        Serial.print(F(Lvalue))
      }
    }
     else if (strcmp(string, "gps") == 0)
    {
      if (! FeedGPS.publish(Lvalue)) {
        Serial.println(F("Failed to send GPS"));
      }
      else {
        Serial.print(F("Sent GPS Data to Adafruit.IO: "))
        Serial.print(F(Lvalue))
      }
    }
    else /* default: */
    {
      Serial.print("not a valid LORA type: " )
      Serial.println(Ltype)
      Serial.print("Message: ")
      Serial.println(Lvalue)
    }

    //referenceFeed.publish...

    //have to show a message when the feed is unknown, likely with:
    if (! esp32dummy.publish(x++)) {
      Serial.println(F("Failed"));
    }
    else {
      Serial.println(F("OK!"))
    }
  }


 // Splitting Strings efficiently
 // Kudos to H. Pauwelyn for this Code Snippet!
  String getValue(String data, char separator, int index)
  {
      int found = 0;
      int strIndex[] = { 0, -1 };
      int maxIndex = data.length() - 1;

      for (int i = 0; i <= maxIndex && found <= index; i++) {
          if (data.charAt(i) == separator || i == maxIndex) {
              found++;
              strIndex[0] = strIndex[1] + 1;
              strIndex[1] = (i == maxIndex) ? i+1 : i;
          }
      }
      return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
  }


  // Function to connect and reconnect as necessary to the MQTT server.
  // Should be called in the loop function and it will take care if connecting.
  void MQTT_connect() {
    int8_t ret;
    // Stop if already connected.
    if (mqtt.connected()) {
      return;
    }
    Serial.print("Connecting to MQTT... ");

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
  Delay(10) 
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

  Serial.println("LoRa Receiver");
  u8x8.drawString(0, 1, "LoRa Receiver");

  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
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
    Serial.print("Received packet '");
    u8x8.drawString(0, 4, "PacketID");

    // ** Reading Lora Package **
    while (LoRa.available())
    {
      receivedText = (char)LoRa.read();
      Serial.print(receivedText);
      char currentid[64];
      receivedText.toCharArray(currentid, 64);
      //print packet on OLED
      u8x8.drawString(9, 4, currentid);
    }

    // ** Parsing the Received receivedText ** | ** Send to Adafruit.IO **
    // If it contains any of these --> send to Adafruit
    // temperature: 
    // humidity: 
    // pressure: 
    // gps: 
    parsing_lora_packets_and_send_to_AIO(); //parsing anhand der globalen Variable receivedText


    // ** Print RSSI of packet**
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    u8x8.drawString(0, 5, "PacketRS");
    receivedRssi = LoRa.packetRssi();
    char currentrs[64];
    receivedRssi.toCharArray(currentrs, 64);
    u8x8.drawString(9, 5, currentrs);
  }
  //print line break at end of loop
  Serial.println("=====================================")
  //wait for 3 seconds at the end of the function
  //why? because we can only send 30 posts per minute do adafruit.io for free
  delay(3010);
}
