#ifndef _WIZARD_CHARACTER_H_
#define _WIZARD_CHARACTER_H_

#include "cocos2d.h"
#include "Constant.h"
#include "AbsSprite.h"

USING_NS_CC;

class WizardSprite : public AbsSprite {
public:
    WizardSprite();
    CREATE_FUNC(WizardSprite);
    virtual bool init();
};

#endif
