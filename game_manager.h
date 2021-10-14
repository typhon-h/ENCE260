/** @file   game_manager.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   10 Oct 2021
 *  @brief  Controls key game components:
 *          Menus
 *          Game Start/Stop
 *          Key events
 */

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "system.h"

typedef enum {
	HARD_MODE = 0,
	THREE_LIVES,
	WALL_PUSH
} gamemode_t;

// Add more if perferred.
#define DIFFERENT_GAMEMODES 3
#define HARD_MODE_TEXT      " HARDMODE "
#define THREE_LIVES_TEXT    " THREE LIVES "
#define WALL_PUSH_TEXT      " WALL PUSH "


// Initialize game manager
void game_init(uint8_t message_rate);

// Increment the score
void increment_score(void);

// Return current state of game (is it being played)
bool get_game_state(void);

// Retturn PAUSE state of the game (should i pause)
bool get_pause_state(void);

// Checks button input to update PAUSE update
void check_pause_button(void);

// Initialize game components and toggle game state
void game_start(void);

// Poll button input to start game
void game_state_update(void);

// Check game ending conditions and end the game
void check_collisions(void);

//Stop the current game and open game outro
void decrease_lives(void);

// Post game menu - Game over screen and score
void game_outro(void);

// Returns collsion status when given a position
bool collision_dectection(void);

#endif
