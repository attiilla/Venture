//
// Created by atila-rex on 6/24/25.
//
#include "Wood.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Wood::Wood(Game *game, const std::string &texturePath)
    :Actor(game)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 0.0f, true);
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 10);
    mColliderComponent = new AABBColliderComponent(this, 12, 0, 8, Game::TILE_SIZE, ColliderLayer::Wood);
}

// void Wood::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) {
//     SDL_Log("h");
//     if (other->GetLayer() == ColliderLayer::Projectile) {
//         Kill();
//     }
// }

void Wood::Kill() {
    SDL_Log("kill");
    SetState(ActorState::Destroy);
}