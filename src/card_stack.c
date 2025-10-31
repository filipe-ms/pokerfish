#include "card_stack.h"
#include "raylib.h"

// Singleton
List* card_stack = NULL;

static void CardStack_Shuffle(List* source, List* destination) {
	if (!source || !destination) return;
	while (source->size > 0) {
		int index = source->size > 0 ? GetRandomValue(0, source->size - 1) : 0;
		Card* card = (Card*)List_GetByIndex(source, index);

		if (card) {
			Card card_copy = *card;
			List_AddLast(destination, &card_copy);
			List_RemoveAt(source, index);
		}
		else {
			List_RemoveAt(source, 0);
		}
	}
}

void CardStack_Init(void) {
	if (card_stack) {
		List_Destroy(card_stack);
		card_stack = NULL;
	}

	List* orderedDeck = List_Create(sizeof(Card));
	if (!orderedDeck) return;

	for (int suit = 0; suit < NUMBER_OF_SUITS; suit++) {
		for (int rank = ACE; rank <= KING; rank++) {
			Card card = { 0 };
			card.suit = (enum CardSuit)suit;
			card.rank = (enum CardRank)rank;
			List_AddLast(orderedDeck, &card);
		}
	}

	card_stack = List_Create(sizeof(Card));

	if (!card_stack) {
		List_Destroy(orderedDeck);
		return;
	}

	CardStack_Shuffle(orderedDeck, card_stack);

	List_Destroy(orderedDeck);
}

Card CardStack_TakeTopCard(void) {
	if (card_stack == NULL || card_stack->size == 0) return (Card) { 0 };

	Card* card_ptr = (Card*)List_GetByIndex(card_stack, 0);
	Card pickedCard = *card_ptr;
	List_RemoveFirst(card_stack);
	return pickedCard;
}