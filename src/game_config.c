// game_config.c

#include "game_config.h"

#define GAME_SCREEN_WIDTH 1280
#define GAME_SCREEN_HEIGHT 800

const GameWindow game_window = {
    .width = GAME_SCREEN_WIDTH,
    .height = GAME_SCREEN_HEIGHT,
    .center_width = GAME_SCREEN_WIDTH / 2,
    .center_height = GAME_SCREEN_HEIGHT / 2,
    .game_area_start = GAME_SCREEN_WIDTH * 0.75,
	.game_area_width = GAME_SCREEN_WIDTH - (GAME_SCREEN_WIDTH * 0.7),
	.game_area_center_width = (GAME_SCREEN_WIDTH * 0.7) / 2
};