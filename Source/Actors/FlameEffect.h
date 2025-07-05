#pragma once
#include "Actor.h"
#include "../AudioSystem.h"

class MainChar;
class DrawSpriteComponent;

class FlameEffect : public Actor
{
public:
    explicit FlameEffect(Game* game, MainChar* ownerChar);
    ~FlameEffect() override;

protected:
    void OnUpdate(float deltaTime) override;

private:
    MainChar* mOwnerChar;
    DrawSpriteComponent* mSprite;
    SoundHandle mSoundHandle;
};