
#include "i2cLCD.h"

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
long total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;

i2cLCD mylcd;

char s[16];
unsigned long cnt = 0;

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);

  mylcd.init();

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  total = 0;
  for (int i = numReadings-1; i > 0; --i) {
    readings[i] = readings[i-1];
  }
  // read from the sensor:
  readings[0] = analogRead(inputPin);

  //  a[0]--a[N-1]までの合計を計算する
  for (int i = 0; i < numReadings; ++i) {
    total = total + readings[i];
  }

  // calculate the average:
  average = total / numReadings;

  if(cnt%10){
    mylcd.clear();
    sprintf(s, "A0 avr =   %04d", average);
    for(int i=0;i<16;i++){
      mylcd.writeData(s[i]);
    }
    mylcd.move(1,0);
    sprintf(s, "A0 raw =   %04d", readings[0]);
    for(int i=0;i<16;i++){
      mylcd.writeData(s[i]);
    }
  }
  cnt++;
  delay(100);        // delay in between reads for stability
}
