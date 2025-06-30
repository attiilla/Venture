//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "MainChar.h"
#include "enemy_1.h"

Spawner::Spawner(Game* game, float spawnDistance)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
{

}

void Spawner::OnUpdate(float deltaTime)
{
    if (abs(GetGame()->GetMainChar()->GetPosition().x - GetPosition().x) < mSpawnDistance)
    {
        auto enemy = new enemy_1(GetGame());
        enemy->SetPosition(GetPosition());
        mState = ActorState::Destroy;
    }
}