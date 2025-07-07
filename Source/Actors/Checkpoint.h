#pragma once

#include "Actor.h"
#include <string>

class Checkpoint : public Actor
{
public:
    Checkpoint(class Game* game, const std::string& inactiveTexture, const std::string& activeTexture);

    void OnUpdate(float deltaTime) override;

private:
    bool mIsActivated;
    std::string mActiveTexturePath;

    class AABBColliderComponent* mColliderComponent;
    class DrawSpriteComponent* mDrawComponent;
};