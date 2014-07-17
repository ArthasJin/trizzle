#include "KnightSprite.h"

KnightSprite::KnightSprite() {
    mType = Constant::TYPE_KNIGHT;
}

bool KnightSprite::init() {
    return Sprite::init();
}

