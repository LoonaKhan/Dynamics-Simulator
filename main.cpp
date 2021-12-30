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
#include <utility>
#include <vector>
#include <string>
#include <unistd.h>
#include <SFML//Graphics.hpp>
#include <cstdlib>
#include <ctime>
using namespace sf;
using namespace std;

class particle{
    public:
        vector<float> v, a, size, pos = {0.0,0.0};
        RectangleShape player;
        float right, left, top, bottom, m;

    explicit particle(vector<float> v={0,0}, vector<float> a={0,0}, float m=1.0f,
                      vector<float> pos={0,0}, const string& colour="Red", vector<float> dimentions={10.0, 10.0}){

        this->v =std::move(v);
        this->a=std::move(a);
        this->m=m;

        this->pos=pos;
        this->player.setPosition(pos[0], pos[1]);

        this->size = dimentions;
        this->player.setSize(Vector2f(dimentions[0], dimentions[1]));
        this->player.setOrigin(dimentions[0]/2, dimentions[1]/2);

        this->left = this->player.getOrigin().x-dimentions[0]/2;
        this->right = this->player.getOrigin().x+dimentions[0]/2;
        this->top = this->player.getOrigin().y-dimentions[1]/2;
        this->bottom = this->player.getOrigin().y-dimentions[1]/2;

        if (colour == "Red") {
            this->player.setFillColor(Color::Red);
        }
        else if(colour =="Blue") {
            this->player.setFillColor(Color::Blue);
        }
        else if(colour =="Green") {
            this->player.setFillColor(Color::Green);
        }
        else if(colour =="Yellow") {
            this->player.setFillColor(Color::Yellow);
        }
        else if(colour =="White") {
            this->player.setFillColor(Color::White);
        }
        else if(colour =="Cyan") {
            this->player.setFillColor(Color::Cyan);
        }
        else if(colour =="Magenta") {
            this->player.setFillColor(Color::Magenta);
        }

    }

        void simulate(){
            check_inputs();
            accelerate();
            move();
        };

        void move(){
            // place new elements at the end of the vector then removes the
            // first 2 indexes
            // this will be redundant since we wont need pos
            for (int i=0, iter=this->pos.size(); i<iter; i++){
                this->pos.insert(this->pos.end(), this->pos[i]+this->v[i]);
            }
            for (int i=0, iter=this->pos.size()/2; i<iter; i++){
                this->pos.erase(this->pos.begin());
            }

            this->player.move(this->v[0], this->v[1]);

            // refactor this for each side of the hitbox
            for (int i=0; i< this->pos.size();i++){
                if ((this->pos[i]<=0 and this->v[i]<0)or (this->pos[i]>=1024 and this->v[i]>0)){
                    /*
                     * Reduces velocity a tad upon hitting an edge.
                     *
                     * The reason we take off 10% of the momentum is because trying to give the edge a mass would be
                     * problematic. It does not move, but setting its Vfinal to 0 means no change in velocity.
                     *
                     * so giving it's final momentum an arbitrarily small value would make sense. however setting it to
                     * a fixed value would lead to inconsistencies in extremeties. setting it very low could mean a
                     * lower momentum from mass 1 would give an improper Vfinal for mass 1.
                     *
                     * Thus, simply reducing momentum by a percentage is preferable and always returns valid.
                     */
                    this->v[i] = -(((this->m*this->v[i])*0.9f)/this->m);

                }
            }
        }

        float collision(particle mass2){
            // make this a list of all particles collided, not just 2
            float Px_total = (this->m*this->v[0])+(mass2.m*mass2.v[0]);
            float Py_total = (this->m*this->v[1])+(mass2.m*mass2.v[1]);

            this->v = {};

            //return mass2 velocity
        }

        void accelerate(){
            for (int i=0, iter=v.size(); i<iter; i++){
                v.insert(v.end(), v[i]+a[i]);
                //cout <<v.back() << " Accel: {"<<a[0] <<", "<<a[1] << "}"<<endl;
            }
            for (int i=0, iter=v.size()/2; i<iter; i++){
                v.erase(v.begin()); 
            }
        }

        void check_inputs(){
            // runs every frame and checks inputs of the arrow keys 
            // if the key is pressed, add a certain amount to the acceleration
            // in that direction.
            // todo:
            // figure out how to detect keypresses
            if (Keyboard::isKeyPressed(Keyboard::Key::A)){
                a[0]-= 0.0000000001;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::D)){
                a[0]+= 0.0000000001;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::W)){
                a[1]-= 0.0000000001;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
                a[1] += 0.0000000001;
            }
        }

};

// global variables
vector<particle> particles;
float max_accel = 2.0;
float min_accel = 0.00000001;
int FPS = 60; //frames per second
float SPF = 1.0f/(float)FPS; // seconds per frame
bool Mouse_Clicked_Last_Frame = false;


float get_rand(float max=::max_accel){
    srand(time(0));
    float num = ((float)rand())/((float)RAND_MAX/max);
    int sign = rand();
    if (sign%2==0)
        return -num;
    return num;
}

void new_particle(const vector<float>& mouse_pos){
    /*
     * Creates new particles
     */
    string colours[] = {"Red", "Blue", "Green", "Yellow", "White", "Cyan", "Magenta"};
    float vx, vy, colour;
    vx = get_rand();
    vy = get_rand();
    colour = abs(get_rand(colours->size()));
    particle p({vx,vy}, {0.0, 0.0}, 1.0, mouse_pos, colours[int(min(((int)colour), (int)colours->size() - 1))]);
    particles.push_back(p);
    if (particles.size() >100)
        particles.erase(particles.begin());
}

int main(){
    RenderWindow window(VideoMode(1024,1024), "Window", Style::Close | Style::Resize); // creates the window
    //particle q({0,0}, {0,0}, 1.0, {0,0}, "Red"); //initializes the particle

    /*
     * the initial message
     */
    //cout << "Initial Velocity: [" << p.v[0]<<","<<p.v[1]<<"]" << endl;
    //cout << "Initial Acceleration: [" << p.a[0]<<","<<p.a[1]<<"]" << endl;
    //cout << "-----------------------------" <<endl;
    //sleep(3);

    while (window.isOpen()){
        Event evnt{};

        while(window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed or Keyboard::isKeyPressed(Keyboard::Key::Escape)){
                window.close();
                for (int p=0; p<particles.size(); p++){
                    //cout << particles[p].a[0]<<particles[p].a[1] <<endl;
                }
            }
        }

        //drag click to create more
        if (Mouse::isButtonPressed(Mouse::Right) and !Mouse_Clicked_Last_Frame){ // creates a new particle on left click
            ::Mouse_Clicked_Last_Frame = true;
            Vector2i mouse_pos = Mouse::getPosition(window);
            ::new_particle({(float)mouse_pos.x, (float)mouse_pos.y});
        }
        else if(!Mouse::isButtonPressed(Mouse::Right) and Mouse_Clicked_Last_Frame){ // if we dont click, set false
            Mouse_Clicked_Last_Frame = false;
        }

        for (int part = 0; part<particles.size(); part++){ // simulates all particles
            particles[part].simulate();
        }
        //q.simulate();
        //cout<<"Velocity: [" << p.v.front() <<","<< p.v.back() << "]" <<endl;
        //cout<<"Position: [" << p.pos.front() <<","<< p.pos.back() << "]" <<endl;
        //cout <<"Acceleration: ["<< p.a.front() <<","<<p.a.back()<<"]"<<endl;
        //cout<<"-----------------------------" <<endl;
        //sleep(1);

        window.clear();
        //window.draw(q.player);
        for (int part = 0; part<particles.size(); part++){
            window.draw(particles[part].player);
        }
        //window.draw(p.player); //draw to the back buffer
        window.display(); //move the back buffer to the front buffer


    }
    
    return 0;
}
