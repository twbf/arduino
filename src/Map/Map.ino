#include "MeMCore.h"

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

void setup(){
  Serial.begin(9600);
}

void loop(){
  if (ultraSensor.distanceCm()<10){
      move(3,100);
  } else {
      move(1,100);
  }
  Serial.print(ultraSensor.distanceCm() );
  Serial.println(" cm");
  delay(100); /* the minimal measure interval is 100 milliseconds */
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

void forward(){

}
