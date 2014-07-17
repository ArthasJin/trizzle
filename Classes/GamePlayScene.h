#ifndef _GAMEPLAY_SCENE_H_
#define _GAMEPLAY_SCENE_H_

#include "cocos2d.h"
#include "Constant.h"
#include "MenuScene.h"
#include "Board.h"

USING_NS_CC;

class GamePlayScene : public Layer {
private:
    Sprite *mPlayer;

    TMXTiledMap *mTiledMap;
    void initBackground();
    void initTiledMap();
    void initNavigator();
    void initTouchListener();
    void navigatorCallback(Ref* sender);
    void start();
    void back();
    Vec2 tileCoordForPosition(Vec2 position);
    void setPlayerPosition(Vec2 &postion);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    void onUpdate(float dt);
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GamePlayScene);
};

#endif
