//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "MainChar.h"
#include "Enemies/Junim.h"
#include "Enemies/Gerold.h"
#include "Enemies/Iga.h"

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
        if (mEnemy == 1) { //Inimigo que pula e corre
            auto enemy = new Gerold(GetGame(), ElementState::Fire);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==2){
            auto enemy = new Gerold(GetGame(), ElementState::Water);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==3) { //Inimigo basiquinho
            auto enemy = new Junim(GetGame(), ElementState::Fire);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==4) {
            auto enemy = new Junim(GetGame(), ElementState::Water);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==5) { //Inimigo que atira projétil
            auto enemy = new Iga(GetGame(), ElementState::Fire);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        } else if (mEnemy==6) {
            auto enemy = new Iga(GetGame(), ElementState::Water);
            enemy->SetPosition(GetPosition());
            mState = ActorState::Destroy;
        }
    }
}