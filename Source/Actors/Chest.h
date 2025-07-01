//
// Created by atila-rex on 6/30/25.
//

#pragma once
#include <string>
#include "Actor.h"
class Chest : public Actor
{
public:
    explicit Chest(Game* game, std::string texturePath, const bool isStatic = true);

    void SetPosition(const Vector2& position)
    {
        Actor::SetPosition(position);
    }

    void OnUpdate(float deltaTime) override;

    void Kill();

private:
    class AABBColliderComponent* mColliderComponent;
};

