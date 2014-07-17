#include "MenuScene.h"

Scene* MenuScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MenuScene::init() {
    if (!Layer::init()) {
        return false;
    }
    initBackground();
    initMenuItem();
    return true;
}

void MenuScene::initBackground() {
    Size visibleSize = Director::sharedDirector()->getVisibleSize();
    Vec2 origin = Director::sharedDirector()->getVisibleOrigin();
    Sprite *background = Sprite::create("background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(background);
}

void MenuScene::initMenuItem() {
    log("initMenuItem");
    MenuItemFont *menuTitle = MenuItemFont::create(Constant::MENU_PLAY, CC_CALLBACK_1(MenuScene::menuCallback, this));
    menuTitle->setFontName(Constant::FONT);
    menuTitle->setFontSize(100);
    menuTitle->setVisible(false);

    MenuItemFont *play = MenuItemFont::create(Constant::MENU_PLAY, CC_CALLBACK_1(MenuScene::menuCallback, this));
    play->setFontName(Constant::FONT);
    play->setFontSize(100);
    play->setTag(Constant::MENU_PLAY_TAG);

    MenuItemFont *settings = MenuItemFont::create(Constant::MENU_SETTINGS, CC_CALLBACK_1(MenuScene::menuCallback, this));
    settings->setTag(Constant::MENU_SETTINGS_TAG);
    settings->setFontSize(100);
    settings->setFontName(Constant::FONT);

    MenuItemFont *exit = MenuItemFont::create(Constant::MENU_EXIT, CC_CALLBACK_1(MenuScene::menuCallback, this));
    exit->setTag(Constant::MENU_EXIT_TAG);
    exit->setFontSize(100);
    exit->setFontName(Constant::FONT);

    Menu *menu = Menu::create(play, settings, exit, NULL);
    menu->alignItemsVerticallyWithPadding(20.0);

    Size visibleSize = Director::sharedDirector()->getVisibleSize();
    Vec2 origin = Director::sharedDirector()->getVisibleOrigin();
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(menu);
}

void MenuScene::menuCallback(Ref *sender) {
    log("menuCallback");
    MenuItem *item = (MenuItem*) sender;
    int tag = item->getTag();
    if (tag == Constant::MENU_PLAY_TAG) {
        play();
    } else if (tag == Constant::MENU_SETTINGS_TAG) {
        settings();
    } else if (tag == Constant::MENU_EXIT_TAG) {
        exit();
    }
}

void MenuScene::play() {
    log("play");
    Scene *gamePlayScene = GamePlayScene::createScene();
    auto transition = TransitionFade::create(1.0f, gamePlayScene);
    Director::sharedDirector()->replaceScene(transition);
}

void MenuScene::settings() {
    log("settings");
}

void MenuScene::exit() {
    log("exit");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
