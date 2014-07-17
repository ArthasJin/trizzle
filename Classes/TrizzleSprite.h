#ifndef _TRIZZLE_SPRITE_H_
#define _TRIZZLE_SPRITE_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class TrizzleSprite : public Sprite {
private:
    int mType;
    bool mIsEnemy;
    bool mIsAlive;
public:
    TrizzleSprite(int type, bool isEnemy);
    ~TrizzleSprite();
    static TrizzleSprite* create(const string &filename, int type, bool isEnemy);
    virtual int getType();
    virtual bool isEmeny();
    virtual bool isAlive();
    virtual void setAlive(bool alive);
};

#endif
