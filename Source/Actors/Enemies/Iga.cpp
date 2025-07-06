//
// Created by atila_g7 on 06/07/25.
//
#include "Iga.h"
#include "../../Game.h"
#include "../MainChar.h"
#include "../Projectile.h"
const float Iga::SCARE_TIME = 3.0f;
const float Iga::SHOOT_TIME = 3.0f;
const float Iga::LARGE_COOLDOWN = 3.0f;
const float Iga::CALM_COOLDOWN = 1.0f;
const float Iga::SCARED_COOLDOWN = 0.5f;
const int Iga::IGA_LIVES = 8;
Iga::Iga(Game* game, ElementState s)
    : Enemy(game, s, 0.0f)
    , mShootTimer(SHOOT_TIME)
    , mDirection(-1)
    , mBarrel(0.0f)
    , mLargeCooldown(0.0f)
    , mScareState(IgaState::Calm)
{
    mLives = IGA_LIVES;
    mScareTimer = SCARE_TIME;
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f);
    mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));

    mColliderComponent = new AABBColliderComponent(this, 0, 0,
                                                   Game::TILE_SIZE, Game::TILE_SIZE,
                                                   ColliderLayer::Enemy);

    mDrawComponent = new DrawAnimatedComponent(this,
                                                  "../Assets/Sprites/Enemies/Iga/Iga.png",
                                                  "../Assets/Sprites/Enemies/Iga/Iga.json");

    if (s == ElementState::Fire) {
        mDrawComponent->AddAnimation("Dead", {0});
        mDrawComponent->AddAnimation("Idle", {0});
        mDrawComponent->AddAnimation("Walk", {0, 1, 2, 3});
    } else {
        mDrawComponent->AddAnimation("Dead", {4});
        mDrawComponent->AddAnimation("Idle", {4});
        mDrawComponent->AddAnimation("Walk", {4, 5, 6, 7});
    }

    mDrawComponent->SetAnimation("Walk");
    mDrawComponent->SetAnimFPS(5.0f);
}

void Iga::OnUpdate(float deltaTime) {
    if (mIsDying)
    {
        mDyingTimer -= deltaTime;
        if (mDyingTimer <= 0.0f) {
            mState = ActorState::Destroy;
        }
    }
    if (mLargeCooldown>0) {
        mLargeCooldown -= deltaTime;
    } else {
        mLargeCooldown = 0.0f;
    }
    if ( //Vira na direção correta
        (mGame->GetMainChar()->IsCharToLeft(mPosition) && mDirection>0) ||
        (!mGame->GetMainChar()->IsCharToLeft(mPosition) && mDirection<0)
        )
    {
        Flip();
    }
    // Verifica se o MainChar está próximo do alcance e prepara para atirar se ele estiver
    float MainCharHeight = mGame->GetMainChar()->GetPosition().y;
    if (abs(MainCharHeight-mPosition.y)<48){
        mShootTimer = 0;
    }


    if (mLargeCooldown<=0) {
        if(mShootTimer<SHOOT_TIME) {
            mBarrel += deltaTime;
            if (mBarrel>Cooldown()) {
                mBarrel -= Cooldown();
                Shoot();
            }
            mShootTimer+=deltaTime;
        } else {
            mShootTimer=SHOOT_TIME;
            mLargeCooldown=LARGE_COOLDOWN;
        }
    }
}

void Iga::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) {
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
        Kill();
    }
}

void Iga::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) {
    if (other->GetLayer()==ColliderLayer::PlayerW || other->GetLayer()==ColliderLayer::PlayerF) {
        Kill();
    }
}

void Iga::Damage(int d) {
    mLives-=d;
    if (mLives <= 0) {
            Kill();
    }
}


float Iga::Speed() {
    return 0;
}

int Iga::Direction() {
    return mDirection;
}

void Iga::Flip() {
    mDirection*=-1;
}

float Iga::Cooldown() {
    switch (mScareState) {
        case IgaState::Calm:
            return CALM_COOLDOWN;
        case IgaState::Scared:
            return SCARED_COOLDOWN;
        default:
            return 0.0f;
    }
}

void Iga::Shoot() {
    //calcula o tile na frente da Iga
    Vector2 pos = mPosition;
    Vector2 frontTile = Vector2(mDirection>0?pos.x+Game::TILE_SIZE:pos.x-Game::TILE_SIZE,
                                pos.y);
    Projectile::ProjectileType ProjectType = (mElement==ElementState::Fire)? Projectile::ProjectileType::Fire : Projectile::ProjectileType::Water;
    new Projectile(mGame,ProjectType,frontTile,mDirection,4.0f, false);
    const auto soundName = (ProjectType == Projectile::ProjectileType::Fire) ? "Fire.wav" : "Water.wav";
    if (const auto temp = mGame->GetAudio()->PlaySound(soundName, false); !temp.IsValid()) {
        SDL_Log("Failed to play shoot sound");
    }
}