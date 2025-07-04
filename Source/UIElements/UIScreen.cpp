// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "../Game.h"
#include "UIFont.h"

UIScreen::UIScreen(Game* game, const std::string& fontName)
	:mGame(game)
	,mPos(0.f, 0.f)
	,mSize(0.f, 0.f)
	,mState(UIState::Active)
    ,mSelectedButtonIndex(-1)
{
    mGame->PushUI(this);

    mFont = mGame->LoadFont(fontName);
}

UIScreen::~UIScreen()
{
    for (UIText* text : mTexts) {
        delete text;
    }
    mTexts.clear();

    for (UIButton* button : mButtons) {
        delete button;
    }
    mButtons.clear();

    for (UIImage* image : mImages) {
        delete image;
    }
    mImages.clear();
}

void UIScreen::Update(float deltaTime)
{
	
}

void UIScreen::Draw(SDL_Renderer *renderer)
{
    for (UIText* text : mTexts) {
        text->Draw(renderer,mPos);
    }

    for (UIButton* button : mButtons) {
        button->Draw(renderer,mPos);
    }

    for (UIImage* image : mImages) {
        image->Draw(renderer,mPos);
    }
}

void UIScreen::ProcessInput(const uint8_t* keys)
{

}

void UIScreen::HandleKeyPress(int key)
{
    switch (key) {
        case SDLK_w:
            if (mGame->GetGameScene()==Game::GameScene::MainMenu) {
                mButtons[mSelectedButtonIndex]->SetHighlighted(false);
                mSelectedButtonIndex -= 1;
                if (mSelectedButtonIndex < 0) {
                    mSelectedButtonIndex = mButtons.size() - 1;
                }
                mButtons[mSelectedButtonIndex]->SetHighlighted(true);
            }
            break;
        case SDLK_s:
            if (mGame->GetGameScene()==Game::GameScene::MainMenu) {
                mButtons[mSelectedButtonIndex]->SetHighlighted(false);
                mSelectedButtonIndex += 1;
                if (mSelectedButtonIndex >= mButtons.size()) {
                    mSelectedButtonIndex = 0;
                }
                mButtons[mSelectedButtonIndex]->SetHighlighted(true);
            }
            break;
        case SDLK_RETURN:
            if (mSelectedButtonIndex >= 0 && mSelectedButtonIndex < mButtons.size()) {
                mButtons[mSelectedButtonIndex]->OnClick();
            }
            break;
        default:
            break;
    }
}

void UIScreen::Close()
{
	mState = UIState::Closing;
}

UIText* UIScreen::AddText(const std::string &name, const Vector2 &pos, const Vector2 &dims, const int pointSize, const int unsigned wrapLength)
{
    auto t = new UIText(name, mFont, pointSize, wrapLength, pos, dims);
    mTexts.push_back(t);
    return t;

}

UIButton* UIScreen::AddButton(const std::string& name, const Vector2 &pos, const Vector2& dims, std::function<void()> onClick)
{
    auto b = new UIButton(name, mFont, onClick, pos, dims, Vector3(255,165,0));
    mButtons.push_back(b);

    if (mButtons.size()==1) {
        mSelectedButtonIndex = 0;
        b->SetHighlighted(true);
    }

    return b;
}

UIImage* UIScreen::AddImage(SDL_Renderer* renderer, const std::string &imagePath, const Vector2 &pos, const Vector2 &dims, const Vector3 &color)
{
    auto img = new UIImage(renderer,imagePath,pos,dims,color);
    mImages.push_back(img);
    return img;
}