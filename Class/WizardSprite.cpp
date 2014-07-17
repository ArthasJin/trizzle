#include "WizardSprite.h"

WizardSprite::WizardSprite() {
    mType = Constant::TYPE_WIZARD;
}

bool WizardSprite::init() {
    return Sprite::init();
}
