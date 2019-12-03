//Arduino Line Follower Obstacle Panic Robot
//Created By DIY Builder & Project Team

#include<NewPing.h>
#include"main.h"
//#include<servo>

//Motion Motors
const int ENAB = 3;  //ENAB connected to digital pin 3
const int MOTOR_AF = 2; // MOTOR_AF connected to digital pin 2
const int MOTOR_AB = 4; // MOTOR_AB connected to digital pin 4
const int MOTOR_BF = 7; // MOTOR_BF connected to digital pin 7
const int MOTOR_BB = 8; // MOTOR_BB connected to digital pin 8

//Line Sensor
const int RIGHT = A0; // RIGHT sensor connected to analog pin A0
const int MIDL = A1;  // MIDL  sensor connected to analog pin A1
const int LEFT = A2;  // LEFT  sensor connected to analog pin A2
short RL = 0 ;        //remember the direction of the sensor (+1 Right, -1 left, 0 nothing) 
bool stable = 0 ;     //whether the MIDL sensor is on the line or not

//Ultra-Sonic Sensor
const int TRIG =  A3; // TRIG PIN connected to analog pin A3
constexpr int ECHO  = A4; // ECHO PIN connected to analog pin A4
#define MAX_DISTANCE 50 // Define Maximum Distance
#define THRS  500        // Threshold
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);

//Servo Motors
//Futaba S3003 - Servo Standard  -  60°  -  500-3000 μs


//Debug
int o_delay = 0;



void setup() {  
pinMode(ENAB, OUTPUT); // initialize ENAB pin as an output
pinMode(ENAB, OUTPUT); // initialize ENAB pin as an output
pinMode(MOTOR_AF, OUTPUT); // initialize MOTOR_AF pin as an output
pinMode(MOTOR_AB, OUTPUT); // initialize MOTOR_AB pin as an output
pinMode(MOTOR_BF, OUTPUT); // initialize MOTOR_BF pin as an output
pinMode(MOTOR_BB, OUTPUT); // initialize MOTOR_BB pin as an output
pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
pinMode(LED_BUILTIN, OUTPUT); // initialize LED pin as an output

Serial.begin(9600);
delay(100);
Serial.write("test");
}


void loop() {

  int distance = sonar.ping_cm();
  /*if (distance == 0) {
    distance = 30;
  }
  if(distance <=15) {
    Stop();
    delay(100);
    turnRight();
    delay(350);
    moveForward();
    delay(500);
    turnLeft();
    delay(350);
    moveForward();
    delay(700);
    turnLeft();
    delay(300);
    moveForward();
    delay(400);
    turnRight();
    delay(400);
    continue;
  }
  */
  
  //Line Sensor Detection///////////////////

    if (!(analogRead(MIDL)<=THRS)){ //detecte if the robot is on the center
    stable = true;
    RL = 0;
  }
  else { 
    stable = false;
  }

  //detecte right and left sensors
  if (!(analogRead(RIGHT)<=THRS) && (analogRead(LEFT)<=THRS)){
    RL = +1;
  }
  else if ((analogRead(RIGHT)<=THRS) && !(analogRead(LEFT)<=THRS)){
    RL = -1;
  }


  ///Move Action//////////////////////////
  digitalWrite(LED_BUILTIN,HIGH);
  if ((analogRead(RIGHT)>THRS) && (analogRead(LEFT)>THRS)) {
 
    Stop();
    if (o_delay>=1000)
    Serial.println("Stop!");
  }else if (!stable && (RL==+1)) {
    turnRight();
    if (o_delay>=1000)
    Serial.println("Right!");

  }else if (!stable && (RL==-1)) {
    turnLeft();
    if (o_delay>=1000)
    Serial.println("Left!");
  
  }else if (stable || RL==0) {
    moveForward();
    if (o_delay>=1000)
    Serial.println("Forward!");
    digitalWrite(LED_BUILTIN,HIGH);
  }

  ///Other/////////////////////////////////
    delay(70);
  if (o_delay>=1000){
    Serial.print("Right: ");
    Serial.print(analogRead(RIGHT));
    Serial.print("  Middle: ");
    Serial.print(analogRead(MIDL));
    Serial.print("  Left: ");
    Serial.println(analogRead(LEFT));
    Serial.print("Distance :");
    Serial.println(distance);
    o_delay=0;
  }
  else o_delay+=70;
}// end loop

////////////////////////////////////////////////////////////////////

void moveForward(){
  analogWrite(ENAB, 200);
  digitalWrite(MOTOR_AF, HIGH);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, HIGH);
  digitalWrite(MOTOR_BB, LOW);
}

void Stop() {
  analogWrite(ENAB, 0);
  digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, LOW);
  digitalWrite(MOTOR_BB, LOW);
}

void turnRight() {
  analogWrite(ENAB, 200);
  digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, HIGH);
  digitalWrite(MOTOR_BB, LOW);
  
}

void turnLeft() {
  analogWrite(ENAB, 200);
  digitalWrite(MOTOR_AF, HIGH);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, LOW);
  digitalWrite(MOTOR_BB, LOW);
}



/*void turnAround() {  //BY Nagib
  
  
  
  digitalWrite(MOTOR_BF, HIGH);
  digitalWrite(MOTOR_BB, LOW);

  for (int fadeValue = 70; fadeValue <=200; fadeValue +=10) {
    analogWrite(ENA, fadeValue);
    digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, HIGH);
  }

   for (int fadeValue = 200; fadeValue >=70; fadeValue -=10) {
    analogWrite(ENB, fadeValue);
    digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, HIGH);
  }
}*/
