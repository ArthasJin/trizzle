#ifndef _BOARD_H_
#define _BOARD_H_

#include "cocos2d.h"
#include "Constant.h"
#include "AbsSprite.h"
#include "KnightSprite.h"
#include "ThiefSprite.h"
#include "WizardSprite.h"

USING_NS_CC;

using namespace std;

template <class T>
inline void hash_combine(std::size_t & seed, const T & v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
    template<typename S, typename T>
    struct hash<pair<S, T> > {
        inline size_t operator()(const pair<S, T> & v) const {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
}

class Board {
private:
    static std::vector<std::string> sFileNames;
    vector<vector<Sprite *> > mBoardMap;
    TMXTiledMap *mTiledMap;
    TMXLayer *mBackgroundLayer;
    TMXLayer *mMetaLayer;
    TMXLayer *mObjectLayer;
    Layer* mBoardLayer;

    void initTiledMap(const string &filename);
    void initObject();
    Vec2 tileCoordForPosition(Vec2 position);
    Vec2 offsetForPosition(Vec2 position);
    bool canPut(Vec2 &coord);
public:
    Board(Layer *layer);
    void loadTiledMap(const std::string &filename);
    void update(std::pair<int, int> &point);
    AbsSprite *next(AbsSprite *sprite);
    TMXTiledMap *getTiledMap();
    void onTouch(Touch *touch);
};

#endif
