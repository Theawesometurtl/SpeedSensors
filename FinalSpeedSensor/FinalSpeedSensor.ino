/*


*/
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
 
File myFile;
 

#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp; // I2C

RTC_DS3231 rtc;
char t[32];






const double minPressure = 100110;
const int slowestCarWheel = 200;
const int distance = 100;

int times[100];
int speeds[100];
int carsPassed = 0;
//we have two seperate timers so we can find the direction a car is going
int timer = 0;
int delayness = 10;
double pressure;

void collectData() {
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  
  times[carsPassed] = t; //IDK how to access realtime, this might be a huge problem
   myFile = SD.open("test.txt", FILE_WRITE);
 
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(t);
	// close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  carsPassed++;
}

bool timerStarted(int t) {
  if (t > slowestCarWheel) {
    timer = 0;
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
    Serial.println("works");
    delay(100);


  }
}



void setup() {
  Serial.begin(9600);
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
  pressure = bmp.readPressure(); //this is in pa
  //Serial.println(pressure);

  if (timer <= 100000) {
    timer++;
  }

  if (pressure > minPressure) {
    // Serial.println(pressure);
    if (timerStarted(timer)) {
      //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
      collectData();//the bool is for the direction the car went in, I don't know a better way to specify direction
    } else {
      //if the timer hasn't been reset yet, it gets reset now
      timer = 0;
    }
    


  }
  delay(delayness);

}