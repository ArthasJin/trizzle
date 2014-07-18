#include "SplashScene.h"

Scene* SplashScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool SplashScene::init() {
    if (!Layer::init()) {
        return false;
    }
    initBackground();
    initTitle();
    return true;
}

void SplashScene::onEnter() {
    Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(SplashScene::callback), this, 1.0f, 1.0f, 0, false);
}

void SplashScene::initBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Sprite *background = Sprite::create("background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(background);
}

void SplashScene::initTitle() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    LabelTTF *label = LabelTTF::create(Constant::GAME_NAME, Constant::FONT, 120, CCSizeMake(360, 280), kCCTextAlignmentCenter);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(label);
}

void SplashScene::callback(float dt) {
    Scene *menuScene = MenuScene::createScene();
    auto transition = TransitionFade::create(1.0f, menuScene);
    Director::getInstance()->replaceScene(transition);
}
