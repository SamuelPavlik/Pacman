#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

//window size
inline const int WINDOW_WIDTH = 1024;
inline const int WINDOW_HEIGHT = 768;

//menu properties
inline const std::string START_TITLE = "Play";
inline const std::string QUIT_TITLE = "Quit";
inline const int START_X = WINDOW_WIDTH / 2 - START_TITLE.size() / 2 - 28;
inline const int START_Y = WINDOW_HEIGHT / 2 - 40;
inline const int QUIT_X = WINDOW_WIDTH / 2 - QUIT_TITLE.size() / 2 - 28;
inline const int QUIT_Y = WINDOW_HEIGHT / 2 + 28;

//HUD properties
inline const int SCORE_X = 20;
inline const int SCORE_Y = 50;
inline const int SCORE_NUM_X = 100;
inline const int LIVES_X = SCORE_X;
inline const int LIVES_Y = 80;
inline const int LIVES_NUM_X = SCORE_NUM_X;
inline const int BEST_SCORE_X = WINDOW_WIDTH - 144;
inline const int BEST_SCORE_Y = SCORE_Y;
inline const int BEST_SCORE_NUM_X = BEST_SCORE_X + 70;
inline const int FPS_X = WINDOW_WIDTH - 132;
inline const int FPS_Y = LIVES_Y;
inline const int FPS_NUM_X = BEST_SCORE_NUM_X;

//general game constants
inline const size_t TILE_SIZE = 22;
inline const size_t BIG_DOT_POINTS = 20;
inline const size_t SMALL_DOT_POINTS = 10;

inline const float GAME_END_TIME = 20;
inline const char* HUD_FONT = "freefont-ttf\\sfd\\FreeMono.ttf";
inline const char* BOLD_HUD_FONT = "freefont-ttf\\sfd\\FreeMonoBold.ttf";
inline const float COLLISION_DELTA = 10.f;

//pacman constants
inline const float PACMAN_SPEED = 120.f;
inline const int PACMAN_START_TILE_X = 14;
inline const int PACMAN_START_TILE_Y = 23;
inline const float PACMAN_INVULNERABLE_COUNTER = 3.f;
inline const float PACMAN_VISIBILITY_COUNTER = 0.15f;

//ghost constants
inline const int GHOST_START_TILE_X = 14;
inline const int GHOST_START_TILE_Y = 14;
inline const int TOTAL_OFFSET_X = 198;
inline const int TOTAL_OFFSET_Y = 38;
inline const int MAX_PATH_SEARCH_LENGTH = 10;
inline const float GHOST_SPEED = 70.f;
inline const int DEATH_SPEED_MULTIPLIER = 5;
inline const float GHOST_COUNTER = 6.f;
inline const float RELEASE_COUNTER = 5.f;;

//entity tags
inline const std::string ENEMY_TAG = "Enemy";
inline const std::string DOT_TAG = "Dot";
inline const std::string BIG_DOT_TAG = "BigDot";

//paths to sound files
inline const char* DOT_SOUND = "coin.wav";
inline const char* BIG_DOT_SOUND = "pacman_chomp.wav";
inline const char* EAT_GHOST_SOUND = "pacman_eatghost.wav";
inline const char* PAC_DEATH_SOUND = "pacman_death.wav";
inline const char* PAC_WON_SOUND = "pacman_intermission.wav";
inline const char* MENU_SOUND = "pacman_beginning.wav";

#endif // CONSTANTS_H