//
// Created by atila_g7 on 06/07/25.
//
#include "Iga.h"
const float Iga::SCARE_TIME = 3.0f;
const int Iga::IGA_LIVES = 8;
Iga::Iga(Game* game, ElementState s)
    : Enemy(game, s, 0.0f)
    , mStateTimer(0.0f)
    , mDirection(-1)
{
    mLives = IGA_LIVES;
    mScareTimer = SCARE_TIME;
}

void Iga::OnUpdate(float deltaTime) {

}

void Iga::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) {

}

void Iga::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) {

}

void Iga::Damage(int d) {
    mLives-=d;
    if (mLives <= 0) {
            Kill();
    }
}


float Iga::Speed() {
    return 0;
}

int Iga::Direction() {
    return mDirection;
}

void Iga::Flip() {
    mDirection*=-1;
}