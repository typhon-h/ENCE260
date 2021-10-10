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

// Initialize game manager
void game_init(void);

// Return current state of game (is it being played)
bool get_game_state(void);

// Initialize game components and toggle game state
void game_start(uint16_t seed);

// Poll button input to start game
void game_state_update(uint16_t seed);

//Stop the current game and open game outro
void game_end(void);

// For game ending
void game_outro(void);

// Returns collsion status when given a position
bool collision_dectection(void);

#endif
