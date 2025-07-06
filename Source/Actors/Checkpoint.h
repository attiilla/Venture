#pragma once
#include "Actor.h"

class AABBColliderComponent;
class DrawSpriteComponent;

class Checkpoint : public Actor
{
public:
    explicit Checkpoint(Game* game);
	
protected:
    void OnUpdate(float deltaTime) override;

private:
    bool mActivated = false;
};