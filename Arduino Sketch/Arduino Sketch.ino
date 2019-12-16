//Arduino Line Follower Obstacle Panic Robot
//Created By DIY Builder & Project Team

#include"NewPing.h"
#include"style.hpp"
#include"main.h"
#include <Servo.h>

//Motion Motors
constexpr int ENAB = 3;  //ENAB connected to digital pin 3
constexpr int MOTOR_AF = 2;   // MOTOR_AF connected to digital pin 2
constexpr int MOTOR_AB = 4;   // MOTOR_AB connected to digital pin 4
constexpr int MOTOR_BF = 7;   // MOTOR_BF connected to digital pin 7
constexpr int MOTOR_BB = 8;   // MOTOR_BB connected to digital pin 8

const  byte Speed::MAX_SPEED = 185; 
const  byte Speed::MIN_SPEED = 90;

Speed      speed    = 185; // the speed of the motor. range:[0,255]

//Line Sensor
constexpr int RIGHT = A0; // RIGHT sensor connected to analog pin A0
constexpr int MIDL = A1;  // MIDL  sensor connected to analog pin A1
const int LEFT = A2;  // LEFT  sensor connected to analog pin A2
#define THRS  500        // Line Sensor Threshold
short RL = 0 ;        //remember the direction of the sensor (+1 Right, -1 left, 0 nothing) 
bool stable = 0 ;     //whether the MIDL sensor is on the line or not

//Ultra-Sonic Sensor
constexpr int TRIG =  A3; // TRIG PIN connected to analog pin A3
constexpr int ECHO  = A4; // ECHO PIN connected to analog pin A4
#define MAX_CM_DISTANCE 50 // Define Maximum Distance
NewPing sonar(TRIG, ECHO, MAX_CM_DISTANCE);

//Servo Motors
constexpr int pin_servo1 =  9;
constexpr int pin_servo2 = 10;
constexpr int pin_servo3 = 11;
constexpr int pin_servo4 = 11;

//Futaba S3003 - Servo Standard  -  60°  -  500-3000 μs
Servo servo_base;   //the servo that controlls the base
Servo servo_arm;    //the servo that controlls the middle degree of freedom
Servo servo_grp;    //the servo that controlls the gripper
Servo servo_4;      //the servo that controlls the forth degree of free may not be used ` 

//Debug
int o_delay = 0;

void setup() 
{  

pinMode(ENAB, OUTPUT); // initialize ENAB pin as an output
pinMode(ENAB, OUTPUT); // initialize ENAB pin as an output
pinMode(MOTOR_AF, OUTPUT); // initialize MOTOR_AF pin as an output
pinMode(MOTOR_AB, OUTPUT); // initialize MOTOR_AB pin as an output
pinMode(MOTOR_BF, OUTPUT); // initialize MOTOR_BF pin as an output
pinMode(MOTOR_BB, OUTPUT); // initialize MOTOR_BB pin as an output
pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
pinMode(LED_BUILTIN, OUTPUT); // initialize LED pin as an output

servo_base.attach(pin_servo1);  
servo_arm.attach(pin_servo2);   
servo_grp.attach(pin_servo3);  
//servo_4.attach(pin_servo4);   

Serial.begin(9600);
Println("setting up!");

servo_base.write(40);
servo_arm.write(90);
servo_grp.write(5);
//servo_4.write(90);
delay(250);
}


void loop() 
{
  
  //servo_interactive_test();
  //ultrasonic_test();
  //Gripper_test();

  /*Loop{
    speed.pulse_do(turnRight);
    int distance;
  {
    delay(5);
    int distance1 = (sonar.ping_cm()); if (distance1 == 0) distance1 = 52;
    delay(5);
    int distance2 = (sonar.ping_cm()); if (distance2 == 0) distance2 = 52;
    delay(5);
    int distance3 = (sonar.ping_cm()); if (distance3 == 0) distance3 = 52;
    distance  = (distance1 + distance2 + distance3)/3;
    if (distance == 0) then distance = 52;
  }

    
  }*/

  int distance;
{
  delay(5);
  int distance1 = (sonar.ping_cm()); if (distance1 == 0) distance1 = 52;
  delay(5);
  int distance2 = (sonar.ping_cm()); if (distance2 == 0) distance2 = 52;
  delay(5);
  int distance3 = (sonar.ping_cm()); if (distance3 == 0) distance3 = 52;
  distance  = (distance1 + distance2 + distance3)/3;
  if (distance == 0) then distance = 52;
}

  if (distance <= 13){

    Stop();

    while (distance < 12) {
      Println("??!");
      speed = 95;
      moveBackward();
      delay(5);
    {
      int distance1 = (sonar.ping_cm()); if (distance1 == 0) distance1 = 52;
      delay(5);
      int distance2 = (sonar.ping_cm()); if (distance2 == 0) distance1 = 52;
      delay(5);
      int distance3 = (sonar.ping_cm()); if (distance3 == 0) distance1 = 52;
      distance  = (distance1 + distance2 + distance3)/3;
      if (distance == 0) then distance = 52;
    }
      
    constexpr short delay_time = 5;
    if (o_delay>=1000){
      Serial.print("Distance :");
      Serial.println(distance);
      Serial.print("Speed :");
      Serial.println(speed);
      o_delay=0;
    }
    else o_delay+=delay_time;

    }                                                 
    Stop();
    if (distance > 13 ||  distance < 12)// check if the object is in position
    {
      goto skip;      //if not skip to debug and begin new 'void loop()'
    }

    //grip the object
    Println("gripping");
    servo_softmove(servo_base, 110);
    Println("gripping");
    servo_softmove(servo_grp , 155);
    Println("gripping");
    servo_softmove(servo_base,  40);

    Println("turning out");
    speed = 170;
    turnAround( left, 200, 80)  ;
    
    //release the object
    Println("releasing");
    servo_softmove(servo_base, 110, 30);
    Println("releasing");
    servo_softmove(servo_grp ,   2, 39);
    Println("releasing");
    servo_softmove(servo_base,  40); 

    Println("turning back");
    speed = 170;
    turnAround( right, 200, 80) ;
    
    goto skip; //will not perform any other action and skip to the debug output
  
  }
  else if (distance <= 25){
    //will slow down
    speed = (distance-12)*5+95;
  }
  else{
    speed = 165;
  }
  
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

skip:
  ///Other/////////////////////////////////
  constexpr short delay_time = 33;
  delay(delay_time);
  if (o_delay>=1000){
    Serial.print("Right: ");
    Serial.print(analogRead(RIGHT));
    Serial.print("  Middle: ");
    Serial.print(analogRead(MIDL));
    Serial.print("  Left: ");
    Serial.println(analogRead(LEFT));
    Serial.print("Distance :");
    Serial.println(distance);
    Serial.print("Speed :");
    Serial.println(speed);
    o_delay=0;
  }
  else o_delay+=delay_time;
}// end loop

////////////////////////////////////////////////////////////////////

void moveForward(){
  analogWrite(ENAB, speed);
  digitalWrite(MOTOR_AF, HIGH);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, HIGH);
  digitalWrite(MOTOR_BB, LOW);
}

void moveBackward(){
  analogWrite(ENAB, speed);
  digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, HIGH);
  digitalWrite(MOTOR_BF, LOW);
  digitalWrite(MOTOR_BB, HIGH);
}

void Stop() {
  analogWrite(ENAB, 0);
  digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, LOW);
  digitalWrite(MOTOR_BB, LOW);
}

void turnRight() {
  analogWrite(ENAB, speed);
  digitalWrite(MOTOR_AF, LOW);
  digitalWrite(MOTOR_AB, HIGH);
  digitalWrite(MOTOR_BF, HIGH);
  digitalWrite(MOTOR_BB, LOW);
  
}

void turnLeft() {
  analogWrite(ENAB, speed);
  digitalWrite(MOTOR_AF, HIGH);
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BF, LOW);
  digitalWrite(MOTOR_BB, HIGH);
}


void turnAround(direction dir, int time_cs, short pw) 
{  //BY Nagib
  constexpr float LR_factor = (90.0f)/(75);
  int&& time_ms = 10*time_cs;
  switch(dir)
  {
  case left:
    for (; time_ms>pw; time_ms -=pw) {
      speed.pulse_do(turnLeft, pw);
    }
    speed.pulse_do(turnLeft, time_ms);
    break;

  case right:
    //time_ms *= LR_factor;
    for (; time_ms>pw; time_ms -=pw) {
      speed.pulse_do(turnRight, pw);
    }
    speed.pulse_do(turnRight, time_ms);
    break;

    default:break;
  }
}

void servo_softmove (Servo& myservo, int next, int slowy)
{
  let prev = myservo.read();
  let sign = (next-prev)/abs(next-prev);
  for (var i = prev; (next-i)*sign > 0; i+=sign*10){
    myservo.write(i);
    delay(slowy);
  }
  myservo.write(next);
  delay(slowy);
}


///test///////////////////////////////////////////////////////////////

inline void servo_interactive_test()
{
  while (forever) 
  {
    String x; int serv_n,pos;
    Serial.println("enter servo number: ");
    delay(1);
    Loop if (Serial.available()){
      x = Serial.readString();
      Println(x);
      serv_n = x.toInt();
      Print("servo number: ")
      Println(serv_n);
      break;
    }
    
    Serial.println("enter position:");
    delay(1);
    Loop if (Serial.available()){
      delay(1);
      x = Serial.readString();
      Println(x);
      pos = x.toInt();
      Print("pos: "); Println(pos);
      break;
    }

    switch (serv_n)
    {
    case 0 : return;

    case 1 :
      servo_base.write(pos);
    break;

    case 2 :
      servo_arm.write(pos);
    break;

    case 3 :
      servo_grp.write(pos);
    break;
      
    case 4 :
      servo_4.write(pos);
    break;
    }
    
    delay(500);
  }
}/**/


inline void ultrasonic_test()
{
  while(forever)
  {
    Stop();
    int distance = (sonar.ping_cm()+sonar.ping_cm()+sonar.ping_cm())/3;
    if (distance == 0)
      distance = 52;

    constexpr short delay_time = 33;
    delay(delay_time);

    if (o_delay==250){
      Serial.print("Distance :");
      Serial.println(distance);
      o_delay=0;
    }
    else o_delay+=10;
  }
}/**/

inline void Gripper_test()
{
  Loop{
    int distance = (sonar.ping_cm()+sonar.ping_cm()+sonar.ping_cm())/3;
    if (distance == 0)
      distance = 52;

    if (distance >= 12 && distance <= 13){
      //grip the object
      Println("gripping");
      servo_softmove(servo_base, 110);
      servo_softmove(servo_grp , 155);
      servo_softmove(servo_base,  40);


      delay(2000);

      //return back
      Println("returningB");
      servo_softmove(servo_base, 110, 30);
      servo_softmove(servo_grp ,   2, 30);
      servo_softmove(servo_base,  40);

      Print("distance"); Println(distance);
      Println(servo_base.read());
      delay(1000);
    }

    else{
    servo_softmove(servo_base, 40);
    servo_softmove(servo_arm, 90);
    servo_softmove(servo_grp, 5);
    Print("distance"); Println(distance);
    Println(servo_base.read());
    delay(1000);
    }
    
  }
}/**/