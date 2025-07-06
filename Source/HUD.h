//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include <string>

#include "UIElements/UIScreen.h"

class HUD : public UIScreen
{
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int WORD_OFFSET = 25.0f;
    const int CHAR_WIDTH = 20.0f;
    const int HUD_POS_Y = 10.0f;

    HUD(class Game* game, const std::string& fontName);
    ~HUD();
    void Win();
    // Reinsert the HUD elements
    void SetTime(int time);

    void SetLevelName(const std::string& levelName);

    void SetScore(unsigned int score);

    void SetDiamondCount(unsigned int coinCount);
private:
    // HUD elements
    UIText* mWinText;
    UIText* mDiamondsCounter;
    UIText* mScoreCounter;
    UIText* mLevelName;
    UIText* mLivesCount;
    UIText* mTimeText;
};
