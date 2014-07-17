#ifndef _CONSTANT_H_
#define _CONSTANT_H_

class Constant {
public:
    static const char* GAME_NAME;
    static const char* FONT;
    static const double FRAME_RATE;

    static const int TYPE_INVALID;
    static const int TYPE_THIEF;
    static const int TYPE_WIZARD;
    static const int TYPE_KNIGHT;

    static const char* MENU_PLAY;
    static const char* MENU_SETTINGS;
    static const char* MENU_EXIT;

    static const int MENU_PLAY_TAG;
    static const int MENU_SETTINGS_TAG;
    static const int MENU_EXIT_TAG;

    static const int MENU_BACK_TAG;
    static const int MENU_START_TAG;

    static const char* TILED_BACKGROUND_LAYER;
    static const char* TILED_OBJECT_LAYER;
    static const char* TILED_META_LAYER;
};

#endif
