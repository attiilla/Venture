//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "MainChar.h"
#include "enemy_1.h"
#include "Gerold.h"

Spawner::Spawner(Game* game, float spawnDistance, int enemy)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
        ,mEnemy(enemy)
{

}

void SpawneMultir::OnUpdate(float deltaTime)
{
    if (abs(GetGame()->GetMainChar()->GetPosition().x - GetPosition().x) < mSpawnDistance)
    {
        SDL_Log("enemy %d",mEnemy);
        if (mEnemy == 1) {
            auto enemy = new Gerold(GetGame());
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==0){
            auto enemy = new enemy_1(GetGame());
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        }
    }
}