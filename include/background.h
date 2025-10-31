// background.h

#pragma once

typedef enum {
    BG_BLUE = 0,
    BG_GREEN,
    BG_TEAL,
    BG_TOTAL
} BackgroundStyle;

typedef enum {
    STRETCH_NONE,
    STRETCH_SCREEN,
    STRETCH_GAME_AREA,
} Stretch;

void SetBackground(BackgroundStyle style, Stretch stretch);
void DrawBackground(void);
void LoadBackground(void);
void UnloadBackground(void);