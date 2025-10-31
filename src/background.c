#include "background.h"
#include "game_config.h"
#include "raylib.h"

static Texture background[3];

typedef struct {
    BackgroundStyle style;
    Stretch stretch;
} Background;

static Background current_background;

void SetBackground(BackgroundStyle style, Stretch area) {
    current_background.style = style;
    current_background.stretch = area;
}

void DrawBackground(void) {
    Rectangle destRec;

    if(current_background.stretch == STRETCH_SCREEN) {
        destRec = (Rectangle){0, 0, (float)game_window.width, (float)game_window.height};
    }
    else if(current_background.stretch == STRETCH_GAME_AREA) {
         destRec = (Rectangle){0, 0, (float)game_window.game_area_width, (float)game_window.height};
    } else {
        return;
    }

    DrawTexturePro(
        background[current_background.style],
        (Rectangle){0, 0, (float)background[current_background.style].width, (float)background[current_background.style].height},
        destRec,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

void LoadBackground(void) {
    background[BG_BLUE] = LoadTexture("bg_blue.png");
    background[BG_TEAL] = LoadTexture("bg_teal.png");
    background[BG_GREEN] = LoadTexture("bg_green.png");

    current_background.style = BG_TEAL;
    current_background.stretch = STRETCH_SCREEN;
}

void UnloadBackground(void) {
    for (int i = 0; i < BG_TOTAL; i++) {
        UnloadTexture(background[i]);
    }
}