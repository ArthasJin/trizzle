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
    Size visibleSize = Director::sharedDirector()->getVisibleSize();
    Vec2 origin = Director::sharedDirector()->getVisibleOrigin();
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
    point = Director::sharedDirector()->convertToGL(point);
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

TrizzleSprite *Board::getSprite(Vec2 &coord) {
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
    Director::sharedDirector()->getScheduler()->schedule(schedule_selector(Board::update), this, 1.0, false);
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
}

void Board::doPlayerAction() {
    if (!mPlayerQueue.empty()) {
        log("playerAction");
        TrizzleSprite *sprite = mPlayerQueue.front();
        mPlayerQueue.pop();
        moveRight(sprite);
        mPlayerQueue.push(sprite);
    }
}

void Board::doEnemyAction() {
    if (!mEnemyQueue.empty()) {
        log("enemyAction");
        TrizzleSprite *sprite = mEnemyQueue.front();
        mEnemyQueue.pop();
        moveLeft(sprite);
        mEnemyQueue.push(sprite);
    }
}

void Board::moveRight(TrizzleSprite *sprite) {
    Vec2 coord = tileCoordForPosition(sprite->getPosition());
    coord.x -= 1;
    if (canMove(coord)) {
        Vec2 pos = sprite->getPosition();
        pos.x += 32;
        sprite->setPosition(pos);
    }
}

void Board::moveLeft(TrizzleSprite *sprite) {
    Vec2 coord = tileCoordForPosition(sprite->getPosition());
    coord.x += 1;
    if (canMove(coord)) {
        Vec2 pos = sprite->getPosition();
        pos.x -= 32;
        sprite->setPosition(pos);
    }
}

bool Board::canMove(Vec2 &coord) {
    return true;
}

bool Board::canAttack(TrizzleSprite *sprite) {
    return false;
}

void Board::attack(TrizzleSprite *attacker, TrizzleSprite *defender) {

}

bool Board::isFinished() {
    return false;
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
    int x = position.x / mTiledMap->getTileSize().width;
    int y = ((mTiledMap->getMapSize().height * mTiledMap->getTileSize().height) - position.y) / mTiledMap->getTileSize().height;
    return Vec2(x, y);
}
