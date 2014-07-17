#ifndef _THIEF_CHARACTER_H_
#define _THIEF_CHARACTER_H_

#include "cocos2d.h"
#include "Constant.h"
#include "AbsSprite.h"

USING_NS_CC;

class ThiefSprite : public AbsSprite {
public:
    ThiefSprite();
    static ThiefSprite* create(const std::string &filename);
    virtual bool init();
};

#endif

