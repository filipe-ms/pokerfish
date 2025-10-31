#include "draw_card.h"
#include "game_config.h"
#include "player.h"

Rectangle cards[NUMBER_OF_SUITS][NUMBER_OF_RANKS];

Texture card_texture;

static const int card_width = 106;
static const int card_height = 158;
static const Vector2 origin = { 53, 79 };

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
	Rectangle destination = { dest.x, dest.y, card_width, card_height };
	Rectangle source = cards[suit][rank];
	DrawTexturePro(card_texture, source, destination, origin, 0.0f, WHITE);
}

void DrawHandBottom(List* hand) {
	int hand_size = hand->size;
	if (hand_size == 0) return;

	float spacing = 120.0f;

	float start_x = game_window.game_area_center_width - ((hand_size - 1) * spacing) / 2.0f;

	float y_position = game_window.height - 100;

	for (int i = 0; i < hand_size; i++) {
		Card* card = (Card*)List_GetByIndex(hand, i);
		if (card) {
			Vector2 position = { start_x + i * spacing, y_position };
			DrawCard(card->rank, card->suit, position);
		}
	}
}
