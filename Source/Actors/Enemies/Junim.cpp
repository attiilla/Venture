//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Junim.h"

#include "../MainChar.h"
#include "../../Game.h"
#include "../Block.h"
#include "../../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../../Components/DrawComponents/DrawPolygonComponent.h"
#include "../../Random.h"

const float Junim::SCARE_TIME = 3.0f;
const int Junim::JUNIM_LIVES = 1;

Junim::Junim(Game* game, ElementState s)
        : Enemy(game, s)
{
    mLives = JUNIM_LIVES;
    mScareTimer = SCARE_TIME;
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f);
    mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));

    mColliderComponent = new AABBColliderComponent(this, 0, 0,
                                                   Game::TILE_SIZE, Game::TILE_SIZE,
                                                   ColliderLayer::Enemy);

    mDrawComponent = new DrawAnimatedComponent(this,
                                                  "../Assets/Sprites/Enemies/Junim/Junim.png",
                                                  "../Assets/Sprites/Enemies/Junim/Junim.json");

    if (s == ElementState::Fire) {
        mDrawComponent->AddAnimation("Dead", {0});
        mDrawComponent->AddAnimation("Idle", {0});
        mDrawComponent->AddAnimation("Walk", {0, 1, 2});
    } else {
        mDrawComponent->AddAnimation("Dead", {3});
        mDrawComponent->AddAnimation("Idle", {3});
        mDrawComponent->AddAnimation("Walk", {3, 4, 5});
    }

    mDrawComponent->SetAnimation("Walk");
    mDrawComponent->SetAnimFPS(5.0f);
}

void Junim::OnUpdate(float deltaTime)
{
    // if (mIsDying)
    // {
    //     mDyingTimer -= deltaTime;
    //     if (mDyingTimer <= 0.0f) {
    //
    //     }
    // }
    if (mIsDying) {
        mState = ActorState::Destroy;
    }
    /*if (GetPosition().y > GetGame()->GetWindowHeight())
    {
        mState = ActorState::Destroy;
    }*/
    Vector2 velocity = mRigidBodyComponent->GetVelocity();
    // Evita buraco
    AvoidHole(deltaTime, velocity);

    // Persegue o jogador
    Pursuit(deltaTime, SCARE_TIME, velocity);
}

void Junim::Damage(int d) {
    if (const auto temp = mGame->GetAudio()->PlaySound("hurt.mp3", false); !temp.IsValid()) {
        SDL_Log("Failed to play background music: hurt.mp3");
    } else {
        SDL_Log("Playing musical effect: hurt.mp3");
    }
    mLives-=d;
    if (mLives <= 0) {
        Kill();
    }
}

void Junim::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
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

void Junim::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF) {
        other->GetOwner()->Kill();
    }
}

