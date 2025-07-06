//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "enemy_1.h"

#include "MainChar.h"
#include "../Game.h"
#include "Block.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Random.h"

const float enemy_1::SCARE_TIME = 3.0f;
enemy_1::enemy_1(Game* game, float forwardSpeed, float deathTime)
        : Enemy(game, ElementState::Neutral)
        , mScareTimer(SCARE_TIME)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f);
    mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));

    mColliderComponent = new AABBColliderComponent(this, 0, 0,
                                                   Game::TILE_SIZE, Game::TILE_SIZE,
                                                   ColliderLayer::Enemy);

    mDrawComponent = new DrawAnimatedComponent(this,
                                                  "../Assets/Sprites/Goomba/Goomba.png",
                                                  "../Assets/Sprites/Goomba/Goomba.json");

    mDrawComponent->AddAnimation("Dead", {0});
    mDrawComponent->AddAnimation("Idle", {1});
    mDrawComponent->AddAnimation("walk", {1, 2});
    mDrawComponent->SetAnimation("walk");
    mDrawComponent->SetAnimFPS(5.0f);
}

void enemy_1::OnUpdate(float deltaTime)
{
    if (mIsDying)
    {
        mDyingTimer -= deltaTime;
        if (mDyingTimer <= 0.0f) {
            mState = ActorState::Destroy;
        }
    }
    if (GetPosition().y > GetGame()->GetWindowHeight())
    {
        mState = ActorState::Destroy;
    }

    float vx = mRigidBodyComponent->GetVelocity().x;
    float vy = mRigidBodyComponent->GetVelocity().y;
    if (!FloorForward()) {
        mScareTimer = -deltaTime;
        mRigidBodyComponent->SetVelocity(Vector2(-vx, vy));
    }
    if(
        (mGame->GetMainChar()->IsCharToLeft(mPosition) && vx > 0.0f) || //char to left and enemy going to right
        (!mGame->GetMainChar()->IsCharToLeft(mPosition) && vx < 0.0f)   //char to right and enemy going to left
      )
    {
        if (mScareTimer<SCARE_TIME) { // true se o NPC está se afastando de um buraco
            mScareTimer += deltaTime;
        } else { //se o NPC não está se afastando de um buraco, ele vira na direção do MainChar
            mRigidBodyComponent->SetVelocity(Vector2(-vx, mRigidBodyComponent->GetVelocity().y));
        }
    }
}


void enemy_1::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if ((other->GetLayer() == ColliderLayer::Blocks || other->GetLayer() == ColliderLayer::Enemy))
    {
        if (minOverlap > 0) {
            mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));
        }
        else {
            mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, 0.0f));
        }
    }

    if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF){
        other->GetOwner()->Kill();
    }
}

void enemy_1::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF) {
        other->GetOwner()->Kill();
    }
}


