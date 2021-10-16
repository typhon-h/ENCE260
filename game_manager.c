/** @file   game_manager.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   10 Oct 2021
 *  @brief  Controls key game components:
 *          Menus and game modes
 *          Game Start/Stop/Pause
 *          Key events eg. collision, lives, scoring
 */

#include "system.h"
#include "game_manager.h"
#include "character.h"
#include "wall.h"
#include "sound.h"
#include "button.h"
#include "navswitch.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "uint8toa.h"
#include "led.h"


static char GAME_MUSIC[] =   // Music to loop during gameplay
{
#include "sounds/megalovania.mmel"
         " :"           // Loop indefinitely
};

static char END_GAME_MUSIC[] = // End menu music
{
#include "sounds/rick_roll.mmel"
         " :" // Loop indefinitely
};


// Game Constants
static GAMESTATES_t active_game = MENU_STATE;
static uint8_t score            = 0;
static uint8_t game_mode_index  = 0;
static bool    pause_status     = false;
uint8_t wall_random_seed = 0;


char *GAMEMODE_STRINGS[] = // Gamemode prompts
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


/*  Return current state of game, increments wall_random_seed
 *  @brief: number of calls to function (during menus) depends of time spent on menus
 *          ensures that random seed for wall_init() is always different
 */
bool get_game_state()
{
   wall_random_seed++; // Increments variable wall_random_seed
   return (active_game == GAME_PLAY_STATE);
}


/*  Returns the pause state of the game
 *  @return true if the game is paused else false
 */
bool get_pause_state()
{
   return pause_status;
}


/*  Checks for button input to pause game
 *  @brief: button only pauses the game if it's active.
 *          MENU_TONE is played during paused state
 */
void check_pause_button()
{
   button_update(); // Update button input
   if (button_push_event_p(0) & active_game) // if button is pressed AND game is active
   {
      pause_status = !pause_status; // Toggles pause state each press
      led_set(LED1, pause_status);  // If paused, LED lights up
      if (pause_status)
      {
         sound_play(MENU_TONE);     // If paused, MENU_TONE is played
      }
      else
      {
         sound_play(GAME_MUSIC);    // if not paused, GAME_MUSIC is played
      }
   }
}


/*  Updates game states
 *  @brief: checks navswitch to scroll through menu (if in menu).
 *          checks for button push to either redo game or select gamemode.
 */
void game_state_update()
{
	navswitch_update();
	button_update();

	switch (active_game)
	{
	case MENU_STATE: 
		// If any input, gamemode is displayed and game state if SELECTION
		if (navswitch_push_event_p(NAVSWITCH_PUSH) | button_push_event_p(0)) 
		{
            sound_play(MENU_TONE);
			active_game = SELECTION_STATE;
      		tinygl_clear();
			tinygl_text(GAMEMODE_STRINGS[game_mode_index]);
		}
		break;

	case SELECTION_STATE:
	
		if (navswitch_push_event_p(NAVSWITCH_PUSH)) // Change game mode
		{
      		tinygl_clear();
      		game_mode_index = (game_mode_index + 1) % DIFFERENT_GAMEMODES;   // Update GAMEMODE_index (currently selected)
      		tinygl_text(GAMEMODE_STRINGS[game_mode_index]);                  // Display different gamemode text
            sound_play(MENU_TONE);
		}

		if (button_push_event_p(0)) // Start game
		{
       		active_game = GAME_PLAY_STATE;
       		game_start();
		}
		break;

	case GAME_END_STATE: // Return to menu
		if (button_push_event_p(0))
		{
	    	tinygl_clear();
         	tinygl_text(GAME_MODE_PROMPT);
          	sound_play(MENU_TONE);
          	active_game = MENU_STATE;
		}

		break;

    case GAME_PLAY_STATE: // Buttons disabled
    default:
        break;
	}
}


/*  Initialize game components when starting
 *  @brief: gives player lives depending on game mode
 *          initialises all modules used in game
 */
void game_start()
{
   uint8_t player_lives;

   switch ( (GAMEMODES_t)game_mode_index) // Set player lives based on game mode
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
   wall_init(wall_random_seed);   // Initialises wall module with random seed

   sound_play(GAME_MUSIC);        // Plays game music
   score       = 0;               // Reset gamescore (from previous game)
   active_game = GAME_PLAY_STATE;            // game is started so playing state
}


/*  Outlines process of a game_ending (text display, music played)
 *  @brief: Displays score and plays ending music END_GAME_MUSIC
 */
void game_outro()
{
   char end_message[END_PROMPT_LEN + SIZE_OF_UINT8] = END_PROMPT;

   uint8toa(score, end_message + END_PROMPT_LEN, false);
   tinygl_text(end_message);
   sound_play(END_GAME_MUSIC);
}


/*  Checks whether wall has collided with player
 *  @brief: For a collsion to occur, player and ACTIVE_WALL must have
 *          same position AND player must overlap with wall.
 *          Used in function check_collisions()
 */
static bool collision_dectection(void)
{
   CharacterInfoStruct character_info = get_character_info();
   WallStruct     wall = get_active_wall();

   // Checks whether player overlaps the wall, by creating bitmap of character (e.g. 00000100)
   // If (character bitmap) & (wall bitmap) is not zero, then player overlaps with wall_bitmap
   uint8_t player_bitmap       = (wall.wall_type == ROW) ? BIT(character_info.x): BIT(character_info.y);
   bool    potential_collision = (wall.bit_data & player_bitmap) != 0;

   // Checks whether the player is inline with the ACTIVE_WALL position
   // If ROW wall_type, then wall position is compared to the characters y coord, else x coord
   uint8_t player_index     = (wall.wall_type == ROW) ? character_info.y: character_info.x;
   bool    is_same_position = player_index == wall.pos;

   return (potential_collision & is_same_position);
}


/*  Outlines the process of a collsion (decided by each gamemode)
 *  @brief:    HARDMODE = dcreases lives (only 1 life so instant death)
 *             THREE_LIVES = dcreases lives (from which player has 3)
 *             WALL_PUSH = "pushes" by moving player in direction of wall momvement
 */
static void gamemode_collsion_process(void)
{
   bool player_at_border; // boolean describing if player is pushed beyond border

   switch ((GAMEMODES_t)game_mode_index)
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


/*  Checks to see if a "collision" has occured
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



/*  Decreases player lives
 *  @brief: decrease_character_lives() decreases lives and return true is lives = 0
 *          if lives = 0, game enters ending state
 */
void decrease_lives()
{
   if (decrease_character_lives())
   {
      active_game = GAME_END_STATE;
      toggle_wall(false);
      character_disable();
      game_outro();
   }
}


/*  Increments game score
 *  @brief: as there is a single moving wall, this is called ervery time the wall resets
 */
void increment_score()
{
   score++;
}
