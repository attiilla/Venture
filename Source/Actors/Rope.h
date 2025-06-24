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
    explicit Rope(Game *game, const std::string &texturePath, unsigned int length, const bool isStatic = true, const bool vertical = true);
    //void Kill() override;
    //void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    //void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
private:
    unsigned int mLength;
    class AABBColliderComponent* mColliderComponent;
    class DrawSpriteComponent* mDrawComponent;
    bool mVertical;
};