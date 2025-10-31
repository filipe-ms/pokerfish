#pragma once

#include "card_stack.h"
#include "list.h"

typedef struct
{
	int score;
	List* hand;
} Player;

extern Player player;
void Player_Init(Card* cards);
void Player_AddScore(int sum);
void Player_AddToHand(Card card);