/*
we still have two problems, the arduino doesn't have enough ram to store enough data, and the arduino doesn't have
access to the actual time of day. These could both be solved by connecting to wifi, but we might not be close enough to wifi

There are ways to add additional ram to the arduino, but I'm not sure if there's a solution to the time problem.
We can also add a clock to the arduino, something like a ds3231, but it might be difficult

Basically, we either need a wifi adapter chip, or both a RAM chip and a RTC (realtime clock) chip
*/
const int minPressure = 10;
const int slowestCar = 10000;
const int slowestCarWheel = 100;
const int distance = 100;

int times[100];
int speeds[100];
int carsPassed = 0;
int timer1 = 0;
int timer2 = 0;
int pressure;
int speed;

void static collectData(int: time, bool: direction) {
  speed = distance / time;
  Serial.println(time + " " + speed);
  times[carsPassed] = 1; //IDK how to access realtime, this might be a huge problem
  speeds[carsPassed] = speed;
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
    if (timerStarted(timer2)) {
      collectData(timer2, direction);
    } else {
      timer1 = 0;
    }
    


  }

  if (pressure2 > minPressure) {
    if (timerStarted(timer1)) {
      collectData(timer2, direction);
    } else {
      timer2 = 0;
    }

  }






  
  if (pressure1 > minPressure) {//pressure check


    //check to see if this is the second time the car activated the sensor
    //the message won't trigger if a car stops on one side, or a certain amount of time passes between pressures
    if (timer < slowestCar && timer > slowestCarWheel) {
                                                        

    }
    timer = 0;//resetting the timer
  }
    
}
