
#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;


int counter = 1;
const int recieverPin = D3; //White wire
const int broadcastPin = A0; //Olive wire

//Functions

void broadcast(int pin, double data) {
  analogWrite(pin, int(data));
}

double recieve(int pin) {
  int data = analogRead(pin);
  return double(data);
}



void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
  pinMode(recieverPin, INPUT); 
  pinMode(broadcastPin, OUTPUT);
  Serial.println("Board two active");
}

void loop() {
  if (counter < 1000) {
    counter+=50;
  } else {
    counter = 0;
  }
  
  double thing = bmp.readPressure()/100;
  Serial.println(counter);
  broadcast(broadcastPin, counter);
  delay(400);
    //Serial.println(recieve(recieverPin));
}