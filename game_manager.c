#include "system.h"
#include "game_manager.h"
#include "character.h"
#include "wall.h"
#include "button.h"

bool ACTIVE_GAME = false;

// Initialize game manager
void game_init()
{
   button_init();
}

// Return current state of game (is it being played)
bool get_game_state()
{
   return ACTIVE_GAME;
}

// Initialize game components and toggle game state
void game_start(uint16_t seed)
{
   character_init();
   wall_init(seed);

   ACTIVE_GAME = true;
}

// Poll button input to start game
void game_state_update(uint16_t seed)
{
   button_update();

   if (button_push_event_p(0))
   {
      game_start(seed);
   }
}

void game_outro(void)
{
	// Right now 
	// in future can display score / level
	// maybe some epic music
}
