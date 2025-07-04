//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIButton.h"

UIButton::UIButton(const std::string& text, class UIFont* font, std::function<void()> onClick,
                    const Vector2& pos, const Vector2& size, const Vector3& color,
                    int pointSize , unsigned wrapLength,
                    const Vector2 &textPos, const Vector2 &textSize, const Vector3& textColor)
        :UIElement(pos, size, color)
        ,mOnClick(onClick)
        ,mHighlighted(false)
        ,mText(text, font, pointSize, wrapLength, textPos, textSize, textColor)
{

}

UIButton::~UIButton()
{

}


void UIButton::Draw(SDL_Renderer *renderer, const Vector2 &screenPos)
{
    SDL_Rect titleQuad =  {
        static_cast<int>(mPosition.x+screenPos.x),
        static_cast<int>(mPosition.y+screenPos.y),
        static_cast<int>(mSize.x),
        static_cast<int>(mSize.y)
    };
  

    if (mHighlighted) {
        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        SDL_RenderFillRect(renderer, &titleQuad);

        // Contorno branco (2px de largura)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco
        SDL_Rect outlineRect = {
            titleQuad.x - 2,
            titleQuad.y - 2,
            titleQuad.w + 4,
            titleQuad.h + 4
        };
        SDL_RenderDrawRect(renderer, &outlineRect);
    }


    auto textSize = mText.GetSize();
    mText.Draw(renderer, Vector2(mPosition.x+(mSize.x-textSize.x)/2, mPosition.y+(mSize.y-textSize.y)/2));
}

void UIButton::OnClick()
{
    if (mOnClick!=nullptr) {
        mOnClick();
    }
    
}
