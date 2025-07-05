#include "FlameEffect.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"
#include "MainChar.h"

FlameEffect::FlameEffect(Game* game, MainChar* ownerChar)
  : Actor(game)
  , mOwnerChar(ownerChar)
{
    // Posiciona logo ao construir
    Vector2 pos = mOwnerChar->GetPosition();
    pos.y += mOwnerChar->GetColliderHeight();
    SetPosition(pos);

    // Sprite da chama (ajuste width/height ao seu asset)
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
        SDL_Log("❌ FlameEffect: falha ao tocar 'FlameLoop.wav'!");
    }
}

FlameEffect::~FlameEffect()
{
    // Para o som quando o efeito morre
    if (mSoundHandle.IsValid())
    {
        GetGame()->GetAudio()->StopSound(mSoundHandle);
    }
}

void FlameEffect::OnUpdate(float deltaTime)
{
    // Continua sob os pés do MainChar
    Vector2 pos = mOwnerChar->GetPosition();
    pos.y += mOwnerChar->GetColliderHeight();
    SetPosition(pos);
    // Este ator só morre quando MainChar sete Destroy (controle no MainChar)
}