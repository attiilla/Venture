#pragma once

#include "Block.h"

class ElementBlock : public Block {
public:
    ElementBlock(Game* game, const std::string &texturePath, ElementState kill, bool isLiquid, const bool isStatic = true);
    bool IsLiquid(){ return mLiquid; }
private:
    bool mLiquid;
};