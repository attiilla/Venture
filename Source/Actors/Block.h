//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include <string>
#include "Actor.h"

class Block : public Actor
{
public:
    explicit Block(Game* game, const std::string &texturePath, const bool isStatic = true, const bool permeable = false);

    void SetPosition(const Vector2& position)
    {
        Actor::SetPosition(position);
        mOriginalPosition.Set(position.x, position.y);
    }

    void OnUpdate(float deltaTime) override;
    //void OnBump();
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    AABBColliderComponent* GetCollider() const { return  mColliderComponent;};

protected:
    const int BUMP_FORCE = 200;

    Vector2 mOriginalPosition;

    class AABBColliderComponent* mColliderComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    bool mPermeable;
};
