/** @file   game_manager.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   10 Oct 2021
 *  @brief  Controls key game components:
 *          Menus
 *          Game Start/Stop
 *          Key events
 */

#include "system.h"
#include "game_manager.h"
#include "character.h"
#include "wall.h"
#include "button.h"

#include "tinygl.h"
#include "../fonts/font5x7_1.h"


bool ACTIVE_GAME = false;

// Initialize game manager
void game_init()
{
   button_init();
   tinygl_font_set(&font5x7_1);
   tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
   tinygl_text("press to start ");
}


// Return current state of game (is it being played)
bool get_game_state()
{
   return ACTIVE_GAME;
}


// Initialize game components and toggle game state
void game_start(uint16_t seed)
{
   tinygl_clear();
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


//Disable game elements and toggle game state
void game_end(void)
{
   ACTIVE_GAME = false;
   toggle_wall(false);
   character_disable();
   game_outro();
}


void game_outro(void)
{
   tinygl_text("game over ");
   // Right now
   // in future can display score / level
   // maybe some epic music
}


/*  Checks whether wall has collided with player
 *  @brief: For a collsion to occur, player and ACTIVE_WALL must have
 *                      same position AND player must not be inline with hole.
 *          Used to end the game.
 */
bool collision_dectection()
{
   Position_t character_position = get_character_pos();
   Wall_t     wall = get_active_wall();

   // Checks whether player is inline with hole in the wall
   uint8_t player_bitmap       = (wall.wall_type == ROW) ? BIT(character_position.x): BIT(character_position.y);
   bool    potential_collision = (wall.bit_data & player_bitmap) != 0;

   // Checks whether the player is on the ACTIVE_WALL position
   uint8_t player_index     = (wall.wall_type == ROW) ? character_position.y: character_position.x;
   bool    is_same_position = player_index == wall.pos;

   return(potential_collision & is_same_position);
}
