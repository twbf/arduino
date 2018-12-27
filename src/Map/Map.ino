//Questions:
//what is the keyword for flash memory?
//
//how to once you have avoided an obstacle come back to the original path? (needed in mapping)
// A: Maybe store the net time left and right but one would also have to use
//    the distance traveled to get back
//    so, I think that we have to use vectors and think about distance rather than time
//
//What is the syntax for a byte array?
//


#include "MeMCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//objects used
MeUltrasonicSensor ultraSensor(3);
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

//motor fields
int mode;
int time;
int speed;

//timing fields
int last_checked = 0;
int delayCheck = 100;

//maunuvering fields
int dist;
int count = 0;
int ran;
int obstacle = 0;

//from MeMCore example set
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
    return millis(); //not sure what numbers this returns
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
    double distance = sqrt(pow(ultraSensor.distanceCm(),2)-9);  //corects for close distance error
    return distance;
}

void setup(){
    Serial.print("START");
    speed = 100;
    last_checked = 0;
    Serial.begin(9600);
}


//left to right distance scanning
//Goal: store values in a array of distances to find obstacles
int lookAround(){
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

    //gets minumum
    int min = 400; //it is set equal to maximum distance
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
        } else { //no obstacle
            count = 0;
            forward();
        }
    }   //possibly "else" with delay such that it doesn't use too much processing
        // or do math with the array map
}

//picks a direction based off of "ran" (random) variable
void pickADirection(){
    if (ran == 1){
        Serial.println("PICKED LEFT");
        left();
    } else { // 2
        Serial.println("PICKED RIGHT");
        right();
    }
}
