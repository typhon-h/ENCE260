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
#include "../fonts/font3x5_1.h"
#include "uint8toa.h"

#define START_PROMPT      " PRESS TO START "
#define END_PROMPT        " GAME OVER SCORE:" //Additional whitespace to insert score
#define END_PROMPT_LEN    17
#define SIZE_OF_UINT8     8                   //For buffer on end message for score

bool    ACTIVE_GAME = false;
uint8_t SCORE       = 0;

// Initialize game manager
void game_init(uint8_t message_rate)
{
   button_init();
   tinygl_text_speed_set(message_rate);
   tinygl_font_set(&font3x5_1);
   tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
   tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
   tinygl_text(START_PROMPT);
}


// Return current state of game (is it being played)
bool get_game_state()
{
   return ACTIVE_GAME;
}


//Increment score
void increment_score()
{
   SCORE++;
}


// Initialize game components and toggle game state
void game_start()
{
   tinygl_clear();
   character_init();
   wall_init();
   SCORE = 0;

   ACTIVE_GAME = true;
}


// Poll button input to start game
void game_state_update()
{
   button_update();

   if (button_push_event_p(0))
   {
      game_start();
   }
}


void check_game_end(void)
{
   if (get_game_state())
   {
      if (collision_dectection())                           // Reset game variables
      {
         game_end();
      }
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
   char end_message[END_PROMPT_LEN + SIZE_OF_UINT8] = END_PROMPT;

   uint8toa(SCORE, end_message + END_PROMPT_LEN, false);
   tinygl_text(end_message);
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
