/** @file   game_manager.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   10 Oct 2021
 *  @brief  Controls key game components:
 *          Menus and game modes
 *          Game Start/Stop/Pause
 *          Key events eg. collision, lives, scoring
 */

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "system.h"

// Menu Constants
#define GAME_MODE_PROMPT       " SELECT GAMEMODE "
#define END_PROMPT             " GAME OVER SCORE:" //Additional whitespace to insert score
#define END_PROMPT_LEN         17
#define SIZE_OF_UINT8          8                   //For buffer on end message for score
#define MENU_TONE              "A"
// Menu text for each gamemode (for displaying)
#define HARD_MODE_TEXT         " HARDMODE "
#define THREE_LIVES_TEXT       " THREE LIVES "
#define WALL_PUSH_TEXT         " WALL PUSH "
#define DIFFERENT_GAMEMODES    3



// Enum containing all different gamemodes
typedef enum
{
	HARD_MODE = 0,
	THREE_LIVES,
	WALL_PUSH
} GAMEMODES_t;


// Enum containing different game states
typedef enum
{
	MENU_STATE,              // Adding so push button doesnt select gamemode without title
	SELECTION_STATE,         // Menu state
	GAME_PLAY_STATE,         // Actual gameplay state
	GAME_END_STATE           // Game ending
} GAMESTATES_t;



/*  Initialize game manager, LED and starts game menu
 *  @param message_rate: text scroll speed of MENU texts
 *  @brief: using 3x5 font, displays " SELECT GAME MODE "
 */
void game_init(uint8_t message_rate);


/*  Return current state of game, increments wall_random_seed
 *  @brief: number of calls to function (during menus) depends of time spent on menus
 *          ensures that random seed for wall_init() is always different
 */
bool get_game_state(void);


/*  Returns the pause state of the game
 *  @return true if the game is paused else false */
bool get_pause_state(void);


/*  Checks for button input to pause game
 *  @brief: button only pauses the game if it's active.
 *          MENU_TONE is played during paused state */
void check_pause_button(void);


/*  Updates game states
 *  @brief: checks navswitch to scroll through menu (if in menu).
 *          checks for button push to either redo game or select gamemode. */
void game_state_update(void);


/*  Initialize game components when starting
 *  @brief: gives player lives depending on game mode
 *          initialises all modules used in game */
void game_start(void);


/*  Outlines process of a game_ending (text display, music played)
*  @brief: Displays score and plays ending music END_GAME_MUSIC */
void game_outro(void);


/*  Checks to see if a "collision" has occured
 *  @brief: As mentioned, stun was introduced so player doesn't have multiple loss of lives from single collision
 *		Function only returns true if collsion & not stunned player cannot move whilst stunned
 *		(stun is removed after wall moves away, prevent player moving into another part of wall) */
void check_collisions(void);


/*  Decreases player lives
 *  @brief: decrease_character_lives() decreases lives and return true is lives = 0
 *          if lives = 0, game enters ending state */
void decrease_lives(void);


/*  Increments game score
 *  @brief: as there is a single moving wall, this is called ervery time the wall resets */
void increment_score(void);


#endif
