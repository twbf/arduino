#include "MeMCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

MeUltrasonicSensor ultraSensor(3);
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

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

void loop(){
    if ((now() - last_checked) > delayCheck){
        //Serial.print(", count: ");
        last_checked = now();
        dist = distance();
        if (dist<15){
            Serial.print("OBSTACLE - distance: ");
            Serial.print(dist);
            Serial.print(", count: ");
            Serial.println(count);
            delayCheck = 100;
            obstacle = 1;
            if (count == 0){
                ran = rand() % 2 + 1;
            }
            pickADirection();
            count++;
        } else {
            if (obstacle == 1){
                delayCheck = 500;
                pickADirection();
                obstacle = 0;
            } else {
                delayCheck = 100;
            }
            count = 0;
            forward();
        }
    }
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
void map(){
    int foo [5];
    bool done = true;
    while (done){
        move(3,100);
        move(3,100);
        foo[0] = ultraSensor.distanceCm();
        for(int i = 0; i<4; i++){
            move(4,100);
            foo[i+1] = ultraSensor.distanceCm();
        }
        move(3,100);
        move(3,100);
        if(true){ // it is mapped
            done = false;
        }
    }
}
