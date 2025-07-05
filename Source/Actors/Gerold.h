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
    explicit Gerold(Game* game, float forwardSpeed = 150.0f, float jumpSpeed = -550.0f, float deathTime = 0.5f);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void Jump();
    void ChangeState(GeroldState newState);
    GeroldState DecideNextState(int i);
private:
    float mJumpSpeed;
    float mScareTimer;
    float mStateTimer;
    float mJumpTimer;
    int mStateCounter;
    GeroldState mSleepState;
};