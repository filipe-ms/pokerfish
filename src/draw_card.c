#include "draw_card.h"
#include "game_config.h"
#include "player.h"

Rectangle cards[NUMBER_OF_SUITS][NUMBER_OF_RANKS];

Texture card_texture;

static const int card_width = 419;
static const int card_height = 626;
static const int draw_width = 67;
static const int draw_height = 100;
static const Vector2 origin = { 67 / 2.0f, 100 / 2.0f };

static void InitCards(void) {
	for (int suit = 0; suit < NUMBER_OF_SUITS; suit++) {
		for (int rank = 0; rank < NUMBER_OF_RANKS; rank++) {
			cards[suit][rank] = (Rectangle){
				.x = rank * card_width,
				.y = suit * card_height,
				.width = card_width,
				.height = card_height
			};
		}
	}
}

void LoadCardTextures(void) {
	card_texture = LoadTexture("all_cards.png");
	InitCards();
}

void UnloadCardTextures(void) {
	UnloadTexture(card_texture);
}

void DrawCard(CardRank rank, CardSuit suit, Vector2 dest) {
	Rectangle destination = { dest.x, dest.y, draw_width, draw_height };
	Rectangle source = cards[suit][rank];
	DrawTexturePro(card_texture, source, destination, origin, 0.0f, WHITE);
}

void DrawPlayerHand(void) {
	int hand_size = Player_Get()->hand->size;
	if (hand_size == 0) return;

	float spacing = 100.0f;

	float start_x = game_window.game_area_center_width - ((hand_size - 1) * spacing) / 2.0f;

	float y_position = game_window.height - 100;

	for (int i = 0; i < hand_size; i++) {
		Card* card = (Card*)List_GetByIndex(Player_Get()->hand, i);
		if (card) {
			Vector2 position = { start_x + i * spacing, y_position };
			DrawCard(card->rank, card->suit, position);
		}
	}
}
