//
// Created by Brendo on 29/06/2025.
//

#pragma once
#include "Actor.h"

class Projectile final : public Actor {
public:
    enum class ProjectileType {
        Water,
        Fire
    };

    Projectile(Game* game, ProjectileType type, const Vector2& position, float direction, float lifetime);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(float minOverlap, AABBColliderComponent* other) override;
    void Kill() override;

private:
    ProjectileType mType;
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    float mLifeTime;
    const float PROJECTILE_SPEED = 300.0f;
};