//
// Created by Brendo on 29/06/2025.
//

#include "Projectile.h"

#include "Enemies/Enemy.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Projectile::Projectile(Game *game, ProjectileType type, const Vector2 &position, float direction, const float lifetime, bool fromPlayer)
    : Actor(game)
      , mType(type)
      , mLifeTime(lifetime) {
    SetPosition(position);

    mRigidBodyComponent = new RigidBodyComponent(this, 0.1f, 0.0f);
    mRigidBodyComponent->SetVelocity(Vector2(PROJECTILE_SPEED * direction, 0.0f));
    mRigidBodyComponent->SetApplyGravity(false);

    if (direction < 0.0f)
    {
        SetRotation(Math::Pi);
    }
    else
    {
        SetRotation(0.0f);
    }
    auto layer = fromPlayer?ColliderLayer::Projectile:ColliderLayer::Enemy_Projectile;
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 21, 9,
                                                   layer);

    if (fromPlayer) {
        if (mType == ProjectileType::Water) {
            mDrawComponent = new DrawAnimatedComponent(this,
                                                    "../Assets/Sprites/Projectiles/Water/Waterball.png",
                                                    "../Assets/Sprites/Projectiles/Water/Waterball.json");
        } else {
            mDrawComponent = new DrawAnimatedComponent(this,
                                                    "../Assets/Sprites/Projectiles/Fire/Fireball.png",
                                                    "../Assets/Sprites/Projectiles/Fire/Fireball.json");
        }
    } else {
        if (mType == ProjectileType::Water) {
            mDrawComponent = new DrawAnimatedComponent(this,
                                                   "../Assets/Sprites/Projectiles/EnemyProjectiles/EnemyProjWater.png",
                                                   "../Assets/Sprites/Projectiles/EnemyProjectiles/EnemyProjWater.json");
        } else {
            mDrawComponent = new DrawAnimatedComponent(this,
                                                   "../Assets/Sprites/Projectiles/EnemyProjectiles/EnemyProjFire.png",
                                                   "../Assets/Sprites/Projectiles/EnemyProjectiles/EnemyProjFire.json");
        }
    }


    mDrawComponent->AddAnimation("Standard", {0,1,2,3,4,5,6,7,8});
    mDrawComponent->SetAnimation("Standard");
    mDrawComponent->SetAnimFPS(9.0f);
}

void Projectile::OnUpdate(float deltaTime) {
    mLifeTime -= deltaTime;
    if (mLifeTime <= 0.0f) {
        SetState(ActorState::Destroy);
    }
}

void Projectile::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) {
    if (other->GetLayer() == ColliderLayer::PlayerW || other->GetLayer() == ColliderLayer::PlayerF) {
        other->GetOwner()->Kill();
    }

    if (other->GetLayer() == ColliderLayer::Enemy) {
        SetState(ActorState::Destroy);
        auto e = dynamic_cast<Enemy*>(other->GetOwner());
        if (e!=nullptr){
            if (static_cast<int>(e->GetElement())==static_cast<int>(mType)){
                e->Damage(1);
            } else {
                e->Damage(2);
            }
        }
    } else if (other->GetLayer() == ColliderLayer::Blocks) {
        SetState(ActorState::Destroy);
    } else if (other->GetLayer() == ColliderLayer::Projectile) {
        SetState(ActorState::Destroy);
        other->GetOwner()->Kill();
    }
}

void Projectile::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) {
    if (other->GetLayer() == ColliderLayer::PlayerW || other->GetLayer() == ColliderLayer::PlayerF) {
        other->GetOwner()->Kill();
    }

    if (other->GetLayer() == ColliderLayer::Enemy) {
        SetState(ActorState::Destroy);
        auto e = dynamic_cast<Enemy*>(other->GetOwner());
        if (e!=nullptr){
            if (static_cast<int>(e->GetElement())==static_cast<int>(mType)){
                e->Damage(1);
            } else {
                e->Damage(2);
            }
        }
    } else if (other->GetLayer() == ColliderLayer::Blocks) {
        SetState(ActorState::Destroy);
    }
}

void Projectile::Kill() {
    SetState(ActorState::Destroy);
}
