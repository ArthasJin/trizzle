#ifndef _KNIGHT_CHARACTER_H_
#define _KNIGHT_CHARACTER_H_

#include "cocos2d.h"
#include "Constant.h"
#include "AbsSprite.h"

USING_NS_CC;

class KnightSprite : public AbsSprite {
public:
    KnightSprite();
    CREATE_FUNC(KnightSprite);
    virtual bool init();
};

#endif
