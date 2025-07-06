//
// Created by atila_g7 on 05/07/25.
//

#pragma once
#include "Actor.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"


class Enemy : public Actor
{
public:
    explicit Enemy(Game* game, ElementState s, float forwardSpeed = 100.0f, float deathTime = 0.5f);

    virtual void OnUpdate(float deltaTime) override;
    virtual void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    virtual void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    virtual void Damage(int d);
    virtual float Speed();
    virtual void Pursuit(float deltaTime, int scare_time);

    void Kill() override;
    bool FloorForward();
    ElementState GetElement() const { return mElement; }
protected:
    bool mIsDying;
    float mForwardSpeed;
    float mDyingTimer;
    float mScareTimer;
    ElementState mElement;
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};