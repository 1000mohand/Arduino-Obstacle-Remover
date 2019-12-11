#ifndef MAIN_H
#define MAIN_H

#include <Servo.h>

void Stop();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void turnAround(direction dir);
void servo_softmove (Servo& myservo, int next, int slowy = 100);

void servo_interactive_test();
void ultrasonic_test();
void Gripper_test()

//this class Speed used to limit the input voltage to Motor
class Speed{
    public :
    Speed(int speed):speed(speed){
        if (speed > MAX_SPEED) speed = MAX_SPEED;
    };
    operator int (){return speed;}
    Speed& operator= (int newspeed){
        this->speed = newspeed;
        return *this;
    }
    private:
    byte speed;
    const static byte MAX_SPEED;
};
enum direction : int {left, right, forward, backward};
#endif //MAIN_H