//Arduno Security Systen
//made by twbf of Titanify

#include "IRremote.h"

const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 12;
const int receiver = 11;

IRrecv irrecv(receiver);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);//initialize the buzzer pin as an output
}

void loop() {
    float dist = distance();
    int result;
    while (dist < 20){
        for(int i=0;i<80;i++){
             digitalWrite(buzzer,HIGH);
             delay(2);//wait for 1ms
             digitalWrite(buzzer,LOW);
             delay(2);//wait for 1ms
         }
         if (irrecv.decode(&results)){
           result = translateIR();
           irrecv.resume(); // receive the next value
         }
         if (result == 22){
             dist = 21;
             delay(3000);
         }
    }
    delay(500);
}

float microsecondsToInches(long microseconds) {
    return (float(microseconds) / 74) / 2;
}

float distance(){
    long duration, cm;
    float inches;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    // convert the time into a distance
    inches = microsecondsToInches(duration);
    return inches;
}
int translateIR() {
    int result;
      switch(results.value){

      case 0xFFA25D: result=22;    break;          //POWER
      case 0xFFE21D: result=1;    break;          //FUNC/STOP
      case 0xFF629D: result=2;    break;          //VOL+
      case 0xFF22DD: result=3;    break;          //FAST BACK
      case 0xFF02FD: result=4;    break;          //PAUSE
      case 0xFFC23D: result=5;    break;          //FAST FOWARD
      case 0xFFE01F: result=6;    break;          //DOWN
      case 0xFFA857: result=7;    break;          //VOL-
      case 0xFF906F: result=8;    break;          //UP
      case 0xFF9867: result=9;    break;          //EQ
      case 0xFFB04F: result=10;    break;         //ST/REPT
      case 0xFF6897: result=11;    break;             //0
      case 0xFF30CF: result=12;    break;             //1
      case 0xFF18E7: result=13;    break;             //2
      case 0xFF7A85: result=14;    break;             //3
      case 0xFF10EF: result=15;    break;             //4
      case 0xFF38C7: result=16;    break;             //5
      case 0xFF5AA5: result=17;    break;             //6
      case 0xFF42BD: result=18;    break;             //7
      case 0xFF4AB5: result=19;    break;             //8
      case 0xFF52AD: result=20;    break;             //9
      case 0xFFFFFFFF: result=21;break;         //REAPEAT

      default:
        result=0;                    //OTHER

      }
  return result;
}
