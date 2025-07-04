//
// Created by Lucas N. Ferreira on 28/05/25.
//

#include "UIImage.h"

UIImage::UIImage(SDL_Renderer* renderer, const std::string &imagePath, const Vector2 &pos, const Vector2 &size, const Vector3 &color)
    : UIElement(pos, size, color),
    mTexture(nullptr)
{
    SDL_Surface* surface = IMG_Load(imagePath.c_str());

    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
    mTexture = texture;
}

UIImage::~UIImage()
{
    if (mTexture!=nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

void UIImage::Draw(SDL_Renderer* renderer, const Vector2 &screenPos)
{
    if (mTexture==nullptr) {
        return;
    }

    SDL_Rect quad = {
        static_cast<int>(mPosition.x+screenPos.x),
        static_cast<int>(mPosition.y+screenPos.y),
        static_cast<int>(mSize.x),
        static_cast<int>(mSize.y)
    };

    SDL_RenderCopy(renderer, mTexture, nullptr, &quad);
}