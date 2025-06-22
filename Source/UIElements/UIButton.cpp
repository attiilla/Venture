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
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Crie um SDL_Rect chamado titleQuad com a posição relativa do botão na tela. Some a posição do botão
    //  (mPosition) com a posição da tela (screenPos) para obter a posição final do botão.
    //  Use mSize para definir a largura e altura.
    SDL_Rect titleQuad =  {
        static_cast<int>(mPosition.x+screenPos.x),
        static_cast<int>(mPosition.y+screenPos.y),
        static_cast<int>(mSize.x),
        static_cast<int>(mSize.y)
    };
  

    // TODO 2.: Verifique se o botão está destacado (mHighlighted). Se sim, defina a cor de preenchimento do
    //  retângulo como laranja (200, 100, 0, 255) usando SDL_SetRenderDrawColor. Em seguida,
    //  desenhe o retângulo usando SDL_RenderFillRect com o renderer passado como parâmetro.
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


    // TODO 3.: Desenhe o texto do botão usando o método Draw da classe UIText. Use posição relativa ao botão, ou seja,
    //  a posição do texto deve ser o centro do botão menos a metade do tamanho do texto.
    auto textSize = mText.GetSize();
    mText.Draw(renderer, Vector2(mPosition.x+(mSize.x-textSize.x)/2, mPosition.y+(mSize.y-textSize.y)/2));
}

void UIButton::OnClick()
{
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Verifique se o ponteiro mOnClick não é nulo. Se não for, chame a função mOnClick().
    if (mOnClick!=nullptr) {
        mOnClick();
    }
    
}
