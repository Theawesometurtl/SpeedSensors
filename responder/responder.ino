
#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25
#include <SD.h>
#include <SPI.h>
File myFile;


Adafruit_BMP085 bmp;

// Start Receiver Code
// Ensure the sender and receiver have a shared ground connection! If the grounds aren’t connected then you won’t get any readings

#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.
SoftwareSerial SUART(2,3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 
char myData[100] = "";       // Creates a blank character array of size 10
int i = 0;
int data;

int counter = 0;

//Functions

 



void setup() {
  Serial.begin(9600);
  SUART.begin(9600);
  Serial.println("Initializing SD card...");
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

  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
}

void loop() {
  data = bmp.readPressure()/100;
  // Serial.println(data);//write(Data);
  SUART.print(data);//write(Data);
  SUART.println();   //Newline code (0x0A) as end mark

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
      Serial.print(myData);  
      i = 0;
      myFile = SD.open("test.txt", FILE_WRITE);
      if (myFile) {
        myFile.println(myData);
        myFile.close();

      } else {
        Serial.print("AWHFDSHF PAIN THIS ISN'T WKORTKING THE OPENING OF FILES WAHHHHHHH!!!");
      }
      
            
    }
  }

  if (counter < 1000) {
    counter+=50;
  } else {
    counter = 0;
  }
  
    //Serial.println(recieve(recieverPin));
}