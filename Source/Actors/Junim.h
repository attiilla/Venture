//
// Created by Lucas N. Ferreira on 30/09/23.
//

#pragma once

#include "Enemy.h"

class Junim : public Enemy
{
public:
    static const float SCARE_TIME;
    static const int JUNIM_LIVES;
    explicit Junim(Game* game, ElementState s, float forwardSpeed = 100.0f, float deathTime = 0.5f);
    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void Damage(int d) override;
};