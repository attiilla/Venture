//
// Created by atila-rex on 6/15/25.
//
#include "Coin.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Coin::Coin(Game *game, const char* texturePath, const bool isStatic)
    :Actor(game)
{
    new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 10);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Coin, isStatic);
}

void Coin::Kill() {
    mGame->RemoveActor(this);
    mGame->IncreaseCoin();
    mGame->AddScore(100);

}

void Coin::OnUpdate(float deltaTime) {

}

