#include "CloudEffect.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"

CloudEffect::CloudEffect(Game* game, const Vector2& pos, float lifetime)
  : Actor(game)
  , mLifetime(lifetime)
{
    SetPosition(pos);

    mSprite = new DrawSpriteComponent(this,
        "../Assets/Sprites/Effects/cloud.png",
        /*width=*/60, /*height=*/60,
        /*drawOrder=*/50);

    mSprite->SetAlpha(255);
}

void CloudEffect::OnUpdate(float deltaTime)
{
    mAge += deltaTime;
    if (mAge >= mLifetime)
    {
        mState = ActorState::Destroy;
        return;
    }

    float t = mAge / mLifetime;
    Uint8 alpha = static_cast<Uint8>(255 * (1.0f - t));
    mSprite->SetAlpha(alpha);
}