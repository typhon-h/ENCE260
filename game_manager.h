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





#endif
