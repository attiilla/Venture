//
// Created by atila_g7 on 06/07/25.
//

#pragma once

#include "Enemy.h"

enum class IgaState
{
    Calm = 0,
    Scared = 1,
};

class Iga : public Enemy
{
public:
    static const float SCARE_TIME;
    static const float CALM_SHOT_INTERVAL;
    static const float SCARED_SHOT_INTERVAL;
    static const int IGA_LIVES;
    explicit Iga(Game* game, ElementState s);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void Damage(int d) override;
    float Speed() override;

    int Direction();
    void Flip();
private:
    float mStateTimer;
    int mLives;
    int mDirection;
    IgaState mScareState;
};