
#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;

// Start Receiver Code
// Ensure the sender and receiver have a shared ground connection! If the grounds aren’t connected then you won’t get any readings

#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.
SoftwareSerial SUART(3, 4); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 
char myData[10] = "";       // Creates a blank character array of size 10
int i = 0;
double data;

int counter = 0;

//Functions




void setup() {
  Serial.begin(9600);
  SUART.begin(9600);

  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
}

void loop() {
  data = bmp.readPressure()/100;
  Serial.println(data);//write(Data);
  SUART.print(data);//write(Data);
  SUART.println();   //Newline code (0x0A) as end mark

  

  if (counter < 1000) {
    counter+=50;
  } else {
    counter = 0;
  }
  
  Serial.println(counter);
  delay(400);
    //Serial.println(recieve(recieverPin));
}