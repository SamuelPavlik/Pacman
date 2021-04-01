#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

inline const size_t TILE_SIZE = 22;
inline const size_t BIG_DOT_POINTS = 20;
inline const size_t SMALL_DOT_POINTS = 10;
inline const float GHOST_COUNTER = 20;
inline const float GAME_END_TIME = 20;
inline const char* HUD_FONT = "freefont-ttf\\sfd\\FreeMono.ttf";
inline const float AVATAR_SPEED = 120.f;
inline const float GHOST_SPEED = 70.f;
inline const int DEATH_SPEED_MULTIPLIER = 5;
inline const int AVATAR_START_TILE_X = 14;
inline const int AVATAR_START_TILE_Y = 23;
inline const int GHOST_START_TILE_X = 14;
inline const int GHOST_START_TILE_Y = 14;
inline const int TOTAL_OFFSET_X = 198;
inline const int TOTAL_OFFSET_Y = 38;
inline const int MAX_PATH_SEARCH_LENGTH = 10;
inline const float COLLISION_DELTA = 20.f;

inline const std::string ENEMY_TAG = "Enemy";
inline const std::string DOT_TAG = "Dot";
inline const std::string BIG_DOT_TAG = "BigDot";

#endif // CONSTANTS_H