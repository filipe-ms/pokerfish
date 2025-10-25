#include "cards.h"
#include "raylib.h"

List* deck = NULL;

static void Shuffle(List* source, List* destination) {
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

void InitDeck(void) {
	if (deck) {
		List_Destroy(deck);
		deck = NULL;
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

	deck = List_Create(sizeof(Card));

	if (!deck) {
		List_Destroy(orderedDeck);
		return;
	}

	Shuffle(orderedDeck, deck);

	List_Destroy(orderedDeck);
}

Card GetAndRemoveTopCard(void) {
	if (deck == NULL || deck->size == 0) return (Card) { 0 };

	Card* card_ptr = (Card*)List_GetByIndex(deck, 0);
	Card pickedCard = *card_ptr;
	List_RemoveFirst(deck);
	return pickedCard;
}