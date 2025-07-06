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
const float Gerold::STATE_DURATION = 2.0f;
const float Gerold::JUMP_INTERVAL = 6.0f;
const int Gerold::GEROLD_LIVES = 3;

Gerold::Gerold(Game* game, ElementState s, float forwardSpeed, float jumpSpeed, float deathTime)
        : Enemy(game, s)
        , mJumpSpeed(jumpSpeed)
        , mSleepState(GeroldState::Sleepy)
        , mStateTimer(0.0f)
        , mJumpTimer(JUMP_INTERVAL)
        , mStateCounter(0)
        , mBaseSpeed(forwardSpeed)
{
    mScareTimer = SCARE_TIME;
    mLives = GEROLD_LIVES;
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f);
    mRigidBodyComponent->SetVelocity(Vector2(-mBaseSpeed/2, 0.0f));

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

void Gerold::OnUpdate(float deltaTime) {
    if (mIsDying)
    {
        mDyingTimer -= deltaTime;
        if (mDyingTimer <= 0.0f) {
            mState = ActorState::Destroy;
        }
    }

    /*if (GetPosition().y > GetGame()->GetWindowHeight())
    {
        mState = ActorState::Destroy;
    }*/


    Vector2 velocity = mRigidBodyComponent->GetVelocity();


    // Persegue o jogador
    Pursuit(deltaTime,SCARE_TIME,velocity);

    // Evita buraco
    AvoidHole(deltaTime,velocity);

    //Controla a lógica da mudança de estado
    mStateTimer += deltaTime;
    if (mStateTimer >= STATE_DURATION) {
        mStateTimer = 0.0f;
        ChangeState(DecideNextState(mStateCounter++));
    }

    //Dá uns pulos de uma forma que parece aleatória
    mJumpTimer -= deltaTime;
    if (mJumpTimer <= 0.0f){
        mJumpTimer = JUMP_INTERVAL;
        if (mSleepState == GeroldState::Mad) {
            Jump();
        }
    }
}

void Gerold::AvoidHole(float deltaTime,Vector2 velocity) {
    if (mRigidBodyComponent!=nullptr) {
        if (!FloorForward() && mIsOnGround) {
            mRigidBodyComponent->SetVelocity(Vector2(0, velocity.y));
            //SetPosition(Vector2(static_cast<int>(mPosition.x/32),static_cast<int>(mPosition.y/32)));
        }
    }
}

void Gerold::Jump() {
    if (mIsOnGround) {
        mIsOnGround = false;
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed));
        if (const auto temp = mGame->GetAudio()->PlaySound("Jump.mp3", false); !temp.IsValid()) {
            SDL_Log("Failed to play background music: Jump.mp3");
        } else {
            SDL_Log("Playing musical effect: Jump.mp3");
        }
    }
}

void Gerold::Damage(int d) {
    mLives-=d;
    if (mLives <= 0) {
        Kill();
    }
}

void Gerold::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer() == ColliderLayer::Blocks || other->GetLayer() == ColliderLayer::Enemy)
    {
        if (minOverlap > 0) {
            mRigidBodyComponent->SetVelocity(Vector2(-mBaseSpeed, mRigidBodyComponent->GetVelocity().y));
        }
        else {
            mRigidBodyComponent->SetVelocity(Vector2(mBaseSpeed, mRigidBodyComponent->GetVelocity().y));
        }
    } else if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF){
        other->GetOwner()->Kill();
    } else if (other->GetLayer()==ColliderLayer::Projectile) {
        SDL_Log("Projectile type %d hit", other->GetLayer());
        Kill();
        other->GetOwner()->Kill();
    }
}

void Gerold::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF) {
        other->GetOwner()->Kill();
    }
}

void Gerold::ChangeState(GeroldState newState) {
    Vector2 v = mRigidBodyComponent->GetVelocity();
    float direction = v.x>0?1.0f:-1.0f;
    mSleepState = newState;
    mRigidBodyComponent->SetVelocity(Vector2(direction*Speed(), v.y));
    if (mSleepState==GeroldState::Mad) {
            Jump();
    }
}

GeroldState Gerold::DecideNextState(int i) {
    switch (i%8) {
        case 0:
            return GeroldState::Wake;
        case 1:
            return GeroldState::Mad;
        case 2:
            return GeroldState::Wake;
        case 3:
            return GeroldState::Mad;
        case 4:
            return GeroldState::Sleepy;
        case 5:
            return GeroldState::Mad;
        case 6:
            return GeroldState::Mad;
        case 7:
            return GeroldState::Mad;
        default:
            return GeroldState::Sleepy;
    }
}

float Gerold::Speed() {
    switch (mSleepState){
        case GeroldState::Sleepy:
            return mBaseSpeed*0.5;
        case GeroldState::Wake:
            return mBaseSpeed;
        case GeroldState::Mad:
            return mBaseSpeed*2;
    }
}