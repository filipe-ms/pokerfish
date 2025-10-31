// game_menu.c

#include "game_menu.h"
#include "game_config.h"
#include "background.h"
#include "scene_manager.h"
#include "game_screen.h"
#include "game_blackjack.h"

#include "raylib.h"

typedef enum {
    START,
    QUIT
} Option;

static Option option = START;

static void UpdateOption() {
    if (IsKeyPressed(KEY_DOWN)) option++;
    else if (IsKeyPressed(KEY_UP)) option--;
    
    else if (IsKeyPressed(KEY_ENTER) && option == QUIT) EndSceneManager();
    else if (IsKeyPressed(KEY_ENTER) && option == START) ChangeScene(blackjack);

    if(option < START) option = QUIT;
    if(option > QUIT) option = START;
}

static void LoadScene(void) {}
static void UnloadScene(void) {}

static void UpdateScene(void) {
    UpdateOption();
}

static void DrawScene(void) {
    DrawBackground();
    DrawText("Start", game_window.center_width, game_window.center_height, 20, option == START ? RED : RAYWHITE);
    DrawText("Quit", game_window.center_width, game_window.center_height + 40, 20, option == QUIT ? RED : RAYWHITE);
}

Scene game_menu = {
    .load_scene = LoadScene,
    .unload_scene = UnloadScene,
    .update_scene = UpdateScene,
    .draw_scene = DrawScene
};