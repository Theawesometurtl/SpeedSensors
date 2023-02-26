/*
NOT FINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


also kinda copy pasted from the other file, so it doesn't totally make sense out of context




































The code so far works like so:

a car runs our pressure sensor and the time it is currently is saved

a timer is started, so the car doesn't trigger the sensor multiple times
pretty simple


(Alex) I am planning to write a python function that will be able to read our data through a serial port and format
it nicely into a file. This python function will be in a different folder. In order for that to work, we would
need to create a function on the arduino to print our array to the serial monitor. I don't know how we would trigger
this function to print out the array, but I'm assuming we would use a button, which would be one more thing to add
onto each arduino


*/
const int minPressure = 10;

int times[100];
int timer = 0;
int carsPassed = 0;
int pressure;



//currently we don't have a way to trigger this, but this should send all the collected
//data through the serial port, to be either read through the serial monitor, or 
//made into a file using python.
void sendToSerialPort() {
  for (int i = 0; i < carsPassed - 1; i++) {
    Serial.println(times[i]);
    delay(100);


  }
}



void setup() {
  Serial.begin(9600);
}

void loop() {
  pressure1 = analogRead(0);
  pressure2 = analogRead(1);
  

  if (timer <= slowestCar) {//idk whether it's a good idea to let this num get bigger infinitely
    timer++1;
  }

  if (pressure1 > minPressure) {
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer, true);//the bool is for the direction the car went in, I don't know a better way to specify direction
    } else {
      //if the timer hasn't been reset yet, it gets reset now
      timer1 = 0;
    }
    


  }



}