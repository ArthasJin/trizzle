#ifndef _BOARD_H_
#define _BOARD_H_

#include "cocos2d.h"
#include "Constant.h"
#include "TrizzleSprite.h"
#include "Toast.h"

#include <time.h>

USING_NS_CC;
using namespace std;

template <class T>
inline void hash_combine(size_t & seed, const T & v) {
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

class OnMenuClickedListener {
public:
    virtual void onMenuClicked(int menu) = 0;
};

class Board : public Ref {
private:
    static vector<string> sFiles;
    static vector<string> sEnemyFiles;
    static unordered_map<string, Vector<SpriteFrame *> > sFrameDict;
    vector<vector<TrizzleSprite *> > mBoardMap;
    TMXTiledMap *mTiledMap;
    TMXLayer *mBackgroundLayer;
    TMXLayer *mMetaLayer;
    TMXLayer *mObjectLayer;
    TMXLayer *mCheckLayer;
    Layer* mBoardLayer;
    string mLevel;
    Sprite *mTapSprite;
    Sprite *mInstruction;
    Sprite *mUpArrow;

    bool mIsRunning;
    bool mIsPlayerTurn;
    OnMenuClickedListener *mMenuClickedListener;

    queue<TrizzleSprite *> mPlayerQueue;
    queue<TrizzleSprite *> mEnemyQueue;

    int mGameStatus;

    void initTiledMap(const string &filename);
    void initObject();
    Vec2 tileCoordForPosition(Vec2 position);
    Vec2 offsetForPosition(Vec2 position);
    bool canPut(Vec2 &coord);
    TrizzleSprite *getSprite(Vec2 coord);
    void setSprite(Vec2 &coord, TrizzleSprite *sprite);
    void update(float dt);
    void moveRight(TrizzleSprite *sprite);
    void moveLeft(TrizzleSprite *sprite);
    bool canMove(Vec2 &coord);
    void attack(TrizzleSprite *attacker, TrizzleSprite *defender);
    bool canAttack(TrizzleSprite *sprite, TrizzleSprite **defender);
    bool isFinished();
    void doPlayerAction();
    void doEnemyAction();
    void explode(TrizzleSprite *sprite);
    void defend(TrizzleSprite *sprite);
    void showMessage(const string msg);
    Vector<SpriteFrame *> getSpriteFrames(TrizzleSprite *sprite, int direction);
    void showMenu(bool shouldNext);
    void menuCallback(Ref *sender);
    void showToast(const string &message, float duration);
    bool canLose(Vec2 &coord);
public:
    Board(Layer *layer);
    void loadTiledMap(const string &filename);
    TrizzleSprite *getNext(TrizzleSprite *sprite);
    TMXTiledMap *getTiledMap();
    void onTouch(Touch *touch);
    void startPlay();
    void stopPlay();
    void setOnMenuClickedListener(OnMenuClickedListener *listener);
};

#endif
