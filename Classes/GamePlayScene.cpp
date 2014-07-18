#include "GamePlayScene.h"

GamePlayScene::GamePlayScene() : mBoard(Board(this)) {}

Scene* GamePlayScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GamePlayScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GamePlayScene::init() {
    if (!Layer::init()) {
        return false;
    }
    initBackground();
    initGameBoard();
    initNavigator();
    initTouchListener();
    return true;
}

void GamePlayScene::initBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Sprite *background = Sprite::create("background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(background);
}

void GamePlayScene::initGameBoard() {
    mBoard.loadTiledMap("test.tmx");
    mTiledMap = mBoard.getTiledMap();
    // mTiledMap->setScale(2.0);

    addChild(mTiledMap);
}

void GamePlayScene::initNavigator() {
    MenuItemImage *back = MenuItemImage::create("back.png", "back.png", CC_CALLBACK_1(GamePlayScene::navigatorCallback, this));
    back->setTag(Constant::MENU_BACK_TAG);
    MenuItemImage *start = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(GamePlayScene::navigatorCallback, this));
    start->setTag(Constant::MENU_START_TAG);

    Menu *menu = Menu::create(back, start, NULL);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    back->setPosition(Vec2(origin.x + visibleSize.width - back->getContentSize().width / 2,
                                    origin.y + back->getContentSize().height / 2));
    start->setPosition(Vec2(origin.x + start->getContentSize().width / 2,
                                    origin.y + back->getContentSize().height / 2));
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
}

void GamePlayScene::navigatorCallback(Ref* sender) {
    MenuItem *item = (MenuItem *) sender;
    int tag = item->getTag();
    if (tag == Constant::MENU_BACK_TAG) {
        back();
    } else if (tag == Constant::MENU_START_TAG) {
        start();
    }
}

void GamePlayScene::start() {
    mBoard.startPlay();
}

void GamePlayScene::back() {
    mBoard.stopPlay();
    Scene *menuScene = MenuScene::createScene();
    auto transition = TransitionFade::create(1.0f, menuScene);
    Director::getInstance()->replaceScene(transition);
}

void GamePlayScene::initTouchListener() {
    setTouchEnabled(true);
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GamePlayScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GamePlayScene::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

bool GamePlayScene::onTouchBegan(Touch *touch, Event *unused_event) {
    return true;
}

void GamePlayScene::onTouchEnded(Touch *touch, Event *unused_event) {
    mBoard.onTouch(touch);
}
