/**
 * Complete project details at https://RandomNerdTutorials.com/arduino-load-cell-hx711/
 *
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/

#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 8;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;

void setup() {
  //Serial.begin(57600);
  Serial.begin(115200);
  
  //Serial.println("HX711 Demo");
  //Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-459.542);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
}

void loop() {
  readData();
}

void readData(){
  Serial.print(scale.get_units(),1);
  Serial.println("");
}
