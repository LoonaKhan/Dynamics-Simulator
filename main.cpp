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
        vector<float> v; //velocity is constant unless changed
        vector<float> a; //acceleration is constant unless changed
        float m; //mass is constant unless changed
        vector<float> pos = {0.0, 0.0};

        vector<float> move(){
            // place new elements at the end of the vector then removes the
            // first 2 indexes
            for (int i=0, iter=pos.size(); i<iter; i++){
                pos.insert(pos.end(), pos[i]+v[i]); 
            }
            for (int i=0, iter=pos.size()/2; i<iter; i++){
                pos.erase(pos.begin());
            }
            return pos;
        }

        vector<float> accelerate(){
            for (int i=0, iter=v.size(); i<iter; i++){
                v.insert(v.end(), v[i]+a[i]); 
            }
            for (int i=0, iter=v.size()/2; i<iter; i++){
                v.erase(v.begin()); 
            }
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
    p.v = {1.0,0}; p.a = {0,-0.5}; p.m = 1.0;
    
    cout << "Initial Velocity: [" << p.v[0]<<","<<p.v[1]<<"]" << endl;
    cout << "Initial Acceleration: [" << p.a[0]<<","<<p.a[1]<<"]" << endl;

    cout << "-----------------------------" <<endl;
    sleep(3);

    while (true){
        p.move();
        p.accelerate();
        cout<<"Velocity: [" << p.v.front() <<","<< p.v.back() << "]" <<endl;
        cout<<"Position: [" << p.pos.front() <<","<< p.pos.back() << "]" <<endl;
        cout<<"-----------------------------" <<endl;
        sleep(1);
    }
    
    return 0;
}
