//
// Created by atila-rex on 6/30/25.
//

#pragma once

#include "Enemy.h"

enum class GeroldState
{
    Sleepy = 0,
    Wake = 1,
    Mad = 2,
};

class Gerold : public Enemy
{
public:
    static const float SCARE_TIME;
    static const float STATE_DURATION;
    static const float JUMP_INTERVAL;
    static const int GEROLD_LIVES;
    explicit Gerold(Game* game, ElementState s,float forwardSpeed = 150.0f, float jumpSpeed = -550.0f);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void AvoidHole(float deltaTime, Vector2 velocity) override;
    void Jump();
    void ChangeState(GeroldState newState);
    void Damage(int d) override;
    GeroldState DecideNextState(int i);
    float Speed() override;
private:
    float mJumpSpeed;
    float mStateTimer;
    float mJumpTimer;
    float mBaseSpeed;
    int mStateCounter;
    int mLives;
    GeroldState mSleepState;
};