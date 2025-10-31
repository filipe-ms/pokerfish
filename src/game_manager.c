// game_manager.c

#include "game_manager.h"
#include "player.h"
#include "card_stack.h"
#include "draw_card.h"
#include "scene_manager.h"
#include "game_menu.h"
#include "raylib.h"
#include "background.h"

static int GetHandSum(void);

void LoadGame(void) {
	LoadCardTextures();
	CardStack_Init();

	Card initial_cards[] = {
		CardStack_TakeTopCard(),
		CardStack_TakeTopCard()
	};

	Player_Init(initial_cards);
}

void UnloadGame(void) {
	UnloadCardTextures();
}

void UpdateGame(void) {
	if (GetHandSum() > 21) {
		ChangeScene(game_menu);
	}
	else {
		if (IsKeyPressed(KEY_SPACE)) {
			Player_AddToHand(CardStack_TakeTopCard());
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


// Static
static int GetHandSum() {
	int sum = 0;
	int ace_count = 0;

	if (!player.hand) return 0;

	for (int i = 0; i < player.hand->size; i++) {
		Card* card = (Card*)List_GetByIndex(player.hand, i);
		if (card) {
			if (card->rank >= TEN && card->rank <= KING) sum += 10;
			else if (card->rank >= TWO && card->rank <= NINE) sum += (int)(card->rank + 1);
			else if (card->rank == ACE) ace_count++;
		}
	}

	for (int i = 0; i < ace_count; i++) {
		if (sum + 11 <= 21) sum += 11;
		else sum += 1;
	}

	return sum;
}