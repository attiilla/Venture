//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "MainChar.h"
#include "Block.h"
#include "Projectile.h"
#include "CloudEffect.h"
#include "FlameEffect.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "Enemies/Iga.h"

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

    // float collider_width = Game::TILE_SIZE;
    // float collider_height = Game::TILE_SIZE;
    // float offsetY = Game::TILE_SIZE * 0.5f;
    // Vector2 v1 = Vector2(0, offsetY);
    // Vector2 v2 = Vector2(0, offsetY+collider_height);
    // Vector2 v3 = Vector2(collider_width, collider_height+offsetY);
    // Vector2 v4 = Vector2(collider_width, offsetY);
    // std::vector<Vector2> v = { v1, v2, v3, v4 };
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 26,32,
    element==ElementState::Water?ColliderLayer::PlayerW:ColliderLayer::PlayerF);

    mDrawComponent = new DrawAnimatedComponent(this,
                                              "../Assets/Sprites/Chars/animations.png",
                                              "../Assets/Sprites/Chars/animations.json");
    // new DrawPolygonComponent(this, v,101);
    mDrawComponent->AddAnimation("Idle_Fire", {0});
    mDrawComponent->AddAnimation("Walking_Fire", {0,1,2,3,4});
    mDrawComponent->AddAnimation("Idle_Water", {5});
    mDrawComponent->AddAnimation("Walking_Water", {5,6,7,8,9});

    mDrawComponent->SetAnimation(element == ElementState::Water ? "Idle_Water" : "Idle_Fire");
    mDrawComponent->SetAnimFPS(10.0f);
}

void MainChar::OnProcessInput(const uint8_t* state)
{
    if(mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    if (state[SDL_SCANCODE_RIGHT])
    {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * mForwardSpeed);
        mRotation = 0.0f;
        mIsRunning = true;
    }

    if (state[SDL_SCANCODE_LEFT])
    {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * -mForwardSpeed);
        mRotation = Math::Pi;
        mIsRunning = true;
    }

    if (!state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT])
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

            Vector2 cloudPos = GetPosition();
            cloudPos.y += mColliderComponent->GetHeight() / 3;
            cloudPos.x -= mColliderComponent->GetWidth() / 3;

            new CloudEffect(GetGame(), cloudPos);

            if (const auto temp = mGame->GetAudio()->PlaySound("Jump.mp3", false); !temp.IsValid()) {
                SDL_Log("Failed to play background music: Jump.mp3");
            } else {
                SDL_Log("Playing musical effect: Jump.mp3");
            }
        }
    } else if (key == SDLK_z && isPressed) {
        SwapElement();
    } else if (key == SDLK_x && isPressed) {
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
    } else if (key == SDLK_r && isPressed)
    {
        if (mGame->GetDiamondCount() > 0)
        {
            //mGame->DecreaseDiamond();
            Kill();
        }
    }
}

void MainChar::OnUpdate(float deltaTime)
{
    //SDL_Log("Position: %d;%d",static_cast<int>(mPosition.x),static_cast<int>(mPosition.y));
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
    if (mGame->GetGamePlayState() == Game::GamePlayState::Playing && mPosition.y > 4200)
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

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    // Só planear quando estiver no modo fogo, no ar, caindo e segurando Space
    bool wantGlide =
        (mElement == ElementState::Fire) &&
        (!mIsOnGround) &&
        (mRigidBodyComponent->GetVelocity().y > 0.0f) &&
        state[SDL_SCANCODE_SPACE];

    if (wantGlide)
    {
        // cria só se ainda não existir
        if (!mFlameEffect)
        {
            mFlameEffect = new FlameEffect(GetGame(), this);
        }
    }
    else
    {
        // remove se existir
        if (mFlameEffect)
        {
            mFlameEffect->SetState(ActorState::Destroy);
            mFlameEffect = nullptr;
        }
    }
}

void MainChar::ManageAnimations()
{
    if (mIsRunning) {
        if (mElement == ElementState::Water) {
            mDrawComponent->SetAnimation("Walking_Water");
        } else if (mElement == ElementState::Fire) {
            mDrawComponent->SetAnimation("Walking_Fire");
        }
    } else {
        if (mElement == ElementState::Water) {
            mDrawComponent->SetAnimation("Idle_Water");
        } else if (mElement == ElementState::Fire) {
            mDrawComponent->SetAnimation("Idle_Fire");
        }
    }
}

void MainChar::Kill()
{
    auto deadHandle = GetGame()->GetAudio()->PlaySound("Dead.wav", false);
    if (!deadHandle.IsValid()) {
        SDL_Log("Erro ao tocar Dead.wav");
    }
    if (mGame->GetDiamondCount()>0) {
        mGame->DecreaseDiamond();
        Vector2 cp = GetGame()->GetLastCheckpoint();
        Vector2 respawnPos(cp.x, cp.y);
        SDL_Log("Respawn em (%.1f,%.1f)", respawnPos.x, respawnPos.y);
        SetPosition(respawnPos);

        mRigidBodyComponent->SetVelocity(Vector2::Zero);
        mRigidBodyComponent->SetEnabled(true);
        mColliderComponent   ->SetEnabled(true);
        mIsOnGround = false;
        mHasDoubleJumped = false;

        GetGame()->SetGamePlayState(Game::GamePlayState::Playing);

        auto music = GetGame()->GetMusicHandle();
        if (music.IsValid()) {
            GetGame()->GetAudio()->ResumeSound(music);
        }
    } else {
        mIsDying = true;
        mGame->SetGamePlayState(Game::GamePlayState::GameOver);

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
        mGame->ResetDiamonds();
        mGame->ResetGameScene(3.5f); // Reset the game scene after 3 seconds
    }
}

void MainChar::Win(AABBColliderComponent *chestCollider)
{
    mGame->SetGamePlayState(Game::GamePlayState::LevelComplete);

    // Set mario velocity to go down
    mRigidBodyComponent->SetVelocity(Vector2::Zero); // 100 pixels per second
    mRigidBodyComponent->SetApplyGravity(false);

    // Disable collider
    chestCollider->SetEnabled(false);

    //mPosition.Set(chestCollider->GetOwner()->GetPosition().x + Game::TILE_SIZE / 4.0f, mPosition.y);

    mGame->GetAudio()->StopAllSounds();

    mGame->WinGame();
}

void MainChar::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (other->GetLayer()==ColliderLayer::Enemy)
    {
        auto iga = dynamic_cast<Iga*>(other->GetOwner());
        if (iga==nullptr) {
            Kill();
        } else {
            iga->Kill();
        }

    }
    else if (other->GetLayer() == ColliderLayer::Chest)
    {
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
    else if (other->GetLayer() == ColliderLayer::Enemy_Projectile) {
        other->GetOwner()->Kill();
        Kill();
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
            //block->OnBump();
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
    }else if (other->GetLayer() == ColliderLayer::Chest) {
        Win(other);
    }else if (other->GetLayer() == ColliderLayer::Enemy_Projectile) {
        other->GetOwner()->Kill();
        Kill();
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
        // mDrawComponent.
    } else {
        mElement = ElementState::Water;
        mColliderComponent->SetLayer(ColliderLayer::PlayerW);
    }
}

bool MainChar::IsCharToLeft(Vector2 position) const{
    return mPosition.x < position.x;
}

float MainChar::GetColliderHeight() const
{
    return mColliderComponent->GetHeight();
}
