#include "Checkpoint.h"
#include "../Game.h"
#include "../Actors/MainChar.h"                         // <<< este include
#include "../Components/ColliderComponents/AABBColliderComponent.h"  // <<< e este
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Checkpoint::Checkpoint(Game* game)
  : Actor(game)
{

}

void Checkpoint::OnUpdate(float)
{
    if (mActivated) return;

    Vector2 cpMin = GetPosition();
    Vector2 cpMax = cpMin + Vector2(Game::TILE_SIZE, Game::TILE_SIZE);

    auto player = const_cast<MainChar*>(GetGame()->GetMainChar());
    Vector2 pPos = player->GetPosition();
    auto pc = player->GetComponent<AABBColliderComponent>();
    float w = (float)pc->GetWidth();
    float h = (float)pc->GetHeight();
    Vector2 pMin{ pPos.x, pPos.y };
    Vector2 pMax{ pMin.x + w, pMin.y + h };

    bool overlap =
        pMin.x < cpMax.x && pMax.x > cpMin.x &&
        pMin.y < cpMax.y && pMax.y > cpMin.y;

    if (overlap)
    {
        mActivated = true;
        SDL_Log("Checkpoint ativado em (%.1f, %.1f)", cpMin.x, cpMin.y);
        GetGame()->SetLastCheckpoint(cpMin);
    }
}