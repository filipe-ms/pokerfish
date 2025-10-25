#include "game_screen.h"
#include "game_config.h"

#include "raylib.h"

// Game Assets
static Texture cards;
static Texture cardback;

static void LoadScene(void) {
	cards = LoadTexture("AllCards.png");
	cardback = LoadTexture("cardBackRed.png");
}

static void UnloadScene(void) {
	UnloadTexture(cards);
	UnloadTexture(cardback);
}
static void UpdateScene(void) {}

static void DrawGameScreenLeft(void) {
	DrawText("Game Screen", game_window.game_area_center_width, game_window.center_height, 20, RAYWHITE);
}

static void DrawGameScreenRight(void) {
	DrawRectangle(game_window.game_area_start, 0, game_window.game_area_width, game_window.height, BLUE);
}

void DrawScene(void) {
	DrawGameScreenLeft();
	DrawGameScreenRight();
}

Scene game_screen = {
	.load_scene = LoadScene,
	.unload_scene = UnloadScene,
	.update_scene = UpdateScene,
	.draw_scene = DrawScene
};