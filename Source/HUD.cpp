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
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.: Adicione um texto com a string "Time" no canto superior direito da tela, como no jogo orginal. Note que
    //  a classe HUD tem constantes WORD_HEIGHT, WORD_OFFSET, CHAR_WIDTH, POINT_SIZE e HUD_POS_Y que podem ser usadas
    //  para posicionar e definir o tamanho do texto.
    AddText("Time",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (4 * CHAR_WIDTH)), HUD_POS_Y),
        Vector2(4 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    // TODO 2.: Adicione um texto com a string "400" (400 segundos) logo abaixo do texto "Time".
    //  Guarde o ponteiro do texto em um membro chamado mTimeText.
    mTimeText = AddText("400",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (3 * CHAR_WIDTH)), HUD_POS_Y+WORD_HEIGHT),
        Vector2(3 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    // TODO 3.: Adicione um texto com a string "World" à esquerda do texto "Time", como no jogo original.
    AddText("World",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (11 * CHAR_WIDTH)), HUD_POS_Y),
        Vector2(5 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    // TODO 4.: Adicione um texto com a string "1-1" logo abaixo do texto "World".
    mLevelName = AddText("1-1",
        Vector2(mGame->GetWindowWidth() - (WORD_OFFSET + (11 * CHAR_WIDTH)), HUD_POS_Y+WORD_HEIGHT),
        Vector2(3 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    // TODO 5.: Adicione um texto com a string "Mario" no canto superior esquerdo da tela
    AddText("Mario",
        Vector2(WORD_OFFSET, HUD_POS_Y),
        Vector2(5 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    // TODO 6.: Adicione um texto com a string "000000" logo abaixo do texto "Mario"
    mScoreCounter = AddText("000000",
        Vector2(WORD_OFFSET, HUD_POS_Y + WORD_HEIGHT),
        Vector2(6 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    AddText("Coins",
        Vector2(WORD_OFFSET+ (8 * CHAR_WIDTH), HUD_POS_Y),
        Vector2(5 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

    mCoinsCounter = AddText("000",
    Vector2(WORD_OFFSET+ (9 * CHAR_WIDTH), HUD_POS_Y+WORD_HEIGHT),
        Vector2(3 * CHAR_WIDTH, WORD_HEIGHT),
        POINT_SIZE);

}

HUD::~HUD()
{

}

void HUD::SetTime(int time)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.: Utilize o método SetText() do mTimeText para atualizar o texto com o tempo restante. Lembre-se que
    //  o tempo é um inteiro que representa os segundos restantes, e deve ser convertido para string.
    mTimeText->SetText(std::to_string(time));

    // TODO 2.: A posição e o tamanho do texto irão mudar dependendo do número de dígitos na variável time.
    //  Ajuste a posição e o tamanho do mTimeText de acordo com o número de dígitos, de tal forma que
    //  o texto fique alinhado à direita com o texto "Time" e o tamanho do texto seja proporcional ao número de dígitos.


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
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.: Utilize o método SetText() do mLevelName para atualizar o texto com o nome do nível.
    mLevelName->SetText(levelName);
}

void HUD::SetCoinCount(const unsigned int coinCount)
{
    std::string sCoins = std::to_string(coinCount);
    int offset = 3-sCoins.length();
    std::string prefix = std::string(offset, '0');
    mCoinsCounter->SetText(prefix + sCoins);
}

void HUD::SetScore(const unsigned int score) {
    std::string sScore = std::to_string(score);
    int offset = 6-sScore.length();
    std::string prefix = std::string(offset, '0');
    mScoreCounter->SetText(prefix + sScore);
}