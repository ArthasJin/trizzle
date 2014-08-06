#ifndef _TOAST_H_
#define _TOAST_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class Toast : public Ref {
public:
    static void showToast(Node* node, const string &msg, float dt);
    void removeToast(Node *node);
};

#endif
