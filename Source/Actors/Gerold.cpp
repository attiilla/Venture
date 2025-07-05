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

Gerold::Gerold(Game* game, float forwardSpeed, float jumpSpeed, float deathTime)
        : Enemy(game)
        , mJumpSpeed(jumpSpeed)
        , mScareTimer(SCARE_TIME)
        , mSleepState(GeroldState::Sleepy)
        , mStateTimer(0.0f)
        , mJumpTimer(JUMP_INTERVAL)
        , mStateCounter(0)
{

    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f);
    mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed/2, 0.0f));

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


    float vx = mRigidBodyComponent->GetVelocity().x;
    float vy = mRigidBodyComponent->GetVelocity().y;
    // Evita buraco
    if (mIsOnGround && !FloorForward()) {
        mScareTimer = -deltaTime;
        mRigidBodyComponent->SetVelocity(Vector2(-vx, vy));
    }


    if(
        (mGame->GetMainChar()->IsCharToLeft(mPosition) && vx > 0.0f) || //personagem a esquerda enquanto inimigo se move para direita
        (!mGame->GetMainChar()->IsCharToLeft(mPosition) && vx < 0.0f)   //personagem a direita enquanto inimigo se move para esquerda
      )
    {
        if (mScareTimer<SCARE_TIME) { // true se o NPC está se afastando de um buraco
            mScareTimer += deltaTime;
        } else { //faz o inimigo perseguir o jogador se ele não estiver se afastando de um buraco
            mRigidBodyComponent->SetVelocity(Vector2(-vx, vy));
        }
    }

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

void Gerold::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if ((other->GetLayer() == ColliderLayer::Blocks || other->GetLayer() == ColliderLayer::Enemy))
    {
        if (minOverlap > 0) {
            mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, mRigidBodyComponent->GetVelocity().y));
        }
        else {
            mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, mRigidBodyComponent->GetVelocity().y));
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

void Gerold::ChangeState(GeroldState newState) {
    Vector2 v = mRigidBodyComponent->GetVelocity();
    float direction = v.x>0?1.0f:-1.0f;
    mSleepState = newState;
    switch (newState) {
        case GeroldState::Sleepy:{
            //SDL_Log("Gerold is sleeepyy...");
            mRigidBodyComponent->SetVelocity(Vector2(direction*mForwardSpeed/2, v.y));
            break;
        }
        case GeroldState::Wake: {
            //SDL_Log("Gerold is wake.");
            mRigidBodyComponent->SetVelocity(Vector2(direction*mForwardSpeed, v.y));
            break;
        }
        case GeroldState::Mad: {
            //SDL_Log("Gerold is MAD!!!");
            mRigidBodyComponent->SetVelocity(Vector2(1.5*direction*mForwardSpeed, v.y));
            Jump();
            break;
        }
    }
}

GeroldState Gerold::DecideNextState(int i) {
    switch (i%8) {
        case 0:
            return GeroldState::Sleepy;
        case 1:
            return GeroldState::Wake;
        case 2:
            return GeroldState::Sleepy;
        case 3:
            return GeroldState::Mad;
        case 4:
            return GeroldState::Wake;
        case 5:
            return GeroldState::Sleepy;
        case 6:
            return GeroldState::Wake;
        case 7:
            return GeroldState::Mad;
        default:
            return GeroldState::Wake;
    }
}
