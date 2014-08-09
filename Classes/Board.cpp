#include "Board.h"

Board::Board(Layer *layer) : mBoardLayer(layer),
                             mPlayerQueue(queue<TrizzleSprite *>()),
                             mEnemyQueue(queue<TrizzleSprite *>()),
                             mIsRunning(false),
                             mIsPlayerTurn(true),
                             mGameStatus(Constant::GAME_STATUS_UNKNOWN) {}

vector<string> makeVector() {
    vector<string> v;
    v.push_back("thief.png");
    v.push_back("wizard.png");
    v.push_back("knight.png");
    return v;
}

vector<string> makeEnemy() {
    vector<string> v;
    v.push_back("thief_enemy.png");
    v.push_back("wizard_enemy.png");
    v.push_back("knight_enemy.png");
    return v;
}

Vector<SpriteFrame *> createFrames(const string &filename, int n) {
    Vector<SpriteFrame*> animFrames(n);
    for(int i = 0; i < n; i++) {
        auto frame = SpriteFrame::create(filename, Rect(0 + i * 64, 0, 64, 64));
        animFrames.pushBack(frame);
    }
    return animFrames;
}

unordered_map<string, Vector<SpriteFrame *> > makeFrameDict() {
    unordered_map<string, Vector<SpriteFrame *> > dict;
    Vector<SpriteFrame *> explodeFrames = createFrames("explosion.png", 40);
    Vector<SpriteFrame *> wizardLeftFrames = createFrames("wizard_walk_left.png", 9);
    Vector<SpriteFrame *> wizardRightFrames = createFrames("wizard_walk_right.png", 9);

    Vector<SpriteFrame *> knightLeftFrames = createFrames("knight_walk_left.png", 8);
    Vector<SpriteFrame *> knightRightFrames = createFrames("knight_walk_right.png", 8);

    Vector<SpriteFrame *> thiefLeftFrames = createFrames("thief_walk_left.png", 9);
    Vector<SpriteFrame *> thiefRightFrames = createFrames("thief_walk_right.png", 9);

    dict["explosion"] = explodeFrames;
    dict["wizard_walk_left"] = wizardLeftFrames;
    dict["wizard_walk_right"] = wizardRightFrames;
    dict["knight_walk_left"] = knightLeftFrames;
    dict["knight_walk_right"] = knightRightFrames;
    dict["thief_walk_left"] = thiefLeftFrames;
    dict["thief_walk_right"] = thiefRightFrames;
    return dict;
}

vector<string> Board::sFiles = makeVector();

vector<string> Board::sEnemyFiles = makeEnemy();

unordered_map<string, Vector<SpriteFrame *> > Board::sFrameDict = makeFrameDict();

void Board::loadTiledMap(const string &filename) {
    initTiledMap(filename);
    initObject();
}

void Board::initTiledMap(const string &filename) {
    mBoardLayer->removeAllChildren();

    mBoardMap.clear();
    mIsRunning = false;
    mIsPlayerTurn = true;
    mGameStatus = Constant::GAME_STATUS_UNKNOWN;

    mPlayerQueue = queue<TrizzleSprite *>();
    mEnemyQueue = queue<TrizzleSprite *>();

    mInstruction = Sprite::create("instruction.png");
    mUpArrow = Sprite::create("up.png");
    mUpArrow->setScale(0.25);
    mUpArrow->setPosition(480, 580);
    mUpArrow->setVisible(false);
    mInstruction->setScale(0.3);
    mInstruction->setPosition(480, 580);
    mBoardLayer->addChild(mInstruction, 999);
    mBoardLayer->addChild(mUpArrow, 999);

    mLevel = filename;
    mTiledMap = TMXTiledMap::create(filename);
    mTiledMap->setAnchorPoint(Vec2(0.5, 0.5));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    mTiledMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    mBackgroundLayer = mTiledMap->getLayer(Constant::TILED_BACKGROUND_LAYER);
    mMetaLayer = mTiledMap->getLayer(Constant::TILED_META_LAYER);
    mObjectLayer = mTiledMap->getLayer(Constant::TILED_OBJECT_LAYER);
    mCheckLayer = mTiledMap->getLayer(Constant::TILED_CHECK_LAYER);

    Size metaSize = mMetaLayer->getLayerSize();
    mBoardMap = vector<vector<TrizzleSprite *> >(metaSize.height, vector<TrizzleSprite *>(metaSize.width, NULL));
    mBoardLayer->addChild(mTiledMap);
}

void Board::initObject() {
    TMXObjectGroup *group = mTiledMap->getObjectGroup("object");
    ValueVector points = group->getObjects();
    for (ValueVector::iterator iter = points.begin(); iter != points.end(); ++iter) {
        Value v = *iter;
        ValueMap spawnPoint = v.asValueMap();
        int x = spawnPoint["x"].asInt();
        int y = spawnPoint["y"].asInt();
        int role = spawnPoint["role"].asInt();
        log("x = %d y = %d role = %d", x, y, role);

        TrizzleSprite *s = TrizzleSprite::create(sEnemyFiles[role], role, true);
        Vec2 offsetPos = offsetForPosition(Vec2(x, y));
        Vec2 coord = tileCoordForPosition(offsetPos);

        setSprite(coord, s);
        s->setAnchorPoint(Vec2::ZERO);
        s->setPosition(offsetPos);

        this->mBoardLayer->addChild(s, 999);
    }
    TMXObjectGroup *tap = mTiledMap->getObjectGroup("tap");
    if (tap) {
        ValueVector objects = tap->getObjects();
        for (ValueVector::iterator iter = objects.begin(); iter != objects.end(); ++iter) {
            Value v = *iter;
            ValueMap tapPoint = v.asValueMap();
            int x = tapPoint["x"].asInt();
            int y = tapPoint["y"].asInt();
            log("x = %d, y = %d", x, y);
            mTapSprite = Sprite::create("tap.png");
            Vec2 pos = offsetForPosition(Vec2(x, y));
            log("pos.x = %d, pos.y = %d", pos.x, pos.y);
            Vec2 coord = tileCoordForPosition(pos);
            log("coord.x = %d, coord.y = %d", coord.x, coord.y);
            mTapSprite->setAnchorPoint(Vec2::ZERO);
            mTapSprite->setPosition(Vec2(x - 32, y));
            this->mBoardLayer->addChild(mTapSprite);
        }
    }
}

TMXTiledMap* Board::getTiledMap() {
    return mTiledMap;
}

void Board::onTouch(Touch *touch) {
    if (!mIsRunning) {
        Vec2 point = touch->getLocationInView();
        point = Director::getInstance()->convertToGL(point);
        point = mBoardLayer->convertToNodeSpace(point);
        Vec2 coord = this->tileCoordForPosition(point);
        if (canPut(coord)) {
            TrizzleSprite *sprite = getNext(getSprite(coord));
            setSprite(coord, sprite);
            point = this->offsetForPosition(point);
            sprite->setAnchorPoint(Vec2::ZERO);
            sprite->setPosition(point);
            mBoardLayer->addChild(sprite);
        }
    }
}

TrizzleSprite *Board::getSprite(Vec2 coord) {
    return mBoardMap[coord.y][coord.x];
}

void Board::setSprite(Vec2 &coord, TrizzleSprite *sprite) {
    mBoardMap[coord.y][coord.x] = sprite;
}

TrizzleSprite *Board::getNext(TrizzleSprite *sprite) {
    TrizzleSprite *newSprite = NULL;
    int type = 0;
    if (sprite == NULL) {
        newSprite = TrizzleSprite::create(sFiles[0], 0, false);
        type = 1;
    } else {
        int nextType = (sprite->getType() + 1) % sFiles.size();
        string filename = sFiles[nextType];
        mBoardLayer->removeChild(sprite);
        newSprite = TrizzleSprite::create(filename, nextType, false);
        type = (nextType + 1) % sFiles.size();
    }
    mUpArrow->setVisible(true);
    mUpArrow->setPosition((type) * 80 + 360, 500);
    Action *action = Blink::create(1, 2);
    mUpArrow->runAction(action);
    return newSprite;
}

bool Board::canPut(Vec2 &coord) {
    int tileGid = mMetaLayer->getTileGIDAt(coord);
    Value properties = mTiledMap->getPropertiesForGID(tileGid);
    if (properties.getType() == Value::Type::MAP) {
        return properties.asValueMap()["put"].asBool();
    }
    return false;
}

void Board::startPlay() {
    if (!mIsRunning) {
        int row = mBoardMap.size();
        int col = mBoardMap[0].size();
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (mBoardMap[i][j]) {
                    TrizzleSprite *sprite = mBoardMap[i][j];
                    if (sprite->isEmeny()) {
                        log("enemy row = %d, col = %d", i, j);
                        mEnemyQueue.push(sprite);
                    }
                }
            }
            for (int j = col - 1; j >= 0; --j) {
                if (mBoardMap[i][j]) {
                    TrizzleSprite *sprite = mBoardMap[i][j];
                    if (!sprite->isEmeny()) {
                        log("player row = %d, col = %d", i, j);
                        mPlayerQueue.push(sprite);
                    }
                }
            }
        }
        if (mPlayerQueue.empty()) {
            showToast("Please put your characters!", 2.0f);
            return;
        } else {
            if (mGameStatus == Constant::GAME_STATUS_UNKNOWN) {
                if (mTapSprite != NULL) {
                    mTapSprite->removeFromParentAndCleanup(true);
                    mTapSprite = NULL;
                }
                if (mUpArrow != NULL) {
                    mUpArrow->setVisible(false);
                }
                mIsRunning = true;
                Director::getInstance()->getScheduler()->schedule(schedule_selector(Board::update), this, 0.5, false);
        //        update(0);
            }
        }
    } else {
        showToast("The game is already started!", 2.0f);
    }
}

void Board::update(float dt) {
    log("Board::update");
    if (!isFinished() && mIsRunning) {
        if (mIsPlayerTurn) {
            doPlayerAction();
        } else {
            doEnemyAction();
        }
        mIsPlayerTurn = !mIsPlayerTurn;
    }
    if (!mPlayerQueue.empty() && mEnemyQueue.empty()) {
        log("player wins!");
        mGameStatus = Constant::GAME_STATUS_WIN;
        stopPlay();
        if (mLevel[5] == '6') {
            showMessage("Clear!");
        } else {
            showMessage("You Win!");
            showMenu(true);
        }
        return;
    }
    if (!mEnemyQueue.empty()) {
        Vec2 vec2 = tileCoordForPosition(mEnemyQueue.back()->getPosition());
        vec2.x -= 1;
        vec2.y -= 1;

        if (mPlayerQueue.empty() && !mEnemyQueue.empty() || canLose(vec2)) {
            log("CPU wins!");
            mGameStatus = Constant::GAME_STATUS_LOSE;
            stopPlay();
            showMessage("You Lose!");
            showMenu(false);
            return;
        }
    }
}

void Board::doPlayerAction() {
    if (!mPlayerQueue.empty()) {
        log("playerAction");
        TrizzleSprite *sprite = mPlayerQueue.front();
        mPlayerQueue.pop();
        if (sprite->isAlive()) {
            TrizzleSprite *defend = NULL;
            if (canAttack(sprite, &defend)) {
                attack(sprite, defend);
            } else {
                moveRight(sprite);
            }
            mPlayerQueue.push(sprite);
        }
    }
}

void Board::doEnemyAction() {
    if (!mEnemyQueue.empty()) {
        log("enemyAction");
        TrizzleSprite *sprite = mEnemyQueue.front();
        mEnemyQueue.pop();
        if (sprite->isAlive()) {
            TrizzleSprite *defend = NULL;
            if (canAttack(sprite, &defend)) {
                attack(sprite, defend);
            } else {
                moveLeft(sprite);
            }
            mEnemyQueue.push(sprite);
        }
    }
}

void Board::explode(TrizzleSprite *sprite) {
    auto animation = Animation::createWithSpriteFrames(Board::sFrameDict["explosion"], 0.02f);
    Animate *animate = Animate::create(animation);
    sprite->runAction(animate);
}

void Board::defend(TrizzleSprite *sprite) {
    Vector<SpriteFrame*> animFrames(40);
    string str = "shield.png";
    for(int i = 0; i < 40; i++) {
        auto frame = SpriteFrame::create(str, Rect(0, 0, 64, 64));
        animFrames.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.02f);
    auto animate = Animate::create(animation);
    sprite->runAction(animate);
}

void Board::moveRight(TrizzleSprite *sprite) {
    Vec2 coord = tileCoordForPosition(sprite->getPosition());
    log("moveRight coord x = %f y = %f", coord.x, coord.y);
    coord.y -= 1;
    coord.x += 1;

    if (canMove(coord)) {
        auto animation = Animation::createWithSpriteFrames(getSpriteFrames(sprite, Constant::DIRECT_RIGHT), 0.1f);
        auto animate = Animate::create(animation);
        Vec2 delta(64, 0);
        MoveBy *moveBy = MoveBy::create(0.5f, delta);

        sprite->runAction(moveBy);
        sprite->runAction(animate);
//        Vec2 pos = sprite->getPosition();
//        pos.x += 64;
//        sprite->setPosition(pos);
        Vec2 prevCoord = Vec2(coord.x - 1, coord.y);
        setSprite(prevCoord, NULL);
        setSprite(coord, sprite);
    }
}

void Board::moveLeft(TrizzleSprite *sprite) {
    Vec2 coord = tileCoordForPosition(sprite->getPosition());
    coord.x -= 1;
    coord.y -= 1;
    if (canMove(coord)) {
        auto animation = Animation::createWithSpriteFrames(getSpriteFrames(sprite, Constant::DIRECT_LEFT), 0.1f);
        auto animate = Animate::create(animation);
        Vec2 delta(-64, 0);
        MoveBy *moveBy = MoveBy::create(0.5f, delta);

        sprite->runAction(moveBy);
        sprite->runAction(animate);
//        Vec2 pos = sprite->getPosition();
//        pos.x -= 64;
//        sprite->setPosition(pos);
        Vec2 prevCoord = Vec2(coord.x + 1, coord.y);
        setSprite(prevCoord, NULL);
        setSprite(coord, sprite);
    }
}

bool Board::canMove(Vec2 &coord) {
    log("canMove coord x = %f y = %f", coord.x, coord.y);
    int tileGid = mMetaLayer->getTileGIDAt(coord);
    Value properties = mTiledMap->getPropertiesForGID(tileGid);
    if (properties.getType() == Value::Type::MAP) {
        log("can move = %d", properties.asValueMap()["move"].asBool());
        return properties.asValueMap()["move"].asBool();
    }
    return false;
}

bool Board::canAttack(TrizzleSprite *sprite, TrizzleSprite **defender) {
    int row = mBoardMap.size();
    int col = mBoardMap[0].size();
    Vec2 pos = sprite->getPosition();
    Vec2 coord = tileCoordForPosition(pos);
    coord.y -= 1;
    log("canAttack coord x = %f y = %f", coord.x, coord.y);
    if (coord.y > 0) {
        TrizzleSprite *sp = getSprite(Vec2(coord.x, coord.y - 1));
        if (sp != NULL && sp->isAlive() && sp->isEmeny() != sprite->isEmeny()) {
            log("can attack up");
            *defender = sp;
            return true;
        }
    }
    if (coord.x < col) {
        TrizzleSprite *sp = getSprite(Vec2(coord.x + 1, coord.y));
        if (sp != NULL && sp->isAlive() && sp->isEmeny() != sprite->isEmeny()) {
            log("can attack right");
            *defender = sp;
            return true;
        }
    }
    if (coord.y < row - 1) {
        TrizzleSprite *sp = getSprite(Vec2(coord.x, coord.y + 1));
        if (sp != NULL && sp->isAlive() && sp->isEmeny() != sprite->isEmeny()) {
            log("can attack down");
            *defender = sp;
            return true;
        }
    }
    if (coord.x > 0) {
        TrizzleSprite *sp = getSprite(Vec2(coord.x - 1, coord.y));
        if (sp != NULL && sp->isAlive() && sp->isEmeny() != sprite->isEmeny()) {
            log("can attack left");
            *defender = sp;
            return true;
        }
    }
    return false;
}

void Board::attack(TrizzleSprite *attacker, TrizzleSprite *defender) {
    log("Board::attack");
    if (attacker->getType() == 0 && defender->getType() == 1 ||
            attacker->getType() == 1 && defender->getType() == 2 ||
            attacker->getType() == 2 && defender->getType() == 0) {
        explode(defender);
        defender->setAlive(false);
        Vec2 coord = tileCoordForPosition(defender->getPosition());
        coord.y -= 1;
        setSprite(coord, NULL);
    } else if (attacker->getType() == defender->getType()) {
        explode(defender);
        explode(attacker);
        defender->setAlive(false);
        attacker->setAlive(false);

        Vec2 coord = tileCoordForPosition(defender->getPosition());
        coord.y -= 1;
        setSprite(coord, NULL);

        coord = tileCoordForPosition(attacker->getPosition());
        coord.y -= 1;
        setSprite(coord, NULL);
    }
}

bool Board::isFinished() {
    if (mPlayerQueue.empty() || mEnemyQueue.empty()) {
        return true;
    }
    return false;
}

void Board::showMessage(const string msg) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    LabelTTF *label = LabelTTF::create(msg, Constant::FONT, 120, CCSizeMake(360, 280), kCCTextAlignmentCenter);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));
    mBoardLayer->addChild(label, 999);
}

void Board::showMenu(bool shouldNext) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Menu *menu = NULL;
    if (!shouldNext) {
        MenuItemFont *menuTitle = MenuItemFont::create(Constant::MENU_PLAY, CC_CALLBACK_1(Board::menuCallback, this));
        menuTitle->setFontName(Constant::FONT);
        menuTitle->setFontSize(64);
        menuTitle->setVisible(false);

        MenuItemFont *retry = MenuItemFont::create(Constant::MENU_RETRY, CC_CALLBACK_1(Board::menuCallback, this));
        retry->setFontName(Constant::FONT);
        retry->setFontSize(64);
        retry->setTag(Constant::MENU_RETRY_TAG);

        menu = Menu::create(retry, NULL);

        menu->setAnchorPoint(Vec2(0.5, 0.5));
        menu->alignItemsHorizontallyWithPadding(20.0);
        menu->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100));
    } else {
        MenuItemFont *menuTitle = MenuItemFont::create(Constant::MENU_PLAY, CC_CALLBACK_1(Board::menuCallback, this));
        menuTitle->setFontName(Constant::FONT);
        menuTitle->setFontSize(64);
        menuTitle->setVisible(false);

        MenuItemFont *retry = MenuItemFont::create(Constant::MENU_RETRY, CC_CALLBACK_1(Board::menuCallback, this));
        retry->setFontName(Constant::FONT);
        retry->setFontSize(64);
        retry->setTag(Constant::MENU_RETRY_TAG);

        MenuItemFont *next = MenuItemFont::create(Constant::MENU_NEXT, CC_CALLBACK_1(Board::menuCallback, this));
        next->setTag(Constant::MENU_NEXT_TAG);
        next->setFontSize(64);
        next->setFontName(Constant::FONT);

        menu = Menu::create(menuTitle, retry, next, NULL);

        menu->setAnchorPoint(Vec2(0.5, 0.5));
        menu->alignItemsHorizontallyWithPadding(20.0);
        menu->setPosition(Vec2(visibleSize.width / 2 + origin.x - 80, visibleSize.height / 2 + origin.y - 100));
    }

    mBoardLayer->addChild(menu, 999);
}

void Board::menuCallback(Ref *sender) {
    MenuItem *item = (MenuItem *) sender;
    mMenuClickedListener->onMenuClicked(item->getTag());
}

void Board::stopPlay() {
    mIsRunning = false;
    Director::sharedDirector()->getScheduler()->unschedule(schedule_selector(Board::update), this);
}

Vec2 Board::offsetForPosition(Vec2 position) {
    int x = (int) (position.x / mTiledMap->getTileSize().width) * mTiledMap->getTileSize().width;
    int y = (int) (position.y / mTiledMap->getTileSize().height) * mTiledMap->getTileSize().height;
    return Vec2(x, y);
}

Vec2 Board::tileCoordForPosition(Vec2 position) {
    int x = (position.x) / mTiledMap->getTileSize().width;
    int y = ((mTiledMap->getMapSize().height * mTiledMap->getTileSize().height) - (position.y)) / mTiledMap->getTileSize().height;
    return Vec2(x, y);
}

Vector<SpriteFrame *> Board::getSpriteFrames(TrizzleSprite *sprite, int direction) {
    if (sprite) {
        if (sprite->getType() == Constant::TYPE_KNIGHT) {
            if (direction == Constant::DIRECT_LEFT) {
                return sFrameDict[Constant::KNIGHT_LEFT];
            } else if (direction == Constant::DIRECT_RIGHT) {
                return sFrameDict[Constant::KNIGHT_RIGHT];
            }
        } else if (sprite->getType() == Constant::TYPE_THIEF) {
            if (direction == Constant::DIRECT_LEFT) {
                return sFrameDict[Constant::THIEF_LEFT];
            } else if (direction == Constant::DIRECT_RIGHT) {
                return sFrameDict[Constant::THIEF_RIGHT];
            }
        } else if (sprite->getType() == Constant::TYPE_WIZARD) {
            if (direction == Constant::DIRECT_LEFT) {
                return sFrameDict[Constant::WIZARD_LEFT];
            } else if (direction == Constant::DIRECT_RIGHT) {
                return sFrameDict[Constant::WIZARD_RIGHT];
            }
        }
    }
    return Vector<SpriteFrame *>();
}

void Board::setOnMenuClickedListener(OnMenuClickedListener *listener) {
    mMenuClickedListener = listener;
}

void Board::showToast(const string &message, float duration) {
    Toast::showToast(this->mBoardLayer, message, duration);
}

bool Board::canLose(Vec2 &coord) {
    if (mCheckLayer) {
        int tileGid = mCheckLayer->getTileGIDAt(coord);
        Value properties = mTiledMap->getPropertiesForGID(tileGid);
        if (properties.getType() == Value::Type::MAP) {
            log("can lose = %d", properties.asValueMap()["lose"].asBool());
            return properties.asValueMap()["lose"].asBool();
        }
    }
    return false;
}
