//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "MainChar.h"
#include "Block.h"
#include "Projectile.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

MainChar::MainChar(Game* game, const float forwardSpeed, const float jumpSpeed, ElementState element)
        : Actor(game)
        , mIsRunning(false)
        , mIsOnPole(false)
        , mIsDying(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
        , mPoleSlideTimer(0.0f)
        , mElement(element)
        , mHasDoubleJumped(false)
        , mProjectileCooldown(0.0f)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE - 4.0f,Game::TILE_SIZE,
    element==ElementState::Water?ColliderLayer::PlayerW:ColliderLayer::PlayerF);

    mDrawComponent = new DrawAnimatedComponent(this,
                                              "../Assets/Sprites/Mario/Mario.png",
                                              "../Assets/Sprites/Mario/Mario.json");

    mDrawComponent->AddAnimation("Dead", {0});
    mDrawComponent->AddAnimation("idle", {1});
    mDrawComponent->AddAnimation("jump", {2});
    mDrawComponent->AddAnimation("run", {3, 4, 5});
    mDrawComponent->AddAnimation("win", {7});

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void MainChar::OnProcessInput(const uint8_t* state)
{
    if(mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    if (state[SDL_SCANCODE_D])
    {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * mForwardSpeed);
        mRotation = 0.0f;
        mIsRunning = true;
    }

    if (state[SDL_SCANCODE_A])
    {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * -mForwardSpeed);
        mRotation = Math::Pi;
        mIsRunning = true;
    }

    if (!state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A])
    {
        mIsRunning = false;
    }
    if (mElement == ElementState::Fire && !mIsOnGround && state[SDL_SCANCODE_SPACE]) {
        if (const Vector2 currentVelocity = mRigidBodyComponent->GetVelocity(); currentVelocity.y > 0) {
            constexpr float slowFallVelocity = 50.0f;
            mRigidBodyComponent->SetVelocity(Vector2(currentVelocity.x, Math::Min(currentVelocity.y, slowFallVelocity)));
        }
    }
}

void MainChar::OnHandleKeyPress(const int key, const bool isPressed)
{
    if(mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    // Jump
    if (key == SDLK_SPACE && isPressed)
    {
        if (mIsOnGround)
        {
            mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed));
            mIsOnGround = false;
            mHasDoubleJumped = false;

            if (const auto temp = mGame->GetAudio()->PlaySound("Jump.mp3", false); !temp.IsValid()) {
                SDL_Log("Failed to play background music: Jump.mp3");
            } else {
                SDL_Log("Playing musical effect: Jump.mp3");
            }
        }
        else if (mElement == ElementState::Water && !mHasDoubleJumped)
        {
            mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed));
            mHasDoubleJumped = true;

            if (const auto temp = mGame->GetAudio()->PlaySound("Jump.mp3", false); !temp.IsValid()) {
                SDL_Log("Failed to play background music: Jump.mp3");
            } else {
                SDL_Log("Playing musical effect: Jump.mp3");
            }
        }
    } else if (key == SDLK_z && isPressed) {
        SwapElement();
    } else if (key == SDLK_j && isPressed) {
        if (mProjectileCooldown <= 0.0f) {
            Projectile::ProjectileType projectileType;
            if (mElement == ElementState::Water) {
                projectileType = Projectile::ProjectileType::Water;
            } else {
                projectileType = Projectile::ProjectileType::Fire;
            }

            Vector2 spawnPos = GetPosition();
            float direction;

            if (mRotation == 0.0f) { // Facing right
                spawnPos.x += mColliderComponent->GetWidth() / 2 + 10.0f;
                direction = 1.0f;
            } else { // Facing left
                spawnPos.x -= mColliderComponent->GetWidth() / 2 + 10.0f;
                direction = -1.0f;
            }
            spawnPos.y += mColliderComponent->GetHeight() / 4.0f;

            new Projectile(mGame, projectileType, spawnPos, direction, 2.0f);

            const auto soundName = (projectileType == Projectile::ProjectileType::Fire) ? "Fire.wav" : "Water.wav";
            if (const auto temp = mGame->GetAudio()->PlaySound(soundName, false); !temp.IsValid()) {
                SDL_Log("Failed to play shoot sound");
            }

            mProjectileCooldown = PROJECTILE_COOLDOWN_TIME;
        } else {
            SDL_Log("Projectile on cooldown! %.2f seconds remaining.", mProjectileCooldown);
        }
    }
}

void MainChar::OnUpdate(float deltaTime)
{
    // Check if Mario is off the ground
    if (mRigidBodyComponent && mRigidBodyComponent->GetVelocity().y != 0)
    {
        mIsOnGround = false;
    }

    if (mProjectileCooldown > 0.0f) {
        mProjectileCooldown -= deltaTime;
    }

    // Limit Mario's position to the camera view
    mPosition.x = Math::Max(mPosition.x, mGame->GetCameraPos().x);

    // Kill mario if he falls below the screen
    if (mGame->GetGamePlayState() == Game::GamePlayState::Playing && mPosition.y > mGame->GetWindowHeight())
    {
        Kill();
    }

    if (mIsOnPole)
    {
        // If Mario is on the pole, update the pole slide timer
        mPoleSlideTimer -= deltaTime;
        if (mPoleSlideTimer <= 0.0f)
        {
            mRigidBodyComponent->SetApplyGravity(true);
            mRigidBodyComponent->SetApplyFriction(false);
            mRigidBodyComponent->SetVelocity(Vector2::UnitX * 100.0f);
            mGame->SetGamePlayState(Game::GamePlayState::Leaving);

            auto temp = mGame->GetAudio()->PlaySound("StageClear.wav", false);
            if (!temp.IsValid()) {
                SDL_Log("Failed to play background music: StageClear.wav");
            } else {
                SDL_Log("Playing musical effect: StageClear.wav");
            }
            mIsOnPole = false;
            mIsRunning = true;
        }
    }

    // If Mario is leaving the level, kill him if he enters the castle
    const float castleDoorPos = Game::LEVEL_WIDTH * Game::TILE_SIZE - 10 * Game::TILE_SIZE;

    if (mGame->GetGamePlayState() == Game::GamePlayState::Leaving &&
        mPosition.x >= castleDoorPos)
    {
        // Stop Mario and set the game scene to Level 2
        mState = ActorState::Destroy;
        mGame->SetGameScene(Game::GameScene::Level2, 3.5f);

        return;
    }

    ManageAnimations();
}

void MainChar::ManageAnimations()
{
    if(mIsDying)
    {
        mDrawComponent->SetAnimation("Dead");
    }
    else if(mIsOnPole)
    {
        mDrawComponent->SetAnimation("win");
    }
    else if (mIsOnGround && mIsRunning)
    {
        mDrawComponent->SetAnimation("run");
    }
    else if (mIsOnGround && !mIsRunning)
    {
        mDrawComponent->SetAnimation("idle");
    }
    else if (!mIsOnGround)
    {
        mDrawComponent->SetAnimation("jump");
    }
}

void MainChar::Kill()
{
    mIsDying = true;
    mGame->SetGamePlayState(Game::GamePlayState::GameOver);
    mDrawComponent->SetAnimation("Dead");

    // Disable collider and rigid body
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);

    mGame->GetAudio()->StopAllSounds();
    auto temp = mGame->GetAudio()->PlaySound("Dead.wav", false);
    if (!temp.IsValid()) {
        SDL_Log("Failed to play background music: Dead.wav");
    } else {
        SDL_Log("Playing musical effect: Dead.wav");
    }
    mGame->ResetScore();
    mGame->ResetCoins();
    mGame->ResetGameScene(3.5f); // Reset the game scene after 3 seconds
}

void MainChar::Win(AABBColliderComponent *poleCollider)
{
    mDrawComponent->SetAnimation("win");
    mGame->SetGamePlayState(Game::GamePlayState::LevelComplete);

    // Set mario velocity to go down
    mRigidBodyComponent->SetVelocity(Vector2::UnitY * 100.0f); // 100 pixels per second
    mRigidBodyComponent->SetApplyGravity(false);

    // Disable collider
    poleCollider->SetEnabled(false);

    // Adjust mario x position to grab the pole
    mPosition.Set(poleCollider->GetOwner()->GetPosition().x + Game::TILE_SIZE / 4.0f, mPosition.y);

    mGame->GetAudio()->StopAllSounds();

    mPoleSlideTimer = POLE_SLIDE_TIME; // Start the pole slide timer
}

void MainChar::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer() == ColliderLayer::Enemy)
    {
        Kill();
    }
    else if (other->GetLayer() == ColliderLayer::Pole)
    {
        mIsOnPole = true;
        Win(other);
    }
    else if (other->GetLayer() == ColliderLayer::Coin)
    {
        auto temp = mGame->GetAudio()->PlaySound("Coin.wav", false);
        if (!temp.IsValid()) {
            SDL_Log("Failed to play background music: Coin.wav");
        } else {
            SDL_Log("Playing musical effect: Coin.wav");
        }
        other->GetOwner()->Kill();
    }
}

void MainChar::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer() == ColliderLayer::Enemy)
    {
        other->GetOwner()->Kill();
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed / 2.5f));

        auto temp = mGame->GetAudio()->PlaySound("Stomp.wav", false);
        if (!temp.IsValid()) {
            SDL_Log("Failed to play background music: Stomp.wav");
        } else {
            SDL_Log("Playing musical effect: Stomp.wav");
        }
    }
    else if (other->GetLayer() == ColliderLayer::Blocks)
    {
        if (!mIsOnGround)
        {
            auto temp = mGame->GetAudio()->PlaySound("Bump.wav", false);
            if (!temp.IsValid()) {
                SDL_Log("Failed to play background music: Bump.wav");
            } else {
                SDL_Log("Playing musical effect: Bump.wav");
            }

            // Cast actor to Block to call OnBump
            auto* block = dynamic_cast<Block*>(other->GetOwner());
            block->OnBump();
        }
    }
    else if (other->GetLayer() == ColliderLayer::Coin)
    {
        auto temp = mGame->GetAudio()->PlaySound("Coin.wav", false);
        if (!temp.IsValid()) {
            SDL_Log("Failed to play background music: Coin.wav");
        } else {
            SDL_Log("Playing musical effect: Coin.wav");
        }
        other->GetOwner()->Kill();
    }
}

void MainChar::SetElement(ElementState element) {
    mElement = element;
    mColliderComponent->SetLayer(element==ElementState::Water?ColliderLayer::PlayerW:ColliderLayer::PlayerF);
}

void MainChar::SwapElement() {
    if (mElement == ElementState::Water) {
        mElement = ElementState::Fire;
        mColliderComponent->SetLayer(ColliderLayer::PlayerF);
    } else {
        mElement = ElementState::Water;
        mColliderComponent->SetLayer(ColliderLayer::PlayerW);
    }
}

bool MainChar::IsCharToLeft(Vector2 position) const{
    return mPosition.x < position.x;
}
