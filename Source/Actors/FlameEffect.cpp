#include "FlameEffect.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"
#include "MainChar.h"

FlameEffect::FlameEffect(Game* game, MainChar* ownerChar)
  : Actor(game)
  , mOwnerChar(ownerChar)
{
    Vector2 pos = mOwnerChar->GetPosition();
    pos.x -= 5;
    pos.y += mOwnerChar->GetColliderHeight() - 7;
    SetPosition(pos);

    mSprite = new DrawSpriteComponent(
        this,
        "../Assets/Sprites/Effects/flame.png",
        /*w=*/32, /*h=*/32,
        /*drawOrder=*/50
    );

    // Toca som em loop
    mSoundHandle = game->GetAudio()->PlaySound("FlameLoop.wav", true);
    if (!mSoundHandle.IsValid())
    {
        SDL_Log("FlameEffect: falha ao tocar 'FlameLoop.wav'!");
    }
}

FlameEffect::~FlameEffect()
{
    if (mSoundHandle.IsValid())
    {
        GetGame()->GetAudio()->StopSound(mSoundHandle);
    }
}

void FlameEffect::OnUpdate(float deltaTime)
{
    Vector2 pos = mOwnerChar->GetPosition();
    pos.x -= 5;
    pos.y += mOwnerChar->GetColliderHeight() - 7;
    SetPosition(pos);
}