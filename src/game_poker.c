#include "game_poker.h"

#include "player.h"
#include "card_stack.h"
#include "draw_card.h"
#include "scene_manager.h"
#include "game_menu.h"
#include "raylib.h"
#include "background.h"
#include "list.h"

struct NPC {
	List* hand;
	int score;
};

List* table_cards = NULL;
List* left_opponent = NULL;
List* top_opponent = NULL;
List* right_opponent = NULL;

static void LoadGame() {
	LoadCardTextures();
	CardStack_Init();
	Card initial_cards[] = {
		CardStack_TakeTopCard(),
		CardStack_TakeTopCard()
	};
	Player_Init(initial_cards);
}

static void UnloadGame() {

}

static void UpdateGame() {

}

static void DrawGame() {
	DrawBackground();
	//DrawHand(player.hand);
}

Scene game_poker = {
	.load_scene = LoadGame,
	.unload_scene = UnloadGame,
	.update_scene = UpdateGame,
	.draw_scene = DrawGame
};
