//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include "../Component.h"
#include "../../Math.h"
#include "../RigidBodyComponent.h"
#include <vector>
#include <map>
#include <set>

enum class ColliderLayer
{
    PlayerF = 0,
    PlayerW = 1,
    Enemy = 2,
    Blocks = 3,
    Permeable = 4,
    Coin = 5,
    Chest = 6,
    Rope = 7,
    Projectile = 8,
    Checkpoint = 9,
    Enemy_Projectile = 10,
    Flame = 11,
    Ice = 12,
    Lava = 13,
    Water = 14,
};

class AABBColliderComponent : public Component
{
public:
    // Collider ignore map
    const std::map<ColliderLayer, const std::set<ColliderLayer>> ColliderIgnoreMap = {
        {ColliderLayer::PlayerF, {ColliderLayer::Projectile}},
        {ColliderLayer::PlayerW, {ColliderLayer::Permeable, ColliderLayer::Projectile}},
        {ColliderLayer::Enemy,  {ColliderLayer::Coin}},
        {ColliderLayer::Blocks, {ColliderLayer::Blocks}},
        {ColliderLayer::Permeable, {}},
        {ColliderLayer::Coin, {ColliderLayer::Enemy}},
        {ColliderLayer::Chest, {}},
        {ColliderLayer::Rope, {}},
        {ColliderLayer::Projectile, {ColliderLayer::PlayerW, ColliderLayer::PlayerF}},
        {ColliderLayer::Enemy_Projectile, {ColliderLayer::Enemy}},
    };

    AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h,
                                ColliderLayer layer, bool isStatic = false, int updateOrder = 10);
    ~AABBColliderComponent() override;

    bool Intersect(const AABBColliderComponent& b) const;

    float DetectHorizontalCollision(RigidBodyComponent *rigidBody);
    float DetectVerticalCollision(RigidBodyComponent *rigidBody);

    void SetStatic(bool isStatic) { mIsStatic = isStatic; }

    Vector2 GetMin() const;
    Vector2 GetMax() const;
    Vector2 GetCenter() const;
    void SetLayer(ColliderLayer layer) { mLayer = layer; }
    ColliderLayer GetLayer() const { return mLayer; }

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    bool Ignore(AABBColliderComponent* collider);
private:
    float GetMinVerticalOverlap(AABBColliderComponent* b) const;
    float GetMinHorizontalOverlap(AABBColliderComponent* b) const;

    void ResolveHorizontalCollisions(RigidBodyComponent *rigidBody, const float minOverlap);
    void ResolveVerticalCollisions(RigidBodyComponent *rigidBody, const float minOverlap);

    Vector2 mOffset;
    int mWidth;
    int mHeight;
    bool mIsStatic;

    ColliderLayer mLayer;
};