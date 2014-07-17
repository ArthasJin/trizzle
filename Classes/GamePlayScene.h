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
    Board mBoard;

    TMXTiledMap *mTiledMap;
    void initBackground();
    void initGameBoard();
    void initNavigator();
    void initTouchListener();
    void navigatorCallback(Ref* sender);
    void start();
    void back();
    void setPlayerPosition(Vec2 &postion);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    void onUpdate(float dt);
public:
    static Scene* createScene();
    GamePlayScene();
    virtual bool init();
    CREATE_FUNC(GamePlayScene);
};

#endif
