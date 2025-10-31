#include "player.h"
#include "card_stack.h"

Player player;

Player* Player_Get(void) {
	return &player;
}

void Player_AddToHand(Card card) {
    List_Add(player.hand, &card);
}

void Player_Init(Card* cards) {
	if (player.hand) List_Destroy(player.hand);

	player.score = 0;
	player.hand = List_Create(sizeof(Card));

    if (cards) {
        for (int i = 0; i < 2; i++) {
            Player_AddToHand(cards[i]);
        }
	}
}

void Player_AddScore(int score) {
	player.score += score;
}