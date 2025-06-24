//
// Created by atila-rex on 6/24/25.
//
#include "Rope.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"

Rope::Rope(Game *game, const std::string &texturePath, unsigned int lenght, const bool isStatic, const bool vertical)
    :Actor(game), mVertical(vertical), mLength(lenght)
{
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE/4, Game::TILE_SIZE, 10);
    if (mVertical) {
        mColliderComponent = new AABBColliderComponent(this,13,0,Game::TILE_SIZE/4,Game::TILE_SIZE,
            ColliderLayer::Rope,true,10);
    } else {
        mColliderComponent = new AABBColliderComponent(this,0,13,Game::TILE_SIZE,Game::TILE_SIZE/4,
            ColliderLayer::Rope,true,10);
    }
}
