//
// Created by Lucas N. Ferreira on 08/12/23.
//

#include "HUD.h"

#include <algorithm>

#include "Game.h"
#include "UIElements/UIText.h"

HUD::HUD(class Game* game, const std::string& fontName)
    :UIScreen(game, fontName)
{
    AddText("Time",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (4 * CHAR_WIDTH)), HUD_POS_Y),
        Vector2(4 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    mTimeText = AddText("300",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (3 * CHAR_WIDTH)), HUD_POS_Y+WORD_HEIGHT),
        Vector2(3 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    AddText("Level",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (11 * CHAR_WIDTH)), HUD_POS_Y),
        Vector2(5 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    mLevelName = AddText("1-1",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (11 * CHAR_WIDTH)), HUD_POS_Y+WORD_HEIGHT),
        Vector2(3 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    AddText("Venture",
        Vector2(WORD_OFFSET, HUD_POS_Y),
        Vector2(5 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    mScoreCounter = AddText("Schmucks!",
        Vector2(WORD_OFFSET, HUD_POS_Y + WORD_HEIGHT),
        Vector2(6 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    AddText("Diamonds",
        Vector2(WORD_OFFSET+ (8 * CHAR_WIDTH), HUD_POS_Y),
        Vector2(8 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    mDiamondsCounter = AddText("000",
    Vector2(WORD_OFFSET+ (10.5 * CHAR_WIDTH), HUD_POS_Y+WORD_HEIGHT),
        Vector2(3 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

}

HUD::~HUD()
{

}

void HUD::Win() {
    AddText("YOU WIN!",
        Vector2(mGame->GetWindowWidth()/2 - (16 * CHAR_WIDTH),mGame->GetWindowHeight()/2-2*WORD_HEIGHT),
        Vector2(32 * CHAR_WIDTH, 4*WORD_HEIGHT),
        POINT_SIZE);
}

void HUD::SetTime(int time)
{
    mTimeText->SetText(std::to_string(time));

    if (time < 100 && 10<time) {
        mTexts.erase(std::remove(mTexts.begin(), mTexts.end(), mTimeText), mTexts.end());
        mTimeText = AddText(std::to_string(time),
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (2 * CHAR_WIDTH)), HUD_POS_Y+WORD_HEIGHT),
        Vector2(2 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);
    } else if (time<10) {
        mTexts.erase(std::remove(mTexts.begin(), mTexts.end(), mTimeText), mTexts.end());
        mTimeText = AddText(std::to_string(time),
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (CHAR_WIDTH)), HUD_POS_Y+WORD_HEIGHT),
        Vector2(CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);
    }
}

void HUD::SetLevelName(const std::string &levelName)
{
    mLevelName->SetText(levelName);
}

void HUD::SetDiamondCount(const unsigned int coinCount)
{
    std::string sDiamonds = std::to_string(coinCount);
    int offset = 3-sDiamonds.length();
    std::string prefix = std::string(offset, '0');
    mDiamondsCounter->SetText(prefix + sDiamonds);
}

void HUD::SetScore(const unsigned int score) {
    std::string sScore = std::to_string(score);
    int offset = 6-sScore.length();
    std::string prefix = std::string(offset, '0');
    mScoreCounter->SetText(prefix + sScore);
}