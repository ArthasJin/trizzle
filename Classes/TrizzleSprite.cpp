#include "TrizzleSprite.h"

TrizzleSprite::TrizzleSprite(int type, bool isEnemy) : mType(type), mIsEnemy(isEnemy), mIsAlive(true) {}

TrizzleSprite::~TrizzleSprite() {}

int TrizzleSprite::getType() {
    return mType;
}

TrizzleSprite* TrizzleSprite::create(const string &filename, int type, bool isEnemy) {
    TrizzleSprite *sprite = new TrizzleSprite(type, isEnemy);
    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

bool TrizzleSprite::isEmeny() {
    return mIsEnemy;
}

bool TrizzleSprite::isAlive() {
    return mIsAlive;
}

void TrizzleSprite::setAlive(bool alive) {
    mIsAlive = alive;
}
