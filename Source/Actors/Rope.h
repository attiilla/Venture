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
    explicit Rope(Game *game, const std::string &texturePath, unsigned int length, bool isStatic = true, bool vertical = true);
    //void Kill() override;
    //void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    //void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
    unsigned int GetLength() const{ return mLength; }
private:
    unsigned int mLength;
    class AABBColliderComponent* mColliderComponent;
    class DrawSpriteComponent* mDrawComponent;
    bool mVertical;
};