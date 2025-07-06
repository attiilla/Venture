//
// Created by atila-rex on 6/15/25.
//
#include "Diamond.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Diamond::Diamond(Game *game, std::string texturePath, const bool isStatic)
    :Actor(game)
{
    new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 10);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Coin, isStatic);
}

void Diamond::Kill() {
    mGame->RemoveActor(this);
    mGame->IncreaseDiamond();
    //mGame->AddScore(100);

}

void Diamond::OnUpdate(float deltaTime) {

}

