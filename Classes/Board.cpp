#include "Board.h"

Board::Board() {}

std::vector<std::string> makeVector() {
    std::vector<std::string> v;
    v.push_back("knight.png");
    v.push_back("thief.png");
    v.push_back("wizard.png");
    return v;
}

std::vector<std::string> Board::sFileNames = makeVector();

void Board::loadTiledMap(const std::string &filename) {
    mTiledMap = TMXTiledMap::create(filename);
    mBackgroundLayer = mTiledMap->getLayer(Constant::TILED_BACKGROUND_LAYER);

    mMetaLayer = mTiledMap->getLayer(Constant::TILED_META_LAYER);
    mMetaLayer->setVisible(false);

    mObjectLayer = mTiledMap->getLayer(Constant::TILED_OBJECT_LAYER);
}

void Board::initObject() {

}

void Board::update(std::pair<int, int> &point) {
    if (mBoardMap.find(point) != mBoardMap.end()) {
        AbsSprite *sprite = mBoardMap.at(point);

    } else {

    }
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
