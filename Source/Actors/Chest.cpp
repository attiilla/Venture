//
// Created by atila-rex on 6/30/25.
//

#include "Chest.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Chest::Chest(Game *game, std::string texturePath, const bool isStatic)
    :Actor(game)
{
    new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 10);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Chest, isStatic);
}

void Chest::Kill() {

}

void Chest::OnUpdate(float deltaTime) {

}

