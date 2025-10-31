// game_blackjack.c

#include "game_blackjack.h"
#include "game_config.h"
#include "player.h"
#include "card_stack.h"
#include "draw_card.h"
#include "scene_manager.h"
#include "game_menu.h"
#include "raylib.h"
#include "background.h"

static void LoadGame(void);
static void UnloadGame(void);
static void UpdateGame(void);
static void DrawGame(void);

Scene blackjack = {
	.load_scene = LoadGame,
	.unload_scene = UnloadGame,
	.update_scene = UpdateGame,
	.draw_scene = DrawGame
};

static int GetHandSum(void);

static void LoadGame(void) {
	LoadCardTextures();
	CardStack_Init();

	Card initial_cards[] = {
		CardStack_TakeTopCard(),
		CardStack_TakeTopCard()
	};

	Player_Init(initial_cards);
}

static void UnloadGame(void) {
	UnloadCardTextures();
}

static void UpdateGame(void) {
	if (GetHandSum() > 21) {
		ChangeScene(game_menu);
	}
	else {
		if (IsKeyPressed(KEY_SPACE)) {
			Player_AddToHand(CardStack_TakeTopCard());
		}
	}
}

static void DrawGame(void) {
	DrawBackground();
	DrawHand(player.hand, game_window.game_area_center_width, game_window.height - 100, 0);
}

static int GetHandSum() {
	int sum = 0;
	int ace_count = 0;

	if (!player.hand) return 0;

	for (int i = 0; i < player.hand->size; i++) {
		Card* card = (Card*)List_GetByIndex(player.hand, i);
		if (card) {
			if ((int)card->rank >= TEN && (int)card->rank <= KING) sum += 10;
			else if ((int)card->rank >= TWO && (int)card->rank <= NINE) sum += (int)(card->rank + 1);
			else if ((int)card->rank == ACE) ace_count++;
		}
	}

	for (int i = 0; i < ace_count; i++) {
		if (sum + 11 <= 21) sum += 11;
		else sum += 1;
	}

	return sum;
}