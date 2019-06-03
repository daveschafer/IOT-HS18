//
// By Group 5 @ HSLU I.BSCI.HS18.IOT
//

// Read Data from Co2 Sensor and print to Serial
// Returns an Air Quality Level:
// 1 = Fresh Air
// 2 = High Pollution
// 3 = Very High Pollution

// Connect Co2 Analog pin to A0
const int co2Pin = 0; 
int co2 = 0;

int read_co2() {
  int co2Analog = -1;
  co2Analog = analogRead(co2Pin);
  if (co2Analog < 100) {
    // Fresh Air --> Level 1
    co2 = 1;
  } else if (co2Analog < 300) {
    // High Pollution --> Level 2
    co2 = 2;
  } else if (co2Analog > 300) {
    // Very High Pollution --> Level 3
    co2 = 3;
  }
  return co2;
}

void setup(){
    Serial.begin(9600);
}
 
void loop(){
    if (Serial.available()) {
      String receivedMsg = Serial.readStringUntil('\n');

      if (receivedMsg.equals("read_co2")) {
        co2 = read_co2();
        // Send Co2 data to Serial (Raspberry)
        Serial.println(co2);
      }
    }
    delay(1000);
}
