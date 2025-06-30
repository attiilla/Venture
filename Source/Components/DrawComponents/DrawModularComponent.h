//
// Created by atila-rex on 6/27/25.
//
/*
#pragma once
#include <string>
#include <vector>
#include "DrawComponent.h"

class DrawModularComponent : public DrawComponent {
public:
    DrawModularComponent(class Actor* owner, const std::string &texturePath, int width = 0, int height = 0, int length, int drawOrder = 100);
    ~DrawModularComponent() override;

    void Draw(SDL_Renderer* renderer, const Vector3 &modColor = Color::White) override;
private:
    std::vector<SDL_Texture*> mSpriteSheetSurfaceVec;
    int mWidth;
    int mHeight;
    int mLength;
};
*/