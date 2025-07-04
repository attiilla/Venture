#pragma once
#include "Actor.h"

class DrawSpriteComponent;

class CloudEffect : public Actor
{
public:
    explicit CloudEffect(Game* game, const Vector2& pos, float lifetime = 0.5f);

protected:
    void OnUpdate(float deltaTime) override;

private:
    DrawSpriteComponent* mSprite;
    float mLifetime;
    float mAge = 0.0f;
};