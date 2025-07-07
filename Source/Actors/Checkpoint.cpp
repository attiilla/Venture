#include "Checkpoint.h"
#include "../Game.h"
#include "MainChar.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../AudioSystem.h"

Checkpoint::Checkpoint(Game* game, const std::string& inactiveTexture, const std::string& activeTexture)
    : Actor(game)
    , mIsActivated(false)
    , mActiveTexturePath(activeTexture)
{
    mDrawComponent = new DrawSpriteComponent(this, inactiveTexture, Game::TILE_SIZE, Game::TILE_SIZE, 50);

    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Coin);
    mColliderComponent->SetEnabled(false);
}

void Checkpoint::OnUpdate(float deltaTime)
{
    if (!mIsActivated)
    {
        auto player = GetGame()->GetMainChar();
        if (player && player->GetState() == ActorState::Active)
        {
            auto playerCollider = player->GetComponent<AABBColliderComponent>();
            if (mColliderComponent->Intersect(*playerCollider))
            {
                mIsActivated = true;

                GetGame()->SetLastCheckpoint(GetPosition());

                GetGame()->GetAudio()->PlaySound("Coin.wav");
            }
        }
    }
}