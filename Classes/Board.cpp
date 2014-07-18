#include "Board.h"

Board::Board(Layer *layer) : mBoardLayer(layer),
                             mPlayerQueue(queue<TrizzleSprite *>()),
                             mEnemyQueue(queue<TrizzleSprite *>()),
                             mIsRunning(false),
                             mIsPlayerTurn(true) {}

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

vector<string> Board::sFiles = makeVector();

vector<string> Board::sEnemyFiles = makeEnemy();

void Board::loadTiledMap(const string &filename) {
    initTiledMap(filename);
    initObject();
}

void Board::initTiledMap(const string &filename) {
    mTiledMap = TMXTiledMap::create(filename);
    mTiledMap->setAnchorPoint(Vec2(0.5, 0.5));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    mTiledMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    mBackgroundLayer = mTiledMap->getLayer(Constant::TILED_BACKGROUND_LAYER);
    mMetaLayer = mTiledMap->getLayer(Constant::TILED_META_LAYER);
    mObjectLayer = mTiledMap->getLayer(Constant::TILED_OBJECT_LAYER);

    Size metaSize = mMetaLayer->getLayerSize();
    mBoardMap = vector<vector<TrizzleSprite *> >(metaSize.height, vector<TrizzleSprite *>(metaSize.width, NULL));
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
}

TMXTiledMap* Board::getTiledMap() {
    return mTiledMap;
}

void Board::onTouch(Touch *touch) {
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

TrizzleSprite *Board::getSprite(Vec2 coord) {
    return mBoardMap[coord.y][coord.x];
}

void Board::setSprite(Vec2 &coord, TrizzleSprite *sprite) {
    mBoardMap[coord.y][coord.x] = sprite;
}

TrizzleSprite *Board::getNext(TrizzleSprite *sprite) {
    TrizzleSprite *newSprite = NULL;
    if (sprite == NULL) {
        newSprite = TrizzleSprite::create(sFiles[0], 0, false);
    } else {
        int nextType = (sprite->getType() + 1) % sFiles.size();
        string filename = sFiles[nextType];
        mBoardLayer->removeChild(sprite);
        newSprite = TrizzleSprite::create(filename, nextType, false);
    }
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
        mIsRunning = true;
        Director::getInstance()->getScheduler()->schedule(schedule_selector(Board::update), this, 0.5, false);
//        update(0);
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
        showMessage("You Win!");
        stopPlay();
    }
    if (mPlayerQueue.empty() && !mEnemyQueue.empty()) {
        log("CPU wins!");
        showMessage("You Lose!");
        stopPlay();
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
    Vector<SpriteFrame*> animFrames(40);
    string str = "explosion.png";
    for(int i = 0; i < 40; i++) {
        auto frame = SpriteFrame::create(str, Rect(0 + i * 64, 0, 64, 64));
        animFrames.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.02f);
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
        Vec2 pos = sprite->getPosition();
        pos.x += 32;
        sprite->setPosition(pos);
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
        Vec2 pos = sprite->getPosition();
        pos.x -= 32;
        sprite->setPosition(pos);
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
    }
}

bool Board::isFinished() {
    if (mPlayerQueue.empty() || mEnemyQueue.empty()) {
        return true;
    }
    int row = mBoardMap.size();
    int col = mBoardMap[0].size();
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {

        }
    }
    return false;
}

void Board::showMessage(const string msg) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    LabelTTF *label = LabelTTF::create(msg, Constant::FONT, 120, CCSizeMake(360, 280), kCCTextAlignmentCenter);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    mBoardLayer->addChild(label);
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
