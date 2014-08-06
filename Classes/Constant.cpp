#include "Constant.h"

const char* const Constant::GAME_NAME = "Trizzle";
const char* const Constant::FONT = "Marker Felt.ttf";
const double Constant::FRAME_RATE = 1.0 / 60.0;

const int Constant::TYPE_THIEF = 0;
const int Constant::TYPE_WIZARD = 1;
const int Constant::TYPE_KNIGHT = 2;

const char* const Constant::MENU_PLAY = "Play";
const char* const Constant::MENU_SETTINGS = "Settings";
const char* const Constant::MENU_EXIT = "Exit";

const char* const Constant::MENU_RETRY = "Retry";
const char* const Constant::MENU_NEXT = "Next";

const char* const Constant::MENU_START = "Start";
const char* const Constant::MENU_BACK = "Back";

const int Constant::MENU_PLAY_TAG = 0;
const int Constant::MENU_SETTINGS_TAG = 1;
const int Constant::MENU_EXIT_TAG = 2;

const int Constant::MENU_BACK_TAG = 3;
const int Constant::MENU_START_TAG = 4;

const int Constant::MENU_RETRY_TAG = 5;
const int Constant::MENU_NEXT_TAG = 6;

const char* const Constant::TILED_BACKGROUND_LAYER = "background";
const char* const Constant::TILED_OBJECT_LAYER = "object";
const char* const Constant::TILED_META_LAYER = "meta";
const char* const Constant::TILED_CHECK_LAYER = "check";

const int Constant::DIRECT_LEFT = 0;
const int Constant::DIRECT_RIGHT = 1;

const char* const Constant::EXPLOSION = "explosion";
const char* const Constant::WIZARD_LEFT = "wizard_walk_left";
const char* const Constant::WIZARD_RIGHT = "wizard_walk_right";
const char* const Constant::KNIGHT_LEFT = "knight_walk_left";
const char* const Constant::KNIGHT_RIGHT = "knight_walk_right";
const char* const Constant::THIEF_LEFT = "thief_walk_left";
const char* const Constant::THIEF_RIGHT = "thief_walk_right";

const int Constant::GAME_STATUS_UNKNOWN = -1;
const int Constant::GAME_STATUS_LOSE = 0;
const int Constant::GAME_STATUS_WIN = 1;
