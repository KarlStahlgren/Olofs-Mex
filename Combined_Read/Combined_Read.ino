
#include <Arduino.h>
#include "HX711.h"

// Pin Declarations
int dataIn = 5;
int clockIn = 2;

//Pins for scale
const int LOADCELL_DOUT_PIN = 8;
const int LOADCELL_SCK_PIN = 6;

// Variables
int clock = 1;
int lastClock = 1;
unsigned long time = 0;
unsigned long timeStart = 0;
int out = 0;
int i=0;

int caliperMeasurement = 0;//output in mm*100

HX711 scale;

int data[24];//stores the measured value as an array of 24 bits

void setup() {
  // Pin Set Up for caliper
  pinMode(dataIn, INPUT);     
  pinMode(clockIn, INPUT);  

  Serial.begin(115200);//baud rate 

  //setup for scale
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-459.542);  //to calibrate scale
  scale.tare();               // reset the scale to 0
}


void loop(){
  readData();
}

void readData(){
  lastClock = clock;
  clock = digitalRead(clockIn);
 
  if (lastClock == 1 && clock == 0){//falling edge
    out = digitalRead(dataIn)+digitalRead(dataIn)+digitalRead(dataIn); // Tripple sampling to remove glitches

    if((micros() - time) > 800){//new 24 bits of data incoming
      i=0;
    }

    // if data in=1, then put that in data[]
    if (out > 1){
      data[i]=1;
    }
    else{
      data[i]=0;
    }
    i++;

    if(i>=24){//to reset i when all the data has been recieved
      i=0;
      for(int j=1; j<=15; j++){//bits 1-15 store the measured value where bit 1 is LSB
        caliperMeasurement += (data[j]<<(j-1));//converting from binary to decimal
      }

      if(data[21]==1){//bit 21 is sign bit
        caliperMeasurement = -caliperMeasurement;
      }
      
      Serial.println(caliperMeasurement);
      readWeight();//Read the scale at the same time as caliper
      caliperMeasurement=0;
    }
    //update time to end of this loop to be used 
    //to time how long next loop takes
    time = micros();
  }
}

void readWeight(){
  Serial.print(scale.get_units(),1);
  Serial.println("");
}