/*
The code so far works like so:

a car runs our first pressure sensor starting a timer

no data will be stored about the until slowestCarWheel time has passed, 100 loops, in this case, meaning nothing
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
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.


SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 
char myData[10] = "";       // Creates a blank character array of size 10
int i = 0;



File myFile;
Adafruit_BMP280 bmp;

RTC_DS3231 rtc;
char t[32];

const double minPressure1 = 1013;
const double minPressure2 = 1033;
const int slowestCar = 10000;
const int slowestCarWheel = 100;
const int distance = 100;


char *times[100];
int speeds[100];
int carsPassed = 0;
//we have two seperate timers so we can find the direction a car is going
int timer1 = 0;
int timer2 = 0;
int z;

int delayness = 1000;
double carSpeed = 0;
double pressure1;
double pressure2;



void collectData(int time, bool direction) {
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  times[carsPassed] = t;
  carSpeed = distance / time;
  speeds[carsPassed] = carSpeed;

  
  myFile = SD.open("test.txt", FILE_WRITE);
 
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(t);
    myFile.println(carSpeed);
    if (direction) {
    myFile.println(1);
    } else {
      myFile.println(0);
    }
	// close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  carsPassed++;
}

bool timerStarted(int timer) {
  if (timer < slowestCarWheel) {
    return true;
  }
  return false;
}

//currently we don't have a way to trigger this, but this should send all the collected
//data through the serial port, to be either read through the serial monitor, or 
//made into a file using python.
void sendToSerialPort() {
  for (int i = 0; i < carsPassed - 1; i++) {
    Serial.print(times[i]);
    delay(100);
    Serial.println(speeds[i]);
    delay(100);


  }
}



void setup() {
  Serial.begin(9600);
  SUART.begin(9600);
  delay(100);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

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

    Serial.print("Initializing SD card...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin 
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
    // or the SD library functions will not work. 
    pinMode(10, OUTPUT);
  
    if (!SD.begin(10)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");

}

void loop() {
  pressure1 = bmp.readPressure()/100; //this is in hpa
  collectData(100, true);

  if (timer1 <= slowestCar) {//idk whether it's a good idea to let this num get bigger infinitely
    timer1++;
  }
  if (timer2 <= slowestCar) {
    timer2++;
  }
  

  if (pressure1 > minPressure1) {
    Serial.println("pressure1 exceeded");    
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer2)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer2, true);//the bool is for the direction the car went in, I don't know a better way to specify direction
    } else {
      //if the timer hasn't been reset yet, it gets reset now
      timer1 = 0;
      Serial.println("timer1 reset");
    }
    


  }

  if (pressure2 > minPressure2) {
    Serial.println("pressure2 exceeded");
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer1)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer2, false);//the bool is for the direction the car went in, I don't know a better way to specify direction
    } else {
      //if the timer hasn't been reset yet, it gets reset now
      Serial.println("timer2 reset");
      timer2 = 0;
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
      // Serial.print(pressure1);
      // Serial.print("sensor 2  ");
      // Serial.println(pressure2);
            
    }
  }

  delay(delayness);

}