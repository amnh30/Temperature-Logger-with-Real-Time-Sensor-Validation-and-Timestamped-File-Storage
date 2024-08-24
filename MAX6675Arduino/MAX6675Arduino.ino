/*
GND --> GND
VCC --> 5V
SO  --> 7;
CS  --> 6;
SCK --> 5;
*/

#include "MAX6675.h"


#define SO 7
#define CS 6
#define SCK 5

MAX6675 thermoCouple(CS, SO, SCK);

uint32_t start, stop;


void setup() {
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.print("MAX6675_LIB_VERSION: ");
  Serial.println(MAX6675_LIB_VERSION);
  Serial.println();
  delay(250);

  SPI.begin();

  thermoCouple.begin();
  thermoCouple.setSPIspeed(4000000);
}


void loop() {
  delay(100);
  int status = thermoCouple.read();
  float temp = thermoCouple.getTemperature();


  Serial.print("temp: ");
  Serial.print(temp);
  Serial.print("\n");


  delay(1000);
}


//  -- END OF FILE --
