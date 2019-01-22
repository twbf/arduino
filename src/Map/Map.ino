//Questions:
//what is the keyword for flash memory?
// A: PROGMEM - https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
// ----- this is not going to be simple, but short of separately running a raspberry pi and offloading
//       all large scale mapping to it (which would allow collabratively behaviour between multiple
//       bots), i don't think there is much in the way of alternatives
// ----- possible alternative: a dedicated higher end arduino (with 8KB of SRAM) only doing the mapping
//       functions and communicating over the single wire bus
//
//how to once you have avoided an obstacle come back to the original path? (needed in mapping)
// A: Maybe store the net time left and right but one would also have to use
//    the distance traveled to get back
//    so, I think that we have to use vectors and think about distance rather than time
// ----- yes, I think that is essentially correct, store a "current intended direction is blah blah", and
//       perhaps a "current adjusted direction to avoid obstacle at so and so is goo goo"
// ===== Thomas believes this is mostly comnpleted, other than:
//              --correcting left and right(don't know which is which)
//              --correcting coeficiants that convert time to distance (might be testing or using distance sensor)
//              --using corners to correct angle and location
//
//What is the syntax for a byte array?
// A: byte mapArray[32][32]; - https://forum.arduino.cc/index.php?topic=220385.0
// ----- there does seem to be some complexities about this sometimes and there is no range checking,
//       perhaps we should look for helpful libraries?
//


#include "MeMCore.h"  //MeUltrasonicSensor, MeDCMotor
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//objects used - from MeMCore
MeUltrasonicSensor ultraSensor(3);
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

//motor fields
int mode;
int time;
int speed;

//timing fields
int count = 0;
int last_checked = 0;
int delayCheck = 2000;
double time_to_distance = 0.004; //for each milisecond forward at 100 speed what is the distance
double time_to_angle = 0.0006; //for each milisecond turing at 100 speed what is the angle change
                              //in radians

//navigation fields
double current_x = 0, current_y = 0, intended_x = 100, intended_y = 0;  //just for now
double current_angle, intended_angle;  //in radians
int obstacle = 0;
int scan[5];
int bounce = 0;

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
    return millis(); //the number of milliseconds since program start
}

void stop(){
    move(1, 0);
}

void forward(){
    mode = 2;
    move(mode,speed);
    time = now();
}

void backward(){
    mode = 1;
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
                                                                // ----- we still need to test this, and potentially
                                                                //       filter out negatives and spurious values
    if(distance>0){
        return distance;
    } else {
        return 0.001;
    }
}


//left to right distance scanning
//Goal: store values in a array of distances to find obstacles
void lookAround(){ // currently not asychronous
    move(3,60);
    delay(325);
    scan[0] = distance();
    for(int i = 0; i<4; i++){
        move(4,60);
        delay(150);
        scan[i+1] = distance();
    }
    move(3,60);
    delay(325);
}

void uploadToCloud(int scan[]){
    //send scan[] or a list of the obstcles (ones under 10 cm) to map_data.php
    //sending scan[] will mean that more of the processing power is on the sever

    //Then when it comes time to update intended location the script can update it
    //     - this would mean all array math would be done in the cloud

    //Thomas probably can figure out how to make the php script work but not how to get server space
    //and thus ip/mac adresses to use. A couple years ago I set up a sever on my laptop but that
    //isn't going to work this time

    //is php or python the right route to go? We need a server side language. Python is maybe
    //better because it has better syntax and can do matrix math better and in the future any
    //machine learning is impossible to do in php.
    //  - for python I have found "Django" a high level library that will make comunicating
    //    simpler, assuming a wifi module will work
}

void setup(){
    Serial.print("START");
    speed = 100;
    last_checked = 0;
    Serial.begin(9600);
    pinMode(A7,INPUT);
}

void run(){
while(true){ //goes forever
    //adjust position
    //doing this every time will allow us to see a constant pregression of position
    int time_elapsed = now() - last_checked;
    if(mode == 1){
        current_x += time_to_distance * time_elapsed * cos(current_angle);
        current_y += time_to_distance * time_elapsed * sin(current_angle);
    } else if(mode == 2){
        current_x -= time_to_distance * time_elapsed * cos(current_angle);
        current_y -= time_to_distance * time_elapsed * sin(current_angle);
    } else if(mode == 3){  //left??
        current_angle -=time_to_angle * time_elapsed; 
    } else if(mode == 4){ //right??
        current_angle += time_to_angle * time_elapsed;
    } //mode == 0 not moving

    if (time_elapsed > delayCheck){
        //if current(x,y) == intended(x,y)
            //update intended(x/y) by some algoriethm

        //scan
        lookAround(); //sets scan

        // get minimum distance from scan
        int minDist = 400; //it is set equal to maximum distance
        for(int i = 0; i<5; i++){
            if(scan[i]<minDist){
                minDist = scan[i];
            }
        }

        if (minDist<15){  // detected obstacle

            //printing to serial
            Serial.print("OBSTACLE - distance: ");
            Serial.print(minDist);
            Serial.print(", count: ");
            Serial.println(count);

            //eventually put obstacle in map
            obstacle = 1;

            //decide which directing to turn based on obstacle thats further away in that direction
            //might have to change delayCheck if each time it is going too far or not enough left or right
            if((scan[0]+scan[1])<(scan[3]+scan[4])){ //right
                delayCheck = 400;
                bounce++;
                right();
            } else { //left
                delayCheck = 400;
                bounce++;
                left();
            }

        } else { //no obstacle - navagate twords intended (x,y)
            obstacle = 0;

            intended_angle = atan2(intended_y-current_y, intended_x-current_x); //gets the angle
            if((current_angle-intended_angle)>200 && bounce == 0){ //.2 is about 10 degrees  -- left??
                delayCheck = 400;
                left();
            } else if((current_angle-intended_angle)<-200 && bounce == 0){ // right??
                delayCheck = 400;
                right();
            } else {
                delayCheck = 700;
                bounce = 0;
                forward();
            }
            count = 0;
        }
        if(false){ // so it doesn't actually do it
            uploadToCloud(scan);
        }
        last_checked = now();
    }   //possibly "else" with delay such that it doesn't use too much processing
        // or do math with the array map
        // ----- this is an excellent concept, to go to the extreme end we could setup
        //       a queue of processing tasks with time estimates for completion to
        //       give us reasonably efficient use of the downtime whilst also
        //       maintaining responsiveness in the rest of the loop, but we should
        //       probably not jump straight to that level of insanity <--KB guesses this is JS talking
}
}

void loop(){
    if((0^(analogRead(A7)>10?0:1))){ //push to start
        delay(500); //slight delay so everthing is smooth
        run();
    }
}
