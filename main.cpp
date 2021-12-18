/*
 * Kinematics Engine
 * Developer: lvlonEmperor
 * Creation Date: 2021-12-17
 * Description:
 * We have an object whose velocity will be reported on in real time
 * we are able to apply an initial speed
 * using kepresses, we can apply an acceleration
 * this acceleration will update the speed every frame(60 fps)
 * we can change the acceleration at any time using  another keypress
 * we can also stop simulation and it will give us the last speed recorded
 * start at 1D, then 2D, then 3D
 */
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
using namespace std;

class particle{
    public:
        float v; //velocity is constant unless changed
        float a; //acceleration is constant unless changed
        float m; //mass is constant unless changed
        float pos = 0.0;

        float move(){
            pos += v;
            return pos;
        }

        float accelerate(){
            v +=a;
            return v;
        }

        float check_inputs(){
            // runs every frame and checks inputs of the arrow keys 
            // if the key is pressed, add a certain amount to the acceleration
            // in that direction.
            // todo:
            // figure out how to detect keypresses
        }

};

int main(){

    particle p;
    p.v = 4.0; p.a = -0.5; p.m = 1.0;
    cout << "created object p(particle)" << endl;
    cout << "Initial Velocity: " << p.v << endl;
    cout << "Initial Acceleration: " << p.a <<endl;
    sleep(3);

    while (true){
        p.move();
        p.accelerate();
        cout << "Velocity: " << p.v <<endl;
        cout << "Position: " << p.pos <<endl;
        sleep(1);
    }
    
    return 0;
}
