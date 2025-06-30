//
// Created by atila-rex on 6/27/25.
//
#include "DrawModularComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"


DrawModularComponent::DrawModularComponent(class Actor* owner, const std::string &texturePath, const int width, const int height, int length, bool vertical, const int drawOrder)
        :DrawComponent(owner, drawOrder)
        ,mWidth(width)
        ,mHeight(height)
        ,mLength(length)
        ,mVert(vertical)
{
    for (int i = 0; i< mLength; i++)
    {
        mSpriteSheetSurfaceVec.push_back(mOwner->GetGame()->LoadTexture(texturePath));
    }
}

DrawModularComponent::~DrawModularComponent() {
    DrawComponent::~DrawComponent();
    for (int i = 0; i < mSpriteSheetSurfaceVec.size(); i++) {
        if (mSpriteSheetSurfaceVec[i] != nullptr) {
            SDL_DestroyTexture(mSpriteSheetSurfaceVec[i]);
            mSpriteSheetSurfaceVec[i] = nullptr;
        }
    }
}

void DrawModularComponent::Draw(SDL_Renderer *renderer, const Vector3 &modColor)
{
    SDL_Rect dstRect = {
        static_cast<int>(mOwner->GetPosition().x - mOwner->GetGame()->GetCameraPos().x),
        static_cast<int>(mOwner->GetPosition().y - mOwner->GetGame()->GetCameraPos().y),
        mWidth,
        mHeight
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (mOwner->GetRotation() == Math::Pi) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_SetTextureBlendMode(mSpriteSheetSurface, SDL_BLENDMODE_BLEND);

    SDL_SetTextureColorMod(mSpriteSheetSurface,
                           static_cast<Uint8>(modColor.x),
                           static_cast<Uint8>(modColor.y),
                           static_cast<Uint8>(modColor.z));

    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, nullptr, &dstRect, mOwner->GetRotation(), nullptr, flip);
}