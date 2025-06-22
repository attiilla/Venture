//
// Created by atila-rex on 6/15/25.
//

#ifndef COIN_H
#define COIN_H
#pragma once
#include <string>
#include "Actor.h"
class Coin : public Actor
{
public:
    explicit Coin(Game* game, const char* texturePath, const bool isStatic = true);

    void SetPosition(const Vector2& position)
    {
        Actor::SetPosition(position);
    }

    void OnUpdate(float deltaTime) override;

    void Kill();

private:
    class AABBColliderComponent* mColliderComponent;
};

#endif //COIN_H
