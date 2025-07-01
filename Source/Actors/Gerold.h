//
// Created by atila-rex on 6/30/25.
//

#pragma once

#include "Actor.h"

enum class GeroldState
{
    Sleepy = 0,
    Wake = 1,
    Mad = 2,
};

class Gerold : public Actor
{
public:
    static const float SCARE_TIME;
    static const float STATE_DURATION;
    static const float JUMP_INTERVAL;
    explicit Gerold(Game* game, float forwardSpeed = 100.0f, float jumpSpeed = -550.0f, float deathTime = 0.5f);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void Jump();
    void Kill() override;

    bool FloorForward();
    void ChangeState(GeroldState newState);
    GeroldState DecideNextState(int i);
private:
    bool mIsDying;
    float mForwardSpeed;
    float mJumpSpeed;
    float mDyingTimer;
    float mScareTimer;
    float mStateTimer;
    float mJumpTimer;
    int mStateCounter;
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    GeroldState mSleepState;
};