//
// Created by Brendo on 29/06/2025.
//

#include "Projectile.h"

#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Projectile::Projectile(Game *game, ProjectileType type, const Vector2 &position, float direction, const float lifetime)
    : Actor(game)
      , mType(type)
      , mLifeTime(lifetime) {
    SetPosition(position);

    mRigidBodyComponent = new RigidBodyComponent(this, 0.1f, 0.0f);
    mRigidBodyComponent->SetVelocity(Vector2(PROJECTILE_SPEED * direction, 0.0f));
    mRigidBodyComponent->SetApplyGravity(false);

    mColliderComponent = new AABBColliderComponent(this, 0, 0, 21, 9,
                                                   ColliderLayer::Projectile);

    if (mType == ProjectileType::Water) {
        mDrawComponent = new DrawAnimatedComponent(this,
                                                   "../Assets/Sprites/Projectiles/Water/Waterball.png",
                                                   "../Assets/Sprites/Projectiles/Water/Waterball.json");
    } else {
        mDrawComponent = new DrawAnimatedComponent(this,
                                                   "../Assets/Sprites/Projectiles/Fire/Fireball.png",
                                                   "../Assets/Sprites/Projectiles/Fire/Fireball.json");
    }

    mDrawComponent->AddAnimation("Standard", {0,1,2,3,4,5,6,7,8,9});
    mDrawComponent->SetAnimation("Standard");
    mDrawComponent->SetAnimFPS(9.0f);

    SDL_Log("Projectile type: %d", mType);
}

void Projectile::OnUpdate(float deltaTime) {
    mLifeTime -= deltaTime;
    if (mLifeTime <= 0.0f) {
        SetState(ActorState::Destroy);
    }
}

void Projectile::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) {
    if (other->GetLayer() == ColliderLayer::PlayerW || other->GetLayer() == ColliderLayer::PlayerF) {
        return;
    }

    if (other->GetLayer() == ColliderLayer::Enemy) {
        other->GetOwner()->Kill();
        SetState(ActorState::Destroy);
    } else if (other->GetLayer() == ColliderLayer::Blocks) {
        SetState(ActorState::Destroy);
    }
}

void Projectile::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) {
    if (other->GetLayer() == ColliderLayer::PlayerW || other->GetLayer() == ColliderLayer::PlayerF) {
        return;
    }

    if (other->GetLayer() == ColliderLayer::Enemy) {
        other->GetOwner()->Kill();
        SetState(ActorState::Destroy);
    } else if (other->GetLayer() == ColliderLayer::Blocks) {
        SetState(ActorState::Destroy);
    }
}

void Projectile::Kill() {
    SetState(ActorState::Destroy);
}
