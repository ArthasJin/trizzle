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
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    static Scene* createScene();
    CREATE_FUNC(SplashScene);
    void onEnter();
};

#endif
