#include "game_poker.h"

#include "game_config.h" // For game_window
#include "player.h"      // For the global 'player'
#include "card_stack.h"
#include "draw_card.h"
#include "scene_manager.h"
#include "game_menu.h" // To go back
#include "raylib.h"
#include "background.h"
#include "list.h"

#include <stdio.h> // For sprintf

// --- Game Definitions ---

// Represents a player at the table
typedef struct {
    List* hand;         // Player's 2-card hand
    int score;          // Player's chip stack
    int current_bet;    // How much this player has bet in the current round
    bool folded;        // Has this player folded?
    bool is_active;     // Is this player in the hand? (not folded, not all-in)
    bool is_human;      // Is this the human player?
    Vector2 hand_pos;   // Where to draw the hand
    const char* name;   // Player's name
} PokerPlayer;

// Game states for betting rounds
typedef enum {
    GAME_STATE_PREFLOP,
    GAME_STATE_FLOP,
    GAME_STATE_TURN,
    GAME_STATE_RIVER,
    GAME_STATE_SHOWDOWN,
    GAME_STATE_HAND_OVER
} GameState;

// Player actions
typedef enum {
    ACTION_FOLD,
    ACTION_CHECK,
    ACTION_CALL,
    ACTION_BET,     // Opening bet
    ACTION_RAISE    // Raising a bet
} PlayerAction;

// UI selection for human player
typedef enum {
    UI_ACTION_FOLD,
    UI_ACTION_CHECK_CALL,
    UI_ACTION_BET_RAISE
} UIAction;


// --- Static Variables ---

static PokerPlayer players[4];
static const int NUM_PLAYERS = 4;
static List* table_cards = NULL; // Community cards

static GameState current_state;
static int pot = 0;
static int current_bet_to_match = 0; // The highest bet in the current round
static int small_blind_amount = 10;
static int big_blind_amount = 20;
static int min_raise = 20;

static int dealer_pos = 0;
static int sb_pos = 1;
static int bb_pos = 2;
static int current_player_turn = 3; // Player after big blind goes first preflop
static int last_raiser = -1; // Index of the last player to raise

static UIAction selected_ui_action = UI_ACTION_CHECK_CALL;
static char status_text[100] = "Welcome to Texas Hold'em!";

// --- Function Prototypes ---

static void LoadGame(void);
static void UnloadGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void StartNewHand(void);
static void AdvanceTurn(void);
static void StartNextBettingRound(void);
static void HandlePlayerAction(int player_index, PlayerAction action, int amount);
static void HandleAIAction(int player_index);
static PlayerAction GetValidActions(int player_index, int* call_amount, int* min_raise_amount);

// --- Scene Definition ---

Scene poker = {
    .load_scene = LoadGame,
    .unload_scene = UnloadGame,
    .update_scene = UpdateGame,
    .draw_scene = DrawGame
};

// --- Game Logic Functions ---

// Resets table for a new hand
static void StartNewHand(void) {
    CardStack_Init();
    pot = 0;
    current_bet_to_match = big_blind_amount;
    min_raise = big_blind_amount;
    current_state = GAME_STATE_PREFLOP;
    sprintf(status_text, "Starting new hand. Blinds are %d/%d", small_blind_amount, big_blind_amount);

    // Clear table cards
    if (table_cards) {
        List_Destroy(table_cards);
    }
    table_cards = List_Create(sizeof(Card));

    // Rotate dealer and blinds
    dealer_pos = (dealer_pos + 1) % NUM_PLAYERS;
    sb_pos = (dealer_pos + 1) % NUM_PLAYERS;
    bb_pos = (dealer_pos + 2) % NUM_PLAYERS;
    current_player_turn = (bb_pos + 1) % NUM_PLAYERS; // Player after BB
    last_raiser = bb_pos; // Big blind is the initial "raiser"

    // Reset players and deal cards
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].hand) {
            List_Destroy(players[i].hand);
        }
        players[i].hand = List_Create(sizeof(Card));
        players[i].current_bet = 0;
        players[i].folded = false;

        // Only active if they have chips
        players[i].is_active = (players[i].score > 0);

        if (players[i].is_active) {
            Card c1 = CardStack_TakeTopCard();
            Card c2 = CardStack_TakeTopCard();
            List_AddLast(players[i].hand, &c1);
            List_AddLast(players[i].hand, &c2);
        }
    }

    // --- Post Blinds ---
    // Small Blind
    int sb_bet = (players[sb_pos].score >= small_blind_amount) ? small_blind_amount : players[sb_pos].score;
    players[sb_pos].score -= sb_bet;
    players[sb_pos].current_bet = sb_bet;
    pot += sb_bet;
    sprintf(status_text, "%s posts small blind %d", players[sb_pos].name, sb_bet);


    // Big Blind
    int bb_bet = (players[bb_pos].score >= big_blind_amount) ? big_blind_amount : players[bb_pos].score;
    players[bb_pos].score -= bb_bet;
    players[bb_pos].current_bet = bb_bet;
    pot += bb_bet;
    sprintf(status_text, "%s posts big blind %d", players[bb_pos].name, bb_bet);
}

// Main scene load
static void LoadGame(void) {
    LoadCardTextures();

    // Initialize players one time
    // Player 0 (Human)
    players[0].hand = NULL;
    players[0].score = 1000;
    players[0].is_human = true;
    players[0].name = "Player";
    players[0].hand_pos = (Vector2){ game_window.game_area_center_width - 60, game_window.height - 100 };

    // Player 1 (Left)
    players[1].hand = NULL;
    players[1].score = 1000;
    players[1].is_human = false;
    players[1].name = "NPC Left";
    players[1].hand_pos = (Vector2){ 100, game_window.center_height - 80 };

    // Player 2 (Top)
    players[2].hand = NULL;
    players[2].score = 1000;
    players[2].is_human = false;
    players[2].name = "NPC Top";
    players[2].hand_pos = (Vector2){ game_window.game_area_center_width - 60, 100 };

    // Player 3 (Right)
    players[3].hand = NULL;
    players[3].score = 1000;
    players[3].is_human = false;
    players[3].name = "NPC Right";
    players[3].hand_pos = (Vector2){ game_window.game_area_start - 100, game_window.center_height - 80 };


    // Use the global player from player.c for score?
    // For now, we'll use the local PokerPlayer struct.
    // player.score = 1000; // If we wanted to use the global one

    StartNewHand();
}

// Main scene unload
static void UnloadGame(void) {
    UnloadCardTextures();
    if (table_cards) List_Destroy(table_cards);
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].hand) List_Destroy(players[i].hand);
    }
}

// Moves to the next player's turn
static void AdvanceTurn(void) {
    int active_players_left = 0;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].is_active && !players[i].folded) {
            active_players_left++;
        }
    }

    if (active_players_left <= 1) {
        // Everyone else folded
        current_state = GAME_STATE_HAND_OVER;
        // TODO: Award pot to last active player
        sprintf(status_text, "%s wins %d!", players[current_player_turn].name, pot);
        return;
    }

    // Find next active player
    do {
        current_player_turn = (current_player_turn + 1) % NUM_PLAYERS;
    } while (players[current_player_turn].folded || !players[current_player_turn].is_active);


    // Check if betting round is over
    // Round ends when:
    // 1. We've looped back to the last raiser.
    // 2. OR (preflop) we've looped back to the big blind and no one raised.
    if (current_player_turn == last_raiser) {
        StartNextBettingRound();
    }
    else if (current_state == GAME_STATE_PREFLOP && current_player_turn == bb_pos && current_bet_to_match == big_blind_amount) {
        // Big blind gets an option to check or raise
        // This logic is slightly simplified.
    }
}

// Deals community cards and moves to next state
static void StartNextBettingRound(void) {
    current_bet_to_match = 0;
    min_raise = big_blind_amount;

    // Find first active player to act (left of dealer)
    current_player_turn = dealer_pos;
    do {
        current_player_turn = (current_player_turn + 1) % NUM_PLAYERS;
    } while (players[current_player_turn].folded || !players[current_player_turn].is_active);

    last_raiser = current_player_turn; // First person to act is the "last raiser"

    // Reset all players' current_bet for the new round
    for (int i = 0; i < NUM_PLAYERS; i++) {
        players[i].current_bet = 0;
    }

    switch (current_state) {
    case GAME_STATE_PREFLOP:
        current_state = GAME_STATE_FLOP;
		Card c1 = CardStack_TakeTopCard();
		Card c2 = CardStack_TakeTopCard();
		Card c3 = CardStack_TakeTopCard();
        List_AddLast(table_cards, &c1);
        List_AddLast(table_cards, &c2);
        List_AddLast(table_cards, &c3);
        sprintf(status_text, "Dealing the Flop. %s's turn.", players[current_player_turn].name);
        break;
    case GAME_STATE_FLOP:
        current_state = GAME_STATE_TURN;
		Card c4 = CardStack_TakeTopCard();
        List_AddLast(table_cards, &c4);
        sprintf(status_text, "Dealing the Turn. %s's turn.", players[current_player_turn].name);
        break;
    case GAME_STATE_TURN:
        current_state = GAME_STATE_RIVER;
		Card c5 = CardStack_TakeTopCard();
        List_AddLast(table_cards, &c5);
        sprintf(status_text, "Dealing the River. %s's turn.", players[current_player_turn].name);
        break;
    case GAME_STATE_RIVER:
        current_state = GAME_STATE_SHOWDOWN;
        // TODO: Implement hand evaluation
        sprintf(status_text, "Showdown! (Hand evaluation not implemented)");
        break;
    case GAME_STATE_SHOWDOWN:
        current_state = GAME_STATE_HAND_OVER;
        // TODO: Award pot to winner
        sprintf(status_text, "Hand over. Press ENTER to start next hand.");
        break;
    case GAME_STATE_HAND_OVER:
        StartNewHand();
        break;
    }
}

// Gets valid actions for the current player
static PlayerAction GetValidActions(int player_index, int* call_amount, int* min_raise_amount) {
    PokerPlayer* p = &players[player_index];
    *call_amount = current_bet_to_match - p->current_bet;

    // Can they check?
    if (*call_amount == 0) {
        *min_raise_amount = min_raise;
        return ACTION_CHECK; // Can check or bet/raise
    }

    // They must call, raise, or fold
    *min_raise_amount = current_bet_to_match + min_raise;
    return ACTION_CALL; // Can call or raise
}

// Processes an action from a player
static void HandlePlayerAction(int player_index, PlayerAction action, int amount) {
    PokerPlayer* p = &players[player_index];
    if (p->folded || !p->is_active) return; // Should not happen

    int call_amount = current_bet_to_match - p->current_bet;

    switch (action) {
    case ACTION_FOLD:
        p->folded = true;
        sprintf(status_text, "%s folds.", p->name);
        break;

    case ACTION_CHECK:
        // Only valid if call_amount is 0
        if (call_amount == 0) {
            sprintf(status_text, "%s checks.", p->name);
        }
        else {
            // This is a logic error, but we'll treat it as a fold
            p->folded = true;
            sprintf(status_text, "%s (tried to check) folds.", p->name);
        }
        break;

    case ACTION_CALL:
        if (call_amount > p->score) { // Player is all-in
            call_amount = p->score;
        }
        p->score -= call_amount;
        p->current_bet += call_amount;
        pot += call_amount;
        sprintf(status_text, "%s calls %d.", p->name, call_amount);
        break;

    case ACTION_BET: // Same as raise, but for opening bet
    case ACTION_RAISE:
        // 'amount' is the TOTAL bet they want to make
        if (amount < current_bet_to_match + min_raise) {
            // Not a valid raise, force a call
            HandlePlayerAction(player_index, ACTION_CALL, 0);
            return;
        }
        if (amount > p->score) { // Player is all-in
            amount = p->score;
        }

        int amount_to_add = amount - p->current_bet;
        p->score -= amount_to_add;
        p->current_bet = amount;
        pot += amount_to_add;

        current_bet_to_match = p->current_bet;
        min_raise = current_bet_to_match - current_bet_to_match; // Simplified
        last_raiser = player_index;
        sprintf(status_text, "%s raises to %d.", p->name, current_bet_to_match);
        break;
    }

    AdvanceTurn();
}

// Simple AI logic
static void HandleAIAction(int player_index) {
    int call_amount = 0;
    int min_raise_amount = 0;
    PlayerAction base_action = GetValidActions(player_index, &call_amount, &min_raise_amount);

    // Very simple AI: Just call
    if (base_action == ACTION_CHECK) {
        HandlePlayerAction(player_index, ACTION_CHECK, 0);
    }
    else {
        HandlePlayerAction(player_index, ACTION_CALL, 0);
    }
    // TODO: Add simple fold/raise logic
}

// Main update loop
static void UpdateGame(void) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        ChangeScene(game_menu);
        return;
    }

    if (current_state == GAME_STATE_HAND_OVER) {
        if (IsKeyPressed(KEY_ENTER)) {
            StartNewHand();
        }
        return;
    }

    if (current_state == GAME_STATE_SHOWDOWN) {
        if (IsKeyPressed(KEY_ENTER)) {
            StartNextBettingRound();
        }
        return;
    }

    // Check if it's human's turn
    if (players[current_player_turn].is_human) {
        // Handle UI selection
        if (IsKeyPressed(KEY_LEFT)) {
            if (selected_ui_action == UI_ACTION_CHECK_CALL) selected_ui_action = UI_ACTION_FOLD;
            else if (selected_ui_action == UI_ACTION_BET_RAISE) selected_ui_action = UI_ACTION_CHECK_CALL;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (selected_ui_action == UI_ACTION_FOLD) selected_ui_action = UI_ACTION_CHECK_CALL;
            else if (selected_ui_action == UI_ACTION_CHECK_CALL) selected_ui_action = UI_ACTION_BET_RAISE;
        }

        // Handle action
        if (IsKeyPressed(KEY_ENTER)) {
            int call_amount = 0;
            int min_raise_amount = 0;
            PlayerAction base_action = GetValidActions(current_player_turn, &call_amount, &min_raise_amount);

            if (selected_ui_action == UI_ACTION_FOLD) {
                HandlePlayerAction(current_player_turn, ACTION_FOLD, 0);
            }
            else if (selected_ui_action == UI_ACTION_CHECK_CALL) {
                if (base_action == ACTION_CHECK) {
                    HandlePlayerAction(current_player_turn, ACTION_CHECK, 0);
                }
                else {
                    HandlePlayerAction(current_player_turn, ACTION_CALL, 0);
                }
            }
            else if (selected_ui_action == UI_ACTION_BET_RAISE) {
                // Simple raise: just min raise
                int raise_to_amount = current_bet_to_match + min_raise;
                if (base_action == ACTION_CHECK) {
                    HandlePlayerAction(current_player_turn, ACTION_BET, min_raise);
                }
                else {
                    HandlePlayerAction(current_player_turn, ACTION_RAISE, raise_to_amount);
                }
            }
        }
    }
    else { // AI's turn
        // Add a slight delay later?
        HandleAIAction(current_player_turn);
    }
}

// --- Draw Functions ---

static void DrawPlayerInfo(int player_index) {
    PokerPlayer* p = &players[player_index];
    Vector2 base_pos = p->hand_pos;
    Color text_color = (player_index == current_player_turn) ? YELLOW : WHITE;

    // Draw Name and Score
    DrawText(TextFormat("%s", p->name), base_pos.x - 40, base_pos.y - 40, 20, text_color);
    DrawText(TextFormat("$%d", p->score), base_pos.x - 40, base_pos.y - 20, 20, text_color);

    // Draw Hand
    if (p->hand) {
        if (p->is_human || current_state == GAME_STATE_SHOWDOWN) {
            DrawHand(p->hand, p->hand_pos.x, p->hand_pos.y, 0);
        }
        else {
            // Draw card backs for NPCs
            DrawCard(KING, SPADES, p->hand_pos, 0); // TODO: Need a DrawCardBack function
            DrawCard(KING, SPADES, (Vector2) { p->hand_pos.x + 120, p->hand_pos.y }, 0);
        }
    }

    // Draw Bet
    if (p->current_bet > 0) {
        DrawText(TextFormat("Bet: %d", p->current_bet), base_pos.x, base_pos.y + 100, 18, SKYBLUE);
    }

    // Draw Folded
    if (p->folded) {
        DrawText("FOLDED", base_pos.x, base_pos.y + 50, 24, RED);
    }

    // Draw Dealer/Blind buttons
    if (player_index == dealer_pos) DrawCircle(base_pos.x - 60, base_pos.y + 40, 15, WHITE);
    if (player_index == sb_pos) DrawCircle(base_pos.x - 60, base_pos.y + 40, 15, GREEN);
    if (player_index == bb_pos) DrawCircle(base_pos.x - 60, base_pos.y + 40, 15, BLUE);
}

static void DrawTable() {
    // Draw community cards
    float table_center_x = game_window.game_area_center_width - (106 * 2.5f);
    float table_center_y = game_window.center_height - 80;
    DrawHand(table_cards, table_center_x, table_center_y, 0);

    // Draw Pot
    DrawText(TextFormat("Pot: %d", pot), table_center_x + 200, table_center_y + 100, 24, GOLD);

    // Draw Status
    DrawText(status_text, game_window.game_area_center_width - MeasureText(status_text, 20) / 2, game_window.height - 250, 20, WHITE);
}

static void DrawUI() {
    if (!players[current_player_turn].is_human || current_state >= GAME_STATE_SHOWDOWN) return;

    int call_amount = 0;
    int min_raise_amount = 0;
    PlayerAction base_action = GetValidActions(current_player_turn, &call_amount, &min_raise_amount);

    float ui_y = game_window.height - 200;
    float ui_x_start = game_window.game_area_center_width - 200;

    // --- Draw Fold ---
    DrawRectangle(ui_x_start, ui_y, 100, 50, (selected_ui_action == UI_ACTION_FOLD) ? RED : DARKGRAY);
    DrawText("FOLD", ui_x_start + 25, ui_y + 15, 20, WHITE);

    // --- Draw Check/Call ---
    const char* check_call_text = (base_action == ACTION_CHECK) ? "CHECK" : TextFormat("CALL %d", call_amount);
    DrawRectangle(ui_x_start + 120, ui_y, 100, 50, (selected_ui_action == UI_ACTION_CHECK_CALL) ? SKYBLUE : DARKGRAY);
    DrawText(check_call_text, ui_x_start + 130, ui_y + 15, 20, WHITE);

    // --- Draw Bet/Raise ---
    const char* bet_raise_text = (base_action == ACTION_CHECK) ? TextFormat("BET %d", min_raise_amount) : TextFormat("RAISE TO %d", min_raise_amount);
    DrawRectangle(ui_x_start + 240, ui_y, 140, 50, (selected_ui_action == UI_ACTION_BET_RAISE) ? GREEN : DARKGRAY);
    DrawText(bet_raise_text, ui_x_start + 250, ui_y + 15, 20, WHITE);

}

// Main draw loop
static void DrawGame(void) {
    DrawBackground();

    // Draw Players
    for (int i = 0; i < NUM_PLAYERS; i++) {
        DrawPlayerInfo(i);
    }

    // Draw Table
    DrawTable();

    // Draw UI
    DrawUI();
}
