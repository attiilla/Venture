#include "BannerScreen.h"
#include "../Game.h"

BannerScreen::BannerScreen(Game* game,
                           const std::string& imagePath,
                           float delay,
                           float stay,
                           float speed,
                           const Vector2& size)
  : UIScreen(game, /*fontName=*/"")
  , mState(BannerState::Delay)
  , mTimer(0.0f)
  , mDelay(delay)
  , mStay(stay)
  , mSpeed(speed)
  , mSize(size)
{
    Vector2 startPos(
		static_cast<float>(game->GetWindowWidth()),
		(static_cast<float>(game->GetWindowHeight()) - size.y) * 0.5f
	);
    mImage = AddImage(GetGame()->GetRenderer(),
                      imagePath,
                      startPos,
                      size);
}

void BannerScreen::Update(float dt)
{
    UIScreen::Update(dt);

    Vector2 pos = mImage->GetPosition();
    switch (mState)
    {
        case BannerState::Delay:
            mTimer += dt;
            if (mTimer >= mDelay)
            {
                mTimer = 0.0f;
                mState = BannerState::Entering;
            }
            break;

        case BannerState::Entering:
        {
            float targetX = (GetGame()->GetWindowWidth() - mSize.x) * 0.5f;
            pos.x -= mSpeed * dt;
            if (pos.x <= targetX)
            {
                pos.x = targetX;
                mState = BannerState::Waiting;
                mTimer = 0.0f;
            }
            break;
        }

        case BannerState::Waiting:
            mTimer += dt;
            if (mTimer >= mStay)
            {
                mState = BannerState::Exiting;
            }
            break;

        case BannerState::Exiting:
            pos.x += mSpeed * dt;
            if (pos.x > GetGame()->GetWindowWidth())
            {
                Close();
            }
            break;
    }
    mImage->SetPosition(pos);
}