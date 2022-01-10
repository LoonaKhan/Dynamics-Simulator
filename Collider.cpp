//
// Created by loona on 2022-01-02.
//

#include "Collider.h"

bool Collider::CheckCollision(Collider &other, float push) {
    sf::Vector2f otherPosition = other.GetPosition();
    sf::Vector2f otherHalfsize = other.GetHalfSize();
    sf::Vector2f thisPosition = GetPosition();
    sf::Vector2f thisHalfSize = GetHalfSize();

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = abs(deltaX)-(otherHalfsize.x + thisHalfSize.x);
    float intersectY = abs(deltaY) - (otherHalfsize.y + thisHalfSize.y);

    if (intersectX<=0.0f and intersectY<=0.0f){
        push  = std::min(std::max(push, 0.0f), 1.0f);

        if (abs(intersectX) < abs(intersectY)){
            if (deltaX > 0.0f){
                Move(intersectX * (1.0f - push), 0.0f);
                other.Move(-intersectX * push, 0.0f);
            }
            else{
                Move(-intersectX * (1.0f - push), 0.0f);
                other.Move(intersectX * push, 0.0f);
            }
        }
        else{
            if (deltaY > 0.0f){
                Move(0.0f, intersectY * (1.0f - push));
                other.Move(0.0f, -intersectY * push);
            }
            else{
                Move(0.0f, -intersectY * (1.0f - push));
                other.Move(0.0f,intersectY * push);
            }
        }

        return true;
    }

    return false;
}

Collider::Collider(sf::RectangleShape &body) :
    body(body)
{

}

Collider::~Collider() {}