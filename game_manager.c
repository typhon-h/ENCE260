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
#include "navswitch.h"

#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "uint8toa.h"

#include "sound.h"

#define GAME_MODE_PROMPT    " SELECT GAME MODE "
#define END_PROMPT          " GAME OVER SCORE:"  //Additional whitespace to insert score
#define END_PROMPT_LEN      17
#define SIZE_OF_UINT8       8                    //For buffer on end message for score


#define MENU_TONE "D#"
char GAME_MUSIC[] = { //Music to loop during gameplay
    #include "sounds/megalovania.mmel"
    " :" // Loop infinitely
};
char END_GAME_MUSIC[] = { //End menu music
    #include "sounds/rick_roll.mmel"
    " :" // Loop infinitely
};


bool    ACTIVE_GAME     = false;
uint8_t SCORE           = 0;         // has a max of 255
uint8_t GAME_MODE_index = 0;

char *GAMEMODE_STRINGS[] =
{
   HARD_MODE_TEXT,
   THREE_LIVES_TEXT,
   WALL_PUSH_TEXT
};


// Initialize game manager, starts game menu
void game_init(uint8_t message_rate)
{
   button_init();
   tinygl_text_speed_set(message_rate);
   tinygl_font_set(&font3x5_1);
   tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
   tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
   tinygl_text(GAME_MODE_PROMPT);
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
   uint8_t player_lives;

   switch ((gamemode_t)GAME_MODE_index)
   {
   case THREE_LIVES:
      player_lives = 3;
      break;

   case HARD_MODE:
      player_lives = 1;
      break;

   case WALL_PUSH:
      player_lives = 1;           //Death intant for OUT_OF_BOUNDS
      break;

   default:
      player_lives = 3;   //Defaults to Three-lives
   }

   tinygl_clear();
   character_init(player_lives);
   wall_init();
   sound_play(GAME_MUSIC);
   SCORE = 0;
   ACTIVE_GAME = true;
}


// Poll button input to start game
void game_state_update()
{
   static bool gamemode_selected = false;
   static bool game_end          = false;

   navswitch_update();

   // If navswitch is pressed, game option menu scrolls down
   if (navswitch_push_event_p(NAVSWITCH_PUSH) && !game_end)
   {
      tinygl_clear();
      GAME_MODE_index = (GAME_MODE_index + 1) % DIFFERENT_GAMEMODES;
      tinygl_text(GAMEMODE_STRINGS[GAME_MODE_index]);
      gamemode_selected = true;
      sound_play(MENU_TONE);
   }

   button_update();

   if (button_push_event_p(0))
   {
      if (game_end) //Show mode menu if pressed at end of game
      {
         tinygl_clear();
         tinygl_text(GAME_MODE_PROMPT);
         game_end = false;
         sound_play(MENU_TONE);
      }
      else if (gamemode_selected)
      {
         // "Game starts when gamemode is selected and button pushed"
         gamemode_selected = false; //Game mode menu reappears on restart
         game_end          = true;  //Next menu will be end of game
         sound_play(MENU_TONE);
         game_start();

      }
   }
}


// if lives = 0, Disable game elements and toggle game state
void decrease_lives(void)
{
   if (decrease_player_lives())
   {
      ACTIVE_GAME = false;
      toggle_wall(false);
      character_disable();
      game_outro();
   }
}


/* Each gammode has different outcomes during collsions*/
void gamemode_collsion_process(void)
{
   bool player_at_border;

   switch ((gamemode_t)GAME_MODE_index)
   {
   case HARD_MODE:
   case THREE_LIVES:
      // Will simply reduce the number of lives by 1
      // stun is introduced to prevent multiple loss of lives from a single collision
      decrease_lives();
      toggle_stun(true);           // Automatically is unstun when wall moves again
      break;

   case WALL_PUSH:

      // Will move character in direction of wall movement)
      if (get_active_wall().wall_type == ROW)
      {
         player_at_border = (get_active_wall().direction == NORTH) ? move_north(): move_south();
      }
      else
      {
         player_at_border = (get_active_wall().direction == EAST) ? move_east(): move_west();
      }
      toggle_wall(1);           // Re-display the part of wall which collided with player
      // (which disappears after player is moved)
      if (player_at_border)
      {
         decrease_lives();
      }
      break;
   }
}


void check_collisions()
{
   if (get_game_state())
   {
      if (collision_dectection() & !get_stun_condition())
      {
         gamemode_collsion_process();
      }
   }
}


void game_outro(void)
{
   char end_message[END_PROMPT_LEN + SIZE_OF_UINT8] = END_PROMPT;

   uint8toa(SCORE, end_message + END_PROMPT_LEN, false);
   tinygl_text(end_message);
   sound_play(END_GAME_MUSIC);
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
