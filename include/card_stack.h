#pragma once

#include "list.h"

typedef enum {
	CLUBS,
	DIAMONDS,
	HEARTS,
	SPADES,
	NUMBER_OF_SUITS
} CardSuit;

typedef enum {
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	NUMBER_OF_RANKS
} CardRank;

typedef struct {
	enum CardSuit suit;
	enum CardRank rank;
} Card;

extern List* card_stack;

void CardStack_Init(void);
Card CardStack_TakeTopCard(void);