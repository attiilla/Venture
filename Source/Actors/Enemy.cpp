//
// Created by atila_g7 on 05/07/25.
//

#include "Enemy.h"
#include "Actor.h"
#include "../Game.h"
Enemy::Enemy(Game* game, float forwardSpeed, float deathTime)
    : Actor(game)
    , mDyingTimer(deathTime)
    , mIsDying(false)
    , mForwardSpeed(forwardSpeed)
    , mDrawComponent(nullptr)
    , mRigidBodyComponent(nullptr)
    , mColliderComponent(nullptr)
{
}

void Enemy::OnUpdate(float deltaTime){}
void Enemy::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other){}
void Enemy::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other){}
void Enemy::Kill() {
    if (!mRigidBodyComponent && !mColliderComponent) {
        mIsDying = true;
        mDrawComponent->SetAnimation("Dead");
        mRigidBodyComponent->SetEnabled(false);
        mColliderComponent->SetEnabled(false);
    }
    //mGame->AddScore(500);
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