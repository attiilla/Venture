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
    explicit Gerold(Game* game, float forwardSpeed = 100.0f, float deathTime = 0.5f);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    void Kill() override;

    bool FloorForward();

private:
    bool mIsDying;
    float mForwardSpeed;
    float mDyingTimer;
    float mScareTimer;
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};