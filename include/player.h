#pragma once

#include "cards.h"
#include "list.h"

typedef struct
{
	int score;
	List* hand;
} Player;

Player* Player_Get(void);
void Player_Init(void);
void Player_AddScore(int sum);
void Player_DrawCard(List* deck);
int Player_GetHandSum(void);