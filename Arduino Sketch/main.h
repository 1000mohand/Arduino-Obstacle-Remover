void Stop();
void moveForward();
void turnLeft();
void turnRight();

//this class Speed used to limit the input voltage to Motor 
//that is, each time the speed is greater than  the maximum speed, the actual speed is reduced to maximum speed
class Speed{
public :
    Speed(int speed):speed(speed){
        if (speed > MAX_SPEED) speed = MAX_SPEED;
    };
    operator int (){return speed;}
    Speed& operator= (int newspeed){
        this->speed = newspeed;
	if (speed > MAX_SPEED) speed = MAX_SPEED;
        return *this;
    }
private:
    byte speed;
    const static byte MAX_SPEED;
};
