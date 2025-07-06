//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "MainChar.h"
#include "Junim.h"
#include "Gerold.h"

Spawner::Spawner(Game* game, float spawnDistance, int enemy)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
        ,mEnemy(enemy)
{

}

void Spawner::OnUpdate(float deltaTime)
{
    if (abs(GetGame()->GetMainChar()->GetPosition().x - GetPosition().x) < mSpawnDistance)
    {
        SDL_Log("enemy %d",mEnemy);
        if (mEnemy == 1) {
            auto enemy = new Gerold(GetGame(), ElementState::Fire);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==2){
            auto enemy = new Gerold(GetGame(), ElementState::Water);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==3) {
            auto enemy = new enemy_1(GetGame(), ElementState::Fire);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==4) {
            auto enemy = new Junim(GetGame(), ElementState::Water);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        }
    }
}