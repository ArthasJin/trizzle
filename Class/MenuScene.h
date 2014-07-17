#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "cocos2d.h"
#include "Constant.h"
#include "GamePlayScene.h"

USING_NS_CC;

class MenuScene : public Layer {
private:
    void initMenuItem();
    void initBackground();
    void menuCallback(Ref *sender);
    void play();
    void settings();
    void exit();
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MenuScene);
};

#endif
