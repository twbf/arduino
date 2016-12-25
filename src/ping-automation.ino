#include <Servo.h>

Servo servoRight;
Servo servoLeft;

const int pingPin = 7;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  servoRight.attach(12);
  servoLeft.attach(13);
}

void loop() {
  if (distance() < 8){
      slowDown();
      backward(500);
      turnLeft(1000);
  }
  else{
      forward(100);
  }
}

float distance(){
    long duration;
    float inches;

    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    // The same pin is used to read the signal from the PING))): a HIGH
    // pulse whose duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    // convert the time into a distance
    inches = microsecondsToInches(duration);
    return inches;
}

float microsecondsToInches(long microseconds) {
  //turning microsecounds to inches.
  //74 n speed of sound
  return (float(microseconds) / 74) / 2;
}

void forward(int time){
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1300);
    delay(time);
}

void turnLeft(int time){
    servoLeft.writeMicroseconds(1300);
    servoRight.writeMicroseconds(1300);
    delay(time);
}

void turnRight(int time){
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1700);
    delay(time);
}

void backward(int time){
    servoLeft.writeMicroseconds(1300);
    servoRight.writeMicroseconds(1700);
    delay(time);
}

void slowDown(){
    for(int i=10; i>0; i--){
        servoLeft.writeMicroseconds(1500+(distance()-3)*50);
        servoRight.writeMicroseconds(1500-(distance()-3)*50);
        delay(100);
    }
}
