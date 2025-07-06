#pragma once
#include "UIScreen.h"
#include "UIImage.h"

enum class BannerState
{
    Delay,
    Entering,
    Waiting,
    Exiting
};

class BannerScreen : public UIScreen
{
public:
    BannerScreen(Game* game,
                 const std::string& imagePath,
                 float delay,
                 float stay,
                 float speed,
                 const Vector2& size);
    ~BannerScreen() override = default;

    void Update(float deltaTime) override;

private:
    UIImage*     mImage;
    BannerState  mState;
    float        mTimer;   // delay ou waiting
    float        mDelay;
    float        mStay;
    float        mSpeed;
    Vector2      mSize;
};