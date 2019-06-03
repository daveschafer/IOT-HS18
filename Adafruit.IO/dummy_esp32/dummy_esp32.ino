/***************************************************
  Adafruit MQTT Library and Wifi.h (for ESP32)
 ****************************************************/
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

const char* WLAN_SSID     = "SSID-HERE";
const char* WLAN_PASS     = "PASSWORD-HERE";

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL -> nope
#define AIO_USERNAME    "hslu_iot_hs18"
#define AIO_KEY         "fb1e2ed9d91848ea9f0f43d85e960fb0"

/************ Global State (you don't need to change this!) ******************/
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'esp32dummy' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
// Dieser Feed wird fuer das senden von Daten verwendet
Adafruit_MQTT_Publish esp32dummy = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dummy-esp32");

// Setup a feed called 'onoff' for subscribing to changes.
// Dieser Feed wird fuer das empfangen von Daten verwendet
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  
  //connect to the wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  WiFi.setHostname("esp32gw001");
  
  Serial.print("Connecting to: ");
  Serial.println(WLAN_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  delay(10);

  Serial.println(F("Adafruit MQTT demo (dummy)"));

  Serial.println();
  
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  //not needed for this demo
  //mqtt.subscribe(&esp32-onoffbutton);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

   //Stopping after 50 Values
    if (x >= 50){
      Serial.println("50 values sent, going to sleep...");
      delay(10000);
      return;
    }
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

 /* Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }    
  } */

  // Now we can publish stuff!
  Serial.print(F("\nSending Dummy val "));
  Serial.print(x);
  Serial.print("...");
  if (! esp32dummy.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }


  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */

  //wait for 2 seconds at the end of the function
  //why? because we can only send 30 posts per minute do adafruit.io for free
  delay(2010);
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
