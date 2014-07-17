#ifndef _SPLASH_SCENE_H_
#define _SPLASH_SCENE_H_

#include "cocos2d.h"

#include "Constant.h"
#include "MenuScene.h"

USING_NS_CC;

class SplashScene : public Layer {
private:
    void callback(float dt);
    void initBackground();
    void initTitle();
public:
    virtual bool init();
    static Scene* createScene();
    CREATE_FUNC(SplashScene);
    void onEnter();
};

#endif
