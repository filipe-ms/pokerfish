#include "player.h"
#include "cards.h"

static Player player;

Player* Player_Get(void) {
	return &player;
}

void Player_DrawCard(List* deck) {
    if (!deck || deck->size == 0) return;
    Card card = GetAndRemoveTopCard();
    List_Add(player.hand, &card);
}

void Player_Init(void) {
	if (player.hand) List_Destroy(player.hand);

	player.score = 0;
	player.hand = List_Create(sizeof(Card));

    Player_DrawCard(deck);
    Player_DrawCard(deck);
}

void Player_AddScore(int score) {
	player.score += score;
}

int Player_GetHandSum(void) {
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