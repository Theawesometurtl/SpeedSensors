/*
The code so far works like so:

a car runs our first pressure sensor starting a timer

no data will be stored about the until fastestCarPassTime time has passed, 100 loops, in this case, meaning nothing
will be triggered if the car is detected on the first pressure sensor multiple times

the car runs over the second pressure sensor
since the timer is still running, we can use our info about the car to calculate speed and determine direction
this data is now stored in an array, and printed to the serial monitor

(Alex) I am planning to write a python function that will be able to read our data through a serial port and format
it nicely into a file. This python function will be in a different folder. In order for that to work, we would
need to create a function on the arduino to print our array to the serial monitor. I don't know how we would trigger
this function to print out the array, but I'm assuming we would use a button, which would be one more thing to add
onto each arduino


*/
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.


SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 
char myData[10] = "";       // Creates a blank character array of size 10
int i = 0;



Adafruit_BMP280 bmp;
//these are for the realtime clock, which isn't being used at the moment
RTC_DS3231 rtc;
char t[32];

const double minPressure1 = 1013;// these minPressure values need to be manually tweaked so the pressure sensor only reads values larger when a car runs over them
const double minPressure2 = 1033;
//a time won't be logged for a car if it took longer to pass than slowestCarPassTime, as the timers won't reset until after slowestCarPassTime has passed:
const long slowestCarPassTime = 10000L;
const double metersDistance = 1; 


//we have two seperate timers so we can find the direction a car is going
long timer1 = -10000L; //these store the time in milliseconds it was when they started. In order to find how long it has been since then, simply subtract them from the millis() function
long timer2 = -10000L;
int z;
double pressure1, pressure2, carSpeed;

int carsPassed = 0;
int delayness = 0; //this should only be more than 0 for managable testing



void collectData(int time, bool direction) {
  //all the data here is sent to the other arduino via SUART because the RTC and SD librarys conflict, so this change would let this arduino handle the rtc and the other
  // arduino handle the SD card. We never actually connected the rtc
  carSpeed = metersDistance / (time / 1000); // finding speed in m/s
  carSpeed *= 3.6; //converting to km/h

  Serial.print("Writing to test.txt...");
  SUART.print("Time the car took to pass: ");
  delay(20);
  SUART.println(String(time));
  delay(20);
  // SUART.println(carSpeed);
  if (direction) {
    SUART.println("The car passed direction 1");//there isn't a good way to specify direction, but direction 1 means the car passed over sensor 1 first then sensor 2
  } else {                                      // and direction 2 is the opposite; the car passed over sensor 2 first, then sensor 1. The arduino this code runs on is sensor 1
    SUART.println("The car passed direction 2");// this arduino is also the arduino with a bmp280 sensor
  }
  delay(20);
  SUART.print("Number of cars passed: ");
  delay(20);
  SUART.println(String(carsPassed));
  delay(20);
  
  delay(20);
  Serial.println(String(time));
  delay(20);
  // SUART.println(carSpeed);
  if (direction) {
    Serial.println("The car passed direction 1");//there isn't a good way to specify direction, but direction 1 means the car passed over sensor 1 first then sensor 2
  } else {                                      // and direction 2 is the opposite; the car passed over sensor 2 first, then sensor 1. The arduino this code runs on is sensor 1
    Serial.println("The car passed direction 2");// this arduino is also the arduino with a bmp280 sensor
  }
  delay(20);
  Serial.print("Number of cars passed: ");
  delay(20);
  Serial.println(String(carsPassed));
  delay(20);
// close the file:
  Serial.println("done.");
  carsPassed++;
}

bool timerStarted(int timer) {
  if (millis() - timer < slowestCarPassTime) {
    return true;
    // Serial.print("TIMER IS STARTED");
  }
  // Serial.print(millis() - timer);
  // Serial.print("   ");
  // Serial.print(millis());
  // Serial.print("   ");
  // Serial.print(timer);
  // Serial.print("   ");
  // Serial.print(slowestCarPassTime);  
  // bool thing = millis() - timer < slowestCarPassTime;
  // Serial.print(thing);  
  return false;
}


void setup() {
  Serial.begin(9600);
  SUART.begin(9600);
  delay(100);
  Wire.begin();


  //everything else in setup is just to setup the pressure sensor
  while ( !Serial ) delay(100);   // wait for native usb
    Serial.println(F("BMP280 test"));
    unsigned status;
    status = bmp.begin(BMP280_ADDRESS);

    if (!status) {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                        "try a different address!"));
      Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
    }

    /* Default settings from the datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); 



}

void loop() {
  pressure1 = bmp.readPressure()/100; //this is in hpa
  

  if (pressure1 > minPressure1) {
    Serial.println("pressure1 exceeded");    
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer2)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer2, false);//the bool is for the direction the car went in, I don't know a better way to specify direction

    }
    //if the timer hasn't been reset yet, it gets reset now
    if (!timerStarted(timer1)) {
      timer1 = millis();
      Serial.println("timer1 reset");
    }
    
  }

  if (pressure2 > minPressure2) {
    Serial.println("pressure2 exceeded");
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer1)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer2, true);//the bool is for the direction the car went in, I don't know a better way to specify direction
    } 

    //if the timer hasn't been reset yet, it gets reset now
    if (!timerStarted(timer2)) {
      timer2 = millis();
      Serial.println("timer2 reset");
    }

  }


  byte n = SUART.available();
  if (n != 0)
  {
    char x = SUART.read(); 
    if (x != 0x0A)  //end mark no found
    {
      myData[i] = x;  //save ASCII coded data in array
      i++;
    }
    else
    {
      pressure2 = atoi(myData);  		 // getting the data in integer form
      i = 0;
      // Serial.print("sensor1  ");
      // Serial.println(pressure1);
      // Serial.print("sensor 2  ");
      // Serial.println(pressure2);
            
    }
  }

  delay(delayness);

}