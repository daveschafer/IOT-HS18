//
// By Group 5 @ HSLU I.BSCI.HS18.IOT
//

// Read Data from Co2 Sensor and print to Serial

#include "AirQuality.h"

AirQuality co2Sensor;

// Connect Co2 Analog pin to A0
const int co2Pin = 0;
int co2 = -1;

int read_c02() {
  co2 = co2Sensor.slope();
  return co2;
}

void setup(){
    Serial.begin(9600);
    co2Sensor.init(co2Pin);
}
 
void loop(){
    if (Serial.available()) {
      String receivedMsg = Serial.readStringUntil('\n');

      if (receivedMsg.equals("read_co2")) {
        co2 = read_c02();
        // Send Co2 data to Serial (Raspberry)
        Serial.println(co2);
      }
    }
    delay(1000);
}

ISR(TIMER2_OVF_vect)
{
    if(co2Sensor.counter==122)//set 2 seconds as a detected duty
    {
        co2Sensor.last_vol=co2Sensor.first_vol;
        co2Sensor.first_vol=analogRead(A0);
        co2Sensor.counter=0;
        co2Sensor.timer_index=1;
        PORTB=PORTB^0x20;
    }
    else
    {
        co2Sensor.counter++;
    }
}
