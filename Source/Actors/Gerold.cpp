//
// Created by atila-rex on 6/30/25.
//
#include "Gerold.h"

#include "MainChar.h"
#include "../Game.h"
#include "Block.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Random.h"

const float Gerold::SCARE_TIME = 3.0f;
Gerold::Gerold(Game* game, float forwardSpeed, float deathTime)
        : Actor(game)
        , mDyingTimer(deathTime)
        , mIsDying(false)
        , mForwardSpeed(forwardSpeed)
        , mScareTimer(SCARE_TIME)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 0.1f);
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

void Gerold::Kill()
{
    mIsDying = true;
    mDrawComponent->SetAnimation("Dead");
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
    mGame->AddScore(500);
}

void Gerold::OnUpdate(float deltaTime)
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
    if (!FloorForward()) {
        mScareTimer = -deltaTime;
        mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));
    }
    float vx = mRigidBodyComponent->GetVelocity().x;
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


void Gerold::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
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

void Gerold::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF) {
        other->GetOwner()->Kill();
    }
}

bool Gerold::FloorForward() {
    float px = mPosition.x;
    float py = mPosition.y;
    float vx = mRigidBodyComponent->GetVelocity().x;
    Vector2 nextTile = Vector2(vx>0?px+1:px-1, py+1);
    std::vector<AABBColliderComponent*> nearbyColliders = mGame->GetNearbyColliders(mPosition, 1);
    for (auto collider : nearbyColliders) {
        Vector2 collider_pos = collider->GetOwner()->GetPosition();
        Vector2 temp = collider_pos - nextTile;
        if (temp.LengthSq()<32){
            return true;
        }
    }
    return false;
}
