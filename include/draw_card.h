#pragma once

#include "raylib.h"
#include "card_stack.h"

void LoadCardTextures(void);
void UnloadCardTextures(void);
void DrawCard(CardRank rank, CardSuit suit, Vector2 dest);
void DrawHandBottom(List* hand);