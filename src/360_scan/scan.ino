#include "MeMCore.h"  //MeUltrasonicSensor, MeDCMotor
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Rundown by Thomas
//Goal: to make a 360 degree scan of a room with the MeUltrasonicSensor to understand the data coming back from the sensor
//Result: successful using a MacBook with python3. Thomas realized that the data coming back from the MeUltrasonicSensor
//        weren't accurate and transformations won't fundamentally change this. Look at the graph (ex_rectangular_room.png) which was
//        done on a rectangular room. Note that it isn't rectangular!
//
//This program is in two parts. Part 1 (scan.ino) completes the scan and prints out the numbers in the serial.
//Part 2 (ploty.py) displays the data that you copied from the serial panel. Yes it is awkward.

//objects used - from MeMCore
MeUltrasonicSensor ultraSensor(3);
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

void move(int direction, int speed) {
      int leftSpeed = 0;
      int rightSpeed = 0;
      if(direction == 1){
        	leftSpeed = speed;
        	rightSpeed = speed;
      }else if(direction == 2){
        	leftSpeed = -speed;
        	rightSpeed = -speed;
      }else if(direction == 3){
        	leftSpeed = -speed;
        	rightSpeed = speed;
      }else if(direction == 4){
        	leftSpeed = speed;
        	rightSpeed = -speed;
      }
      motor_9.run((9)==M1?-(leftSpeed):(leftSpeed));
      motor_10.run((10)==M1?-(rightSpeed):(rightSpeed));
}

double distance(){
    double distance = ultraSensor.distanceCm();
    if(distance>0){
        return distance;
    } else {
        return 0.001;
    }
}

void setup(){
    Serial.begin(9600);
    for(int i = 0; i<108; i++){
        move(3,60);
        delay(66);
        move(3,0);
        delay(200);
        Serial.print(distance());
        Serial.print(", ");
    }
}

void loop (){


}
