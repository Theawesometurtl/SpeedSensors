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
const int minPressure = 10;
const int slowestCar = 10000;
const int slowestCarWheel = 100;
const int distance = 100;

int times[100];
int speeds[100];
int carsPassed = 0;
//we have two seperate timers so we can find the direction a car is going
int timer1 = 0;
int timer2 = 0;
int pressure;
int speed;

void static collectData(int: time, bool: direction) {
  speed = distance / time;
  Serial.println(time + " " + speed);
  times[carsPassed] = 1; //IDK how to access realtime, this might be a huge problem
  speeds[carsPassed] = speed;
  carspassed++;
}

bool static timerStarted(timer) {
  if (timer > slowestCar && timer < slowestCarWheel) {
    return true
  }
  return false
}



void setup() {
  Serial.begin(9600);
}

void loop() {
  pressure1 = analogRead(0);
  pressure2 = analogRead(1);
  
  if (timer <= slowestCar) {//idk whether it's a good idea to let this num get bigger infinitely
    timer1+=1;
    timer2+=1;
  }
  
  if (pressure1 > minPressure) {
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer2)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer2, true);//the bool is for the direction the car went in, I don't know a better way to specify direction
    } else {
      //if the timer hasn't been reset yet, it gets reset now
      timer1 = 0;
    }
    


  }

  if (pressure2 > minPressure) {
    //timer started function checks if the timer is in a certain range, meaning it would have been triggered already
    if (timerStarted(timer1)) {
      //store the data about the cars passed, direction, and speed
      collectData(timer2, false);//the bool is for the direction the car went in, I don't know a better way to specify direction
    } else {
      //if the timer hasn't been reset yet, it gets reset now
      timer2 = 0;
    }

  }

}