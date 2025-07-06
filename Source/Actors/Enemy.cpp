//
// Created by atila_g7 on 05/07/25.
//

#include "Enemy.h"
#include "Actor.h"
#include "MainChar.h"
#include "../Game.h"
Enemy::Enemy(Game* game, ElementState s, float forwardSpeed, float deathTime)
    : Actor(game)
    , mElement(s)
    , mDyingTimer(deathTime)
    , mIsDying(false)
    , mForwardSpeed(forwardSpeed)
    , mLives(0)
    , mDrawComponent(nullptr)
    , mRigidBodyComponent(nullptr)
    , mColliderComponent(nullptr)
{
}

void Enemy::OnUpdate(float deltaTime){}
void Enemy::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other){}
void Enemy::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other){}
void Enemy::Damage(int d){ SDL_Log("ui"); }
void Enemy::Kill() {
    if (mRigidBodyComponent!=nullptr && mColliderComponent!=nullptr) {
        mIsDying = true;
        mDrawComponent->SetAnimation("Dead");
        mRigidBodyComponent->SetEnabled(false);
        mColliderComponent->SetEnabled(false);
    }
    //mGame->AddScore(500);
}

float Enemy::Speed() {
    return mForwardSpeed;
}

void Enemy::Pursuit(float deltaTime, int scare_time) {
    float vx = mRigidBodyComponent->GetVelocity().x;
    float vy = mRigidBodyComponent->GetVelocity().y;
    // Evita buraco
    if (!FloorForward()) {
        mScareTimer = -deltaTime;
        mRigidBodyComponent->SetVelocity(Vector2(-vx, vy));
    }

    if (mScareTimer<scare_time) { // true se o NPC está se afastando de um buraco
        mScareTimer += deltaTime;
    } else //faz o inimigo perseguir o jogador se ele não estiver se afastando de um buraco
    if(mGame->GetMainChar()->IsCharToLeft(mPosition) && vx >= 0.0f) { //personagem a esquerda enquanto inimigo se move para direita
        mRigidBodyComponent->SetVelocity(Vector2(-Speed(), vy));
    } else if (!mGame->GetMainChar()->IsCharToLeft(mPosition) && vx <= 0.0f) { //personagem a direita enquanto inimigo se move para esquerda
        mRigidBodyComponent->SetVelocity(Vector2(Speed(), vy));
    }
}

bool Enemy::FloorForward(){
   
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