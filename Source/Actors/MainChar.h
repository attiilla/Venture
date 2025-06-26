//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

enum class ElementState
{
    Fire = 0,
    Water = 1
};

class MainChar : public Actor
{
public:
    explicit MainChar(Game* game, float forwardSpeed = 1500.0f, float jumpSpeed = -750.0f, ElementState element = ElementState::Water);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnHandleKeyPress(const int key, const bool isPressed) override;

    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    void Kill() override;
    void Win(AABBColliderComponent *poleCollider);

    void SetElement(ElementState element);
    void SwapElement();
    ElementState GetElement() { return mElement; }
private:
    static const int POLE_SLIDE_TIME = 1; // Time in seconds to slide down the pole
    static constexpr float DOUBLE_JUMP_COOLDOWN = 0.5f;

    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    float mPoleSlideTimer;
    float mDoubleJumpTimer;
    bool mIsRunning;
    bool mIsOnPole;
    bool mIsDying;
    ElementState mElement;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};