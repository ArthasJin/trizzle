#include "Board.h"

Board::Board(Layer *layer) : mBoardLayer(layer) {}

std::vector<std::string> makeVector() {
    std::vector<std::string> v;
    v.push_back("knight.png");
    v.push_back("thief.png");
    v.push_back("wizard.png");
    return v;
}

std::vector<std::string> Board::sFileNames = makeVector();

void Board::loadTiledMap(const std::string &filename) {
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

    Size metaSize = mMetaLayer->getContentSize();
    mBoardMap = vector<vector<Sprite *> >(metaSize.height, vector<Sprite *>(metaSize.width, NULL));
}

void Board::initObject() {
    TMXObjectGroup *group = mTiledMap->getObjectGroup("object");
    ValueMap spawnPoint = group->getObject("SpawnPoint");
    int x = spawnPoint["x"].asInt();
    int y = spawnPoint["y"].asInt();
    int role = spawnPoint["role"].asInt();
    if (role == Constant::TYPE_THIEF) {
        log("x = %d y = %d role = %d", x, y, role);
        Sprite *s = Sprite::create("thief_enemy.png");
        s->setAnchorPoint(Vec2::ZERO);
        s->setPosition(this->offsetForPosition(Vec2(x, y)));
        this->mBoardLayer->addChild(s, 999);
    } else if (role == Constant::TYPE_WIZARD) {

    } else if (role == Constant::TYPE_KNIGHT) {

    }

}

TMXTiledMap* Board::getTiledMap() {
    return mTiledMap;
}

void Board::onTouch(Touch *touch) {
    Vec2 point = touch->getLocationInView();
    point = Director::sharedDirector()->convertToGL(point);
    point = mBoardLayer->convertToNodeSpace(point);
    Sprite *s = Sprite::create("wizard.png");

    Vec2 coord = this->tileCoordForPosition(point);
    if (canPut(coord)) {
        log("can put");
        point = this->offsetForPosition(point);
        s->setAnchorPoint(Vec2::ZERO);
        s->setPosition(point);
        mBoardLayer->addChild(s);
    }
}

void Board::update(std::pair<int, int> &point) {

}

AbsSprite* Board::next(AbsSprite *sprite) {
    AbsSprite *newSprite = NULL;
    std::string fileName = sFileNames[(sprite->getType() + 1) % sFileNames.size()];
    // log("fileName = %s", fileName);
//    if (sprite->getType() == Constant::TYPE_KNIGHT) {
//        newSprite = ThiefSprite::create(fileName);
//    } else if (sprite->getType() == Constant::TYPE_THIEF) {
//        newSprite = WizardSprite::create(fileName);
//    } else if (sprite->getType() == Constant::TYPE_WIZARD) {
//        newSprite = KnightSprite::create(fileName);
//    }
    return newSprite;
}

bool Board::canPut(Vec2 &coord) {
    int tileGid = mMetaLayer->getTileGIDAt(coord);
    Value properties = mTiledMap->getPropertiesForGID(tileGid);
    log("type = %d", properties.getType());
    if (properties.getType() == Value::Type::MAP) {
        return properties.asValueMap()["put"].asBool();
    }
    return false;
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
