//
// Created by atila_g7 on 06/07/25.
//
#include "ElementBlock.h"
#include "../Game.h"

ElementBlock::ElementBlock(Game* game, const std::string &texturePath, ElementState kill, const bool isLiquid, const bool isStatic)
    : Block(game, texturePath, isStatic)
{
    ColliderLayer layer = ColliderLayer::Blocks;
    if (kill==ElementState::Fire && isLiquid){
        layer = ColliderLayer::Water;
    } else if (kill==ElementState::Fire && !isLiquid) {
        layer = ColliderLayer::Ice;
    } else if (kill==ElementState::Water && isLiquid) {
        layer = ColliderLayer::Lava;
    } else if (kill==ElementState::Water && !isLiquid) {
        layer = ColliderLayer::Flame;
    }
    delete mColliderComponent;
    mColliderComponent= new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, layer, isStatic);
}