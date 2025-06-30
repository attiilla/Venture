//
// Created by Lucas N. Ferreira on 30/09/23.
//

#pragma once

#include "Actor.h"

class enemy_1 : public Actor
{
public:
    static const float SCARE_TIME;
    explicit enemy_1(Game* game, float forwardSpeed = 100.0f, float deathTime = 0.5f);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    void Kill() override;
    void BumpKill(const float bumpForce = 300.0f);

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