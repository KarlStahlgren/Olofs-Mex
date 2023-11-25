//Simple Digital Calliper Reader
//See http://j44industries.blogspot.com/

// Pin Declarations
int dataIn = 5;
int clockIn = 2;

// Variables
int clock = 1;
int lastClock = 1;
unsigned long time = 0;
unsigned long timeStart = 0;
int out = 0;
int i=0;

int measurement = 0;//output in mm*100

int data[24];//stores the measured value as an array of 24 bits

void setup() {
  // Pin Set Up
  pinMode(dataIn, INPUT);     
  pinMode(clockIn, INPUT);  

  Serial.begin(115200);
  //Serial.println("Ready:"); have to comment this out so python does not cry that data is a string!!
}


void loop(){
  ReadData();
}
void ReadData(){
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
        measurement += (data[j]<<(j-1));//converting from binary to decimal
      }

      if(data[21]==1){//bit 21 is sign bit
        measurement = -measurement;
      }
      
      Serial.println(measurement);
      measurement=0;
    }
    //update time to end of this loop to be used 
    //to time how long next loop takes
    time = micros();
  }
}
/* old code
void readData(){
  lastClock = clock;
  clock = digitalRead(clockIn);
 
  if (lastClock == 1 && clock == 0){
    //Serial.print("falling edge ");
    out = digitalRead(dataIn)+digitalRead(dataIn)+digitalRead(dataIn); // Tripple sampling to remove glitches
    //Serial.print("micros() - time: "); Serial.print(micros() - time);

    //Should we make a new line?
    if((micros() - time) > 800){
      //Serial.print("  micros()-time>800: "); Serial.print(micros()-time>800 );
      Serial.println(" ");
    }
    else if((micros() - time) > 400){
      Serial.print("  ");
    }

    // if data in=1, then print 1
    if (out > 1){
      Serial.print("1");
    }
    else{
      Serial.print("0");
    }
    Serial.print(",");

    //update time to end of this loop to be used 
    //to time how long next loop takes
    time = micros();
  }
}
*/