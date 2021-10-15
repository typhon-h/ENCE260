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
#include "led.h"

#define GAME_MODE_PROMPT    " SELECT GAME MODE "
#define END_PROMPT          " GAME OVER SCORE:"  //Additional whitespace to insert score
#define END_PROMPT_LEN      17
#define SIZE_OF_UINT8       8                    //For buffer on end message for score

#define MENU_TONE           "A"

char GAME_MUSIC[] =   //Music to loop during gameplay
{
#include "sounds/megalovania.mmel"
         " :"           // Loop infinitely
};

char END_GAME_MUSIC[] = //End menu music
{
#include "sounds/rick_roll.mmel"
         " :" // Loop indefinitely
};


bool    ACTIVE_GAME      = false;
uint8_t SCORE            = 0;        // has a max of 255
uint8_t GAME_MODE_index  = 0;
uint8_t WALL_RANDOM_SEED = 0;
bool    PAUSE_STATUS     = false;

char *GAMEMODE_STRINGS[] =
{
   HARD_MODE_TEXT,
   THREE_LIVES_TEXT,
   WALL_PUSH_TEXT
};


/*  Initialize game manager, LED and starts game menu
 *  @param message_rate: text scroll speed of MENU texts
 *  @brief: using 3x5 font, displays " SELECT GAME MODE "
 */
void game_init(uint8_t message_rate)
{
   button_init();
   led_init();
   led_set(LED1, 0);
   tinygl_text_speed_set(message_rate);
   tinygl_font_set(&font3x5_1);
   tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
   tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
   tinygl_text(GAME_MODE_PROMPT);
}


/*  Return current state of game, increments WALL_RANDOM_SEED
 *  @brief: number of calls to function (during menus) depends of time spent on menus
 *          ensures that random seed for wall_init() is always different
 */
bool get_game_state()
{
   WALL_RANDOM_SEED++; // Increments global variable WALL_RANDOM_SEED
   return ACTIVE_GAME;
}


/*  Returns the pause state of the game
 */
bool get_pause_state()
{
   return PAUSE_STATUS;
}


/*  Increments game score
 *  @brief: as there is a single moving wall, this is called ervery time the wall resets
 */
void increment_score()
{
   SCORE++;
}


/*  Checks for button input to pause game
 *  @brief: button only pauses game if the game is active
 *          MENU_TONE is playedduring paused state
 */
void check_pause_button(void)
{
   button_update(); // Update button input
   if (button_push_event_p(0) & ACTIVE_GAME) // if button is pressed AND game is active
   {
      PAUSE_STATUS = !PAUSE_STATUS; // Toggles pause state each press
      led_set(LED1, PAUSE_STATUS);  // If paused, LED lights up
      if (PAUSE_STATUS)
      {
         sound_play(MENU_TONE);     // If paused, MENU_TONE is played
      }
      else
      {
         sound_play(GAME_MUSIC);    // if not paused, GAME_MUSIC is played
      }
   }
}



/*  Initialize game components when starting
 *  @brief: button only pauses game if the game is active
 *          MENU_TONE is playedduring paused state
 */
void game_start()
{
   uint8_t player_lives;

   switch ((GAMEMODES_t)GAME_MODE_index)
   {
   case THREE_LIVES:
      player_lives = 3;       // Can collide with wall up to 3 times
      break;

   case HARD_MODE:
      player_lives = 1;       // Instant death upon collision
      break;

   case WALL_PUSH:
      player_lives = 1;       // Death instant for OUT_OF_BOUNDS
      break;

   default:
      player_lives = 3;   // Game will default to three_lives mode (if index > 3)
   }

   tinygl_clear();                // Clear display
   character_init(player_lives);  // Initialise character module (with given lives)
   wall_init(WALL_RANDOM_SEED);   // Initialises wall module with random seed

   sound_play(GAME_MUSIC);        // PLays game music
   SCORE       = 0;               // Reset gamescore (from previous games)
   ACTIVE_GAME = true;            // game is started so ACTIVE_GAME is true
}


/*  Updates game states
 *  @brief: button only pauses game if the game is active
 *          MENU_TONE is played during paused state
 */
void game_state_update()
{
   static bool gamemode_selected = false;
   static bool game_end          = false;

   navswitch_update();

   // If navswitch is pressed, game option menu scrolls down
   if (navswitch_push_event_p(NAVSWITCH_PUSH) && !game_end)
   {
      tinygl_clear();
      GAME_MODE_index = (GAME_MODE_index + 1) % DIFFERENT_GAMEMODES;   // Update GAMEMODE_index (currently selected)
      tinygl_text(GAMEMODE_STRINGS[GAME_MODE_index]);                  // Display different gamemode text
      gamemode_selected = true;
      sound_play(MENU_TONE);
   }

   button_update(); // Update button input

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


/*  Decreases player lives
 *  @brief: decrease_character_lives() decreases lives and return true is lives = 0
 *          if lives = 0, game enters ending state
 */
void decrease_lives(void)
{
   if (decrease_character_lives())
   {
      ACTIVE_GAME = false;
      toggle_wall(false);
      character_disable();
      game_outro();
   }
}



/*  Outlines the outcome of a collsion (decided by each game_mode)
 *  @brief: HARDMODE    = dcreases lives (only 1 life so instant death)
 *          THREE_LIVES = dcreases lives (from which player has 3)
 *          WALL_PUSH   = "pushes" by moving player in direction of wall momvement
 */
void gamemode_collsion_process(void)
{
   bool player_at_border; // boolean describing if player is pushed beyond border

   switch ((GAMEMODES_t)GAME_MODE_index)
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
      	 // Since Wall is ROW, wall is moving either NORTH or SOUTH
         player_at_border = (get_active_wall().direction == NORTH) ? move_north(): move_south();
      }
      else // Since Wall is COLUMN, wall is moving either EAST or WEST
      {
         player_at_border = (get_active_wall().direction == EAST) ? move_east(): move_west();
      }

      /* Re-display the part of wall which collided with player  */
      /*   (which disappears after player is moved)              */
      toggle_wall(1);

      if (player_at_border)
      {
      	 // Kills player if pushed beyond the wall
         decrease_lives();
      }
      break;
   }
}


/*  Checks to see if a "collision" has aoccured
 *  @brief: As mentioned, stun was introduced so player doesn't have multiple loss of lives from single collision
 *		Function only returns true if collsion & not stunned player cannot move whilst stunned
 *		(stun is removed after wall moves away, prevent player moving into another part of wall)
 */
void check_collisions()
{
   if (get_game_state())
   {
      if (collision_dectection() & !get_stun_condition()) // If collision and not stunned
      {
         gamemode_collsion_process(); // What occurs during a collision
      }
   }
}


/*  Outlines process of a game_ending (text display, music played)
 *  @brief: Displays SCORE and plays ending music END_GAME_MUSIC
 */
void game_outro(void)
{
   char end_message[END_PROMPT_LEN + SIZE_OF_UINT8] = END_PROMPT;

   uint8toa(SCORE, end_message + END_PROMPT_LEN, false);
   tinygl_text(end_message);
   sound_play(END_GAME_MUSIC);
}


/*  Checks whether wall has collided with player
 *  @brief: For a collsion to occur, player and ACTIVE_WALL must have
 *          same position AND player must not be inline with hole.
 *          USed in function check_collisions()
 */
bool collision_dectection()
{
   CharacterInfoStruct character_info = get_character_info();
   WallStruct     wall = get_active_wall();

   // Checks whether player is inline with hole in the wall
   uint8_t player_bitmap       = (wall.wall_type == ROW) ? BIT(character_info.x): BIT(character_info.y);
   bool    potential_collision = (wall.bit_data & player_bitmap) != 0;

   // Checks whether the player is on the ACTIVE_WALL position
   uint8_t player_index     = (wall.wall_type == ROW) ? character_info.y: character_info.x;
   bool    is_same_position = player_index == wall.pos;

   return (potential_collision & is_same_position);
}
