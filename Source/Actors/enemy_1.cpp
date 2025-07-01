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
        : Actor(game)
        , mDyingTimer(deathTime)
        , mIsDying(false)
        , mForwardSpeed(forwardSpeed)
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

void enemy_1::Kill()
{
    mIsDying = true;
    mDrawComponent->SetAnimation("Dead");
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
    mGame->AddScore(500);
}

void enemy_1::BumpKill(const float bumpForce)
{
    mDrawComponent->SetAnimation("Idle");

    mRigidBodyComponent->SetVelocity(Vector2(bumpForce/2.0f, -bumpForce));
    mColliderComponent->SetEnabled(false);

    // Flip upside down (180 degrees)
    SetRotation(180);
    mGame->AddScore(500);
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

bool enemy_1::FloorForward() {
    float px = mPosition.x;
    float py = mPosition.y;
    float vx = mRigidBodyComponent->GetVelocity().x;
    Vector2 nextTile = Vector2(vx>0?px+Game::TILE_SIZE:px-Game::TILE_SIZE, py+Game::TILE_SIZE);
    int nextX = static_cast<int>(nextTile.x/32);
    int nextY = static_cast<int>(nextTile.y/32);
    std::vector<AABBColliderComponent*> nearbyColliders = mGame->GetNearbyColliders(mPosition, 1);
    for (auto collider : nearbyColliders) {
        if (collider->IsEnabled() ) {  //Ignora colisores desbilitados
            Vector2 collider_pos = collider->GetOwner()->GetPosition();
            int cX = static_cast<int>(collider_pos.x/32);
            int cY = static_cast<int>(collider_pos.y/32);
            if (cX==nextX && cY==nextY) {
                return true;
            }
        }
    }
    return false;
}

