#include "GamePlayScene.h"

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
    initTiledMap();
    initNavigator();
    initTouchListener();
    return true;
}

void GamePlayScene::initBackground() {
    Size visibleSize = Director::sharedDirector()->getVisibleSize();
    Vec2 origin = Director::sharedDirector()->getVisibleOrigin();
    Sprite *background = Sprite::create("background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(background);
}

void GamePlayScene::initTiledMap() {
    // Board board("test.tmx");
    mTiledMap = TMXTiledMap::create("test.tmx");
    mTiledMap->setAnchorPoint(Vec2(0.5, 0.5));
    Size visibleSize = Director::sharedDirector()->getVisibleSize();
    Vec2 origin = Director::sharedDirector()->getVisibleOrigin();
    mTiledMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    // mTiledMap->setScale(2.0);

    addChild(mTiledMap);
}

void GamePlayScene::initNavigator() {
    MenuItemImage *back = MenuItemImage::create("back.png", "back.png", CC_CALLBACK_1(GamePlayScene::navigatorCallback, this));
    back->setTag(Constant::MENU_BACK_TAG);
    MenuItemImage *start = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(GamePlayScene::navigatorCallback, this));
    start->setTag(Constant::MENU_START_TAG);

    Menu *menu = Menu::create(back, start, NULL);
    Size visibleSize = Director::sharedDirector()->getVisibleSize();
    Vec2 origin = Director::sharedDirector()->getVisibleOrigin();
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
    log("start");
    schedule(schedule_selector(GamePlayScene::onUpdate), 1.0);
}

void GamePlayScene::back() {
    log("back");
    Scene *menuScene = MenuScene::createScene();
    auto transition = TransitionFade::create(1.0f, menuScene);
    Director::sharedDirector()->replaceScene(transition);
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
    log("onTouchBegan");
    return true;
}

void GamePlayScene::onTouchMoved(Touch *touch, Event *unused_event) {

}

void GamePlayScene::onTouchEnded(Touch *touch, Event *unused_event) {
    log("onTouchEnded");
    Vec2 point = touch->getLocationInView();
    point = Director::sharedDirector()->convertToGL(point);
    point = convertToNodeSpace(point);
    Sprite *s = Sprite::create("wizard.png");
    mPlayer = s;
    point = this->tileCoordForPosition(point);
    log("x = %f, y = %f", point.x, point.y);
    s->setAnchorPoint(Vec2::ZERO);
    s->setPosition(point);
    addChild(s);
//    Vec2 point = touch->getLocationInView();
//    point = Director::sharedDirector()->convertToGL(point);
//    point = convertToNodeSpace(point);
//    Vec2 playerPos = mPlayer->getPosition();
//    Vec2 diff;
//    Vec2::subtract(point, playerPos, &diff);

//    if ( abs(diff.x) > abs(diff.y) ) {
//        if (diff.x > 0) {
//            playerPos.x += mTiledMap->getTileSize().width;
//        } else {
//            playerPos.x -= mTiledMap->getTileSize().width;
//        }
//    } else {
//        if (diff.y > 0) {
//            playerPos.y += mTiledMap->getTileSize().height;
//        } else {
//            playerPos.y -= mTiledMap->getTileSize().height;
//        }
//    }
//    // safety check on the bounds of the map
//    if (playerPos.x <= (mTiledMap->getMapSize().width * mTiledMap->getTileSize().width) &&
//        playerPos.y <= (mTiledMap->getMapSize().height * mTiledMap->getTileSize().height) &&
//        playerPos.y >= 0 &&
//        playerPos.x >= 0)
//    {
//        setPlayerPosition(playerPos);
//    }
}

Vec2 GamePlayScene::tileCoordForPosition(Vec2 position) {
    int x = (int) (position.x / mTiledMap->getTileSize().width) * mTiledMap->getTileSize().width;
    int y = (int) (position.y / mTiledMap->getTileSize().height) * mTiledMap->getTileSize().height;
    return Vec2(x, y);
}

void GamePlayScene::setPlayerPosition(Vec2 &postion) {
//    Vec2 tileCoord = this->tileCoordForPosition(postion);
//    TMXLayer *meta = mTiledMap->getLayer("Meta");
//    int tileGid = meta->getTileGIDAt(tileCoord);
//    log("tileGid = %d", tileGid);
//    if (tileGid) {
//        Value value = mTiledMap->getPropertiesForGID(tileGid);
//        ValueMap dict = value.asValueMap();
//        if (dict["Collidable"].asBool()) {
//            log("true");
//            return;
//        }
//    }
    mPlayer->setPosition(postion);
}

void GamePlayScene::onUpdate(float dt) {
    Vec2 pos = mPlayer->getPosition();
    pos.x += 32;
    mPlayer->setPosition(pos);
}
