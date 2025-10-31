#pragma once

#include "raylib.h"
#include "card_stack.h"

void LoadCardTextures(void);
void UnloadCardTextures(void);
void DrawCard(CardRank rank, CardSuit suit, Vector2 dest, float rotation);
void DrawHand(List* hand, float start_x, float pos_y, float rotation);
void DrawHandHorizontal(List* hand, float pos_x, float  start_y, float rotation);