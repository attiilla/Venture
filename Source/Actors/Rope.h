//
// Created by atila-rex on 6/24/25.
//

#pragma once
#include <string>
#include <vector>
#include "Actor.h"

class Rope : public Actor
{
public:
    explicit Rope(Game *game, const std::string &texturePath, const bool isStatic = true, const bool vertical = true);

    void Kill() override;
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
private:
    std::vector<Vector2> mOccupied;
    class AABBColliderComponent* mColliderComponent;
    class DrawAnimatedComponent* mDrawComponent;
};