#pragma once

#include "raylib.h"
#include "cards.h"

void LoadCardTextures(void);
void UnloadCardTextures(void);
void DrawCard(CardRank rank, CardSuit suit, Vector2 dest);
void DrawPlayerHand(void);