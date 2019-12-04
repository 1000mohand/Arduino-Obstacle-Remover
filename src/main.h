void Stop();
void moveForward();
void turnLeft();
void turnRight();

//this class Speed used to limit the input voltage to Motor
class Speed{
    public :
    Speed(int speed):speed(speed){
        if (speed > MAX_SPEED) speed = MAX_SPEED;
    };
    operator int (){return speed;}
    private:
    byte speed;
    Speed& operator= (int newspeed){
        this->speed = newspeed;
        return *this;
    }
    const static byte MAX_SPEED;
};