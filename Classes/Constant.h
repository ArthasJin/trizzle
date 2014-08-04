#ifndef _CONSTANT_H_
#define _CONSTANT_H_

class Constant {
public:
    static const char* const GAME_NAME;
    static const char* const FONT;
    static const double FRAME_RATE;

    static const int TYPE_THIEF;
    static const int TYPE_WIZARD;
    static const int TYPE_KNIGHT;

    static const char* const MENU_PLAY;
    static const char* const MENU_SETTINGS;
    static const char* const MENU_EXIT;

    static const char* const MENU_START;
    static const char* const MENU_BACK;

    static const char* const MENU_RETRY;
    static const char* const MENU_NEXT;

    static const int MENU_PLAY_TAG;
    static const int MENU_SETTINGS_TAG;
    static const int MENU_EXIT_TAG;

    static const int MENU_BACK_TAG;
    static const int MENU_START_TAG;

    static const int MENU_RETRY_TAG;
    static const int MENU_NEXT_TAG;

    static const char* const TILED_BACKGROUND_LAYER;
    static const char* const TILED_OBJECT_LAYER;
    static const char* const TILED_META_LAYER;

    static const int DIRECT_LEFT;
    static const int DIRECT_RIGHT;

    static const char* const EXPLOSION;
    static const char* const WIZARD_LEFT;
    static const char* const WIZARD_RIGHT;
    static const char* const KNIGHT_LEFT;
    static const char* const KNIGHT_RIGHT;
    static const char* const THIEF_LEFT;
    static const char* const THIEF_RIGHT;
};

#endif
