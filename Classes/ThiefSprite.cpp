#include "ThiefSprite.h"

ThiefSprite::ThiefSprite() {
    mType = Constant::TYPE_THIEF;
}

ThiefSprite* ThiefSprite::create(const std::string &filename) {
    ThiefSprite* sprite = new ThiefSprite();
    if (sprite->initWithSpriteFrameName(filename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return sprite;
}

bool ThiefSprite::init() {
    return Sprite::init();
}
