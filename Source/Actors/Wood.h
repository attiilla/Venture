//
// Created by atila-rex on 6/24/25.
//

#pragma once
#include <string>
#include <vector>
#include "Actor.h"

class Wood : public Actor
{
public:
    explicit Wood(Game *game, const std::string &texturePath);
    void Kill() override;
private:
    class AABBColliderComponent* mColliderComponent;
    class DrawSpriteComponent* mDrawComponent;
    class RigidBodyComponent* mRigidBodyComponent;
};