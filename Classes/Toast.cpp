#include "Toast.h"

void Toast::showToast(Node *node, const string &msg, float dt) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Scale9Sprite* bg = Scale9Sprite::create("toast_bg.png");
    bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 130 * visibleSize.height / 960));
    LabelTTF* pLabel = LabelTTF::create(msg.c_str(), "Arial", 21);
    pLabel->setColor(ccWHITE);
    bg->setContentSize(CCSizeMake(pLabel->getContentSize().width + 10, pLabel->getContentSize().height + 10));
    bg->addChild(pLabel, 1);
    node->addChild(bg, 10);
    pLabel->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
    pLabel->runAction(Sequence::create(FadeIn::create(dt / 5), DelayTime::create(dt / 5 * 3), FadeOut::create(dt / 5),NULL));
    bg->runAction(Sequence::create(FadeIn::create(dt / 5), DelayTime::create(dt / 5 * 3), FadeOut::create(dt / 5),
                                     CallFuncN::create(bg, callfuncN_selector(Toast::removeToast)), NULL));
}

void Toast::removeToast(Node *node) {
    node->removeFromParentAndCleanup(true);
}
