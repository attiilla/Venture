//
// Created by Lucas N. Ferreira on 30/09/23.
//

#pragma once

#include "Enemy.h"

class enemy_1 : public Enemy
{
public:
    static const float SCARE_TIME;
    explicit enemy_1(Game* game, float forwardSpeed = 100.0f, float deathTime = 0.5f);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
private:
    float mScareTimer;
};