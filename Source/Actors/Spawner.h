//
// Created by Lucas N. Ferreira on 30/09/23.
//

#pragma once

#include "Actor.h"

class Spawner : public Actor
{
public:
    explicit Spawner(Game* game, float spawnDistance, int enemy);

    void OnUpdate(float deltaTime) override;
private:
    float mSpawnDistance;
    int mEnemy;
};
