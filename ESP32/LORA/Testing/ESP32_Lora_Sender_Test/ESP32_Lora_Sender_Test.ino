/*
  This is a simple example show the LoRa sended data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include <U8x8lib.h>

#define SDA     4    // GPIO4  -- SX127x's SDA
#define SCK     5    // GPIO5  -- SX127x's SCK
#define SCL     15   // GPIO15 -- SX127X's SCL
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define RST_LED 16   // GPIO16 -- OLED reset pin
#define LED     25   // GPIO25 -- LED Light pin
#define DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    8681E5  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true

#define V2   1

#ifdef V2 //WIFI Kit series V1 not support Vext control
  #define Vext  21
#endif

unsigned int counter = 0;

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void setFrequencyandSend()
{
    unsigned long startFreq = 868000000;
    unsigned long endFreq = 870000000;

    unsigned long i;
    unsigned long freq;
    unsigned long s;
    
    for (i = startFreq; i < endFreq; i = i + 50000)
    {
      LoRa.setFrequency(i);
      
      for (s = 1; s < 100; s++)
      {
        u8x8.setCursor(0, 0);
        u8x8.print("Sending packet: ");
  
        u8x8.setCursor(0, 1);
        u8x8.print(counter);

        u8x8.drawString(0, 3, "on Band: ");
        u8x8.setCursor(0, 4);
        u8x8.print(i);

        counter++;
        
        LoRa.beginPacket();
        LoRa.print("hello ");
        LoRa.print(counter);
        LoRa.endPacket();
        delay(50);
      }
      delay(1000);
    }
}

void setup()
{

  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.setCursor(0, 0);
  u8x8.print(0x3c);
  u8x8.print(SDA);
  u8x8.print(SCL);
  u8x8.print(RST_LED);
        
  pinMode(Vext,OUTPUT);
  pinMode(LED,OUTPUT);
  
  digitalWrite(Vext, LOW);    // set GPIO16 low to reset OLED
  u8x8.print("Starting");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  
  LoRa.setPins(SS,RST,DI00);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0x34);
  LoRa.enableCrc();
  
  
  Serial.begin(115200);
  
  if (!LoRa.begin(BAND))
  {
    u8x8.print("Starting LoRa failed!");
    while (1);
  }
  u8x8.print("LoRa Initial success!");
  delay(1000);
}

void loop()
{

  setFrequencyandSend();
  
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(250);                       // wait for a second
}
