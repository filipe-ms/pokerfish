#include "draw_card.h"
#include "game_config.h"
#include "player.h"

static Rectangle cards[NUMBER_OF_SUITS][NUMBER_OF_RANKS];

static Texture card_texture;

static const int card_width = 106;
static const int card_height = 158;
static const Vector2 origin = { 53, 79 };
static const float spacing = 120.0f;

static void InitCards(void) {
	for (int suit = 0; suit < NUMBER_OF_SUITS; suit++) {
		for (int rank = 0; rank < NUMBER_OF_RANKS; rank++) {
			cards[suit][rank] = (Rectangle){
				.x = (float)rank * card_width,
				.y = (float)suit * card_height,
				.width = (float)card_width,
				.height = (float)card_height
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

void DrawCard(CardRank rank, CardSuit suit, Vector2 dest, float rotation) {
	Rectangle destination = { dest.x, dest.y, card_width, card_height };
	Rectangle source = cards[suit][rank];
	DrawTexturePro(card_texture, source, destination, origin, rotation, WHITE);
}

void DrawHand(List* hand, float start_x, float pos_y, float rotation) {
	if (!hand || !hand->size) return;

	for (int i = 0; i < hand->size; i++) {
		Card* card = (Card*)List_GetByIndex(hand, i);
		if (card) {
			Vector2 position = { start_x + i * spacing, pos_y };
			DrawCard(card->rank, card->suit, position, rotation);
		}
	}
}

void DrawHandHorizontal(List* hand, float pos_x, float  start_y, float rotation) {
	if (!hand || !hand->size) return;

	for (int i = 0; i < hand->size; i++) {
		Card* card = (Card*)List_GetByIndex(hand, i);
		if (card) {
			Vector2 position = { pos_x, start_y + i * spacing };
			DrawCard(card->rank, card->suit, position, rotation);
		}
	}
}
