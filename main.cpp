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
#include "Collider.h"
using namespace sf;
using namespace std;

class Particle{
    public:
        static vector<Particle> particles;
        vector<float> v, a, size,rad, pos = {0.0, 0.0};
        RectangleShape player;
        float right, left, top, bottom, m;
        Collider GetCollider() {return Collider(player);}

    explicit Particle(vector<float> v={0, 0}, vector<float> a={0, 0}, float m=1.0f,
                      vector<float> pos={0,0}, const string& colour="Red", vector<float> dimentions={10.0, 10.0}){

        this->v =std::move(v);
        this->a=std::move(a);
        this->m=m;

        this->pos=pos;
        this->player.setPosition(pos[0], pos[1]);

        this->size = dimentions;
        this->rad = {this->size[0] / 2, this->size[1] / 2};
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
            collision();
            accelerate();
            move();
            //GetCollider().CheckCollision(player.Ge)
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

        void collision(){
            // make this a list of all particles collided, not just 2?
            /*
             * Explanation on the process and explaining deltas, rads and the collision formulas
             */
            for (int i=0; i<Particle::particles.size(); i++){
                Particle mass2 = Particle::particles[i];

                float deltax, deltay; // determine the deltas. the distance from the origins of each particle
                deltax=(abs(this->pos[0]-mass2.pos[0]));
                deltay=(abs(this->pos[1]-mass2.pos[1]));

                // Calculating if the 2 particles are colliding or not based on the rads and deltas
                if ((((deltax-(this->rad[0]+mass2.rad[0])) <=0.0f) and ((deltay-(this->rad[1]+mass2.rad[1]))) <=0.0f)and !(mass2.pos == this->pos)){
                    //cout<< "colliding" <<endl;
                    float tot_mass = this->m + mass2.m;
                    //for (int i=0; i<this->v.size(); i++) {
                        //this->v[i] = -this->v[i];
                        //mass2.v[i] = -mass2.v[i];
                        //this->v[i] = 0.0004;//((this->m - mass2.m)/(tot_mass)) * this->v[i];
                        //mass2.v[i] = 0.005;//((2.0f*this->m)/(tot_mass))*this->v[i];
                        if (mass2.m > this->m){
                            for(int i=0; i<this->v.size(); i++){
                                this->v[i] = -1.1f*this->v[i];
                                mass2.v[i] = -mass2.v[i];
                            }
                        }
                        else{
                            for(int i=0; i<this->v.size(); i++){
                                this->v[i] = -this->v[i];
                                mass2.v[i] = -1.1f*mass2.v[i];
                            }
                        }
                    //}

                    cout << mass2.v[0] << ", " << mass2.v[1] << "      " << this->v[0] << ", "<< this->v[1]<<endl;
                    this->move(); mass2.move();
                    return;

                }
               //this->GetCollider().CheckCollision(&mass2.GetCollider(), 0.0f);
            }
            //cout << " " << endl;

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

class Group{
    /*
     * Make this like Sprite Groups in pygame
     * u can cycle through all particles in a group for collision detection, add and delete them from the group
     * basically a container for particles
     */
public:
    vector<Particle> list;
    Group(){
        this->list = {};
    }

    void add(const Particle& p){
        this->list.push_back(p);
    }
};

// global variables
vector<Particle> Particle::particles = {};
float max_accel = 0.4;
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

float get_pos_rand(float max, float min=0){
    srand(time(0));
    float num = min + ((float)rand())/((float)RAND_MAX/max);
    cout << num <<endl;
    return num;
}

void new_particle(const vector<float>& mouse_pos){
    /*
     * Creates new particles
     */
    string colours[] = {"Red", "Blue", "Green", "Yellow", "White", "Cyan", "Magenta"};
    float vx, vy, mass, colour;
    vx = get_rand();
    vy = get_rand();
    mass = get_pos_rand(6.0f);
    colour = abs(get_rand(colours->size()));

    Particle p({vx, vy}, {0.0, 0.0}, mass, mouse_pos, colours[int(min(((int)colour), (int)colours->size() - 1))], {40.0, 40.0});
    Particle::particles.push_back(p);
    if (Particle::particles.size() >100)
        Particle::particles.erase(Particle::particles.begin());
}

int main(){
    RenderWindow window(VideoMode(1024,1024), "Window", Style::Close | Style::Resize); // creates the window
    //Particle q({0,0}, {0,0}, 1.0, {0,0}, "Red"); //initializes the Particle

    while (window.isOpen()){
        Event evnt{};

        while(window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed or Keyboard::isKeyPressed(Keyboard::Key::Escape)){
                window.close();
                for (int i=0; i<Particle::particles.size(); i++){
                    cout << Particle::particles[i].pos[0] << ", " << Particle::particles[i].pos[1] <<endl;
                }
            }
        }

        //drag click to create more
        if (Mouse::isButtonPressed(Mouse::Right) and !Mouse_Clicked_Last_Frame){ // creates a new Particle on click
            ::Mouse_Clicked_Last_Frame = true;
            Vector2i mouse_pos = Mouse::getPosition(window);
            ::new_particle({(float)mouse_pos.x, (float)mouse_pos.y});
        }
        else if(!Mouse::isButtonPressed(Mouse::Right) and Mouse_Clicked_Last_Frame){ // if we dont click, set false
            Mouse_Clicked_Last_Frame = false;
        }

        for (int part = 0; part<Particle::particles.size(); part++){ // simulates all particles
            Particle::particles[part].simulate();
        }

        window.clear();
        //window.draw(q.player);
        for (int part = 0; part<Particle::particles.size(); part++){
            window.draw(Particle::particles[part].player);
        }
        window.display(); //move the back buffer to the front buffer


    }
    
    return 0;
}
