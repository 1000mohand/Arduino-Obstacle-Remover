#ifndef MAIN_H
#define MAIN_H

#include <Servo.h>
#include <Arduino.h>

//Debug
extern int o_delay;
#define Print(msg) Serial.print(msg);
#define Println(msg) Serial.println(msg);

enum direction : int {left, right, forward, backward};

void Stop();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void turnAround(direction dir, int time = 100);
void servo_softmove (Servo& myservo, int next, int slowy = 100);

inline void servo_interactive_test();
inline void ultrasonic_test();
inline void Gripper_test();

//this class Speed used to limit the input voltage to Motor
class Speed{
    public :
    Speed(int speed):speed(speed){
        if (speed > MAX_SPEED ) {
            this->speed = MAX_SPEED;
            Print("speed changed to: "); Println(this->speed);
        }
        if (speed < MIN_SPEED ) {
            this->speed = MIN_SPEED;
            Print("speed changed to: "); Println(this->speed);
        }
    };
    operator int (){return speed;}
    Speed& operator= (int newspeed){
        if (newspeed > MAX_SPEED){
            this->speed = MAX_SPEED;
            Print("speed changed to: "); Println(this->speed);
        }
        else if (newspeed < MIN_SPEED ) {
            this->speed = MIN_SPEED;
            Print("speed changed to: "); Println(this->speed);
        }
        else this->speed = newspeed;

        return *this;
    }
    private:
    byte speed;
    const static byte MAX_SPEED;
    const static byte MIN_SPEED;
};

#endif //MAIN_H