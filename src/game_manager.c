// game_manager.c

#include "game_manager.h"
#include "player.h"
#include "draw_card.h"
#include "scene_manager.h"
#include "game_menu.h"
#include "raylib.h"
#include "background.h"

void LoadGame(void) {
	LoadCardTextures();
	InitDeck();
	Player_Init();
}

void UnloadGame(void) {
	UnloadCardTextures();
}

void UpdateGame(void) {
	if (Player_GetHandSum() > 21) {
		ChangeScene(game_menu);
	}
	else {
		if (IsKeyPressed(KEY_SPACE)) {
			Player_DrawCard(deck);
		}
	}
}

void DrawGame(void) {
	DrawBackground();
	DrawPlayerHand();
}

Scene game = {
	.load_scene = LoadGame,
	.unload_scene = UnloadGame,
	.update_scene = UpdateGame,
	.draw_scene = DrawGame
};