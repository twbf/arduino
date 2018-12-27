#include "MeMCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

MeUltrasonicSensor ultraSensor(3);
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

// for motor
int mode;
int time;
int speed;

int last_checked = 0;
int dist;
int count = 0;
int ran;
int obstacle = 0;

int delayCheck = 100;

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

int now(){
    return millis();
}

void stop(){
    move(1, 0);
}

void forward(){
    mode = 1;
    move(mode,speed);
    time = now();
}

void backward(){
    mode = 2;
    move(mode,speed);
    time = now();
}

void left(){
    mode = 3;
    move(mode,speed);
    time = now();
}

void right(){
    mode = 4;
    move(mode,speed);
    time = now();
}

double distance(){
    double distance = sqrt(pow(ultraSensor.distanceCm(),2)-9);
    //delay(100); // see what happens
    return distance;
}

void setup(){
    Serial.print("START");
    speed = 100;
    last_checked = 0;
    Serial.begin(9600);
}

int lookAround(){  //left to right?
    int foo [5];
    move(3,40);
    delay(50);
    foo[0] = distance();
    for(int i = 0; i<4; i++){
        move(4,40);
        foo[i+1] = distance();
    }
    move(3,40);
    delay(50);

    //get minumum
    int min = 400; //  it is set equal to maximum
    for(int i = 0; i<5; i++){
        if(foo[i]<min){
            min = foo[i];
        }
    }
    return min;
}

void loop(){
    if ((now() - last_checked) > delayCheck){
        last_checked = now();
        dist = lookAround();
        if (dist<10){  // detected obstacle
            Serial.print("OBSTACLE - distance: ");
            Serial.print(dist);
            Serial.print(", count: ");
            Serial.println(count);
            obstacle = 1;
            if (count == 0){
                ran = rand() % 2 + 1;
            }
            pickADirection();
            count++;
        } else {
            count = 0;
            forward();
        }
    }  //  possibly else with delay such that it doesn't use too much processing power
}

void pickADirection(){
    if (ran == 1){
        Serial.println("PICKED LEFT");
        left();
    } else { // 2
        Serial.println("PICKED RIGHT");
        right();
    }
}
