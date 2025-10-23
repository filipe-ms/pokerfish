// game_config.h

#pragma once

/*
    Codewide constants and definitions go here.
*/

typedef struct {
    int width;
    int height;
    int center_width;
    int center_height;
	int game_area_start;
	int game_area_width;
	int game_area_center_width;
} GameWindow;

extern const GameWindow game_window;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;