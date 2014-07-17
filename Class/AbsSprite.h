#ifndef _ABSSPRITE_H_
#define _ABSSPRITE_H_

#include "cocos2d.h"

USING_NS_CC;


class AbsSprite : public Sprite {
protected:
    int mType;
public:
    CREATE_FUNC(AbsSprite);
    virtual int getType();
};

#endif
