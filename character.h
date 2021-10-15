/** @file   character.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "system.h"

//Character movement restrictions
#define NORTH_CHARACTER_BOUNDARY    0 // Top Row
#define EAST_CHARACTER_BOUNDARY     4 // Right Column
#define SOUTH_CHARACTER_BOUNDARY    6 // Bottom Row
#define WEST_CHARACTER_BOUNDARY     0 // Left Column

//Default starting coordinates
#define DEFAULT_X                   2
#define DEFAULT_Y                   3


// Character information (lives and position)
typedef struct
{
	uint8_t x, y;
	uint8_t lives;
	bool    is_stunned;
} CharacterInfoStruct;


/* Initialisation for character module
 * @param life_count: creates player with respective number of lives
 * @brief: Character creation, spawns character at default coordinates
 */
void character_init(uint8_t life_count);


/*  Return current character information
 *  @return CharacterInfoStruct with coordinates and number of lives
 */
CharacterInfoStruct get_character_info(void);


/*  Returns the stun state of the player
 *  @return 1 if stunned else 0
 */
bool get_stun_condition(void);


/*  Changes the stun state of the player
 *  @param stun_set: bool variable to which character_stun is set
 */
void toggle_stun(bool stun_set);


/* Reduces character lives by 1
 *  @return: returns 1 if no lives left else 0
 */
bool decrease_character_lives(void);


/* Turns off character
 */
void character_disable(void);


/*  Move character 1 unit in cardinal direction
 *  @return 1 if character moving off boundary else 0
 */
bool move_west(void);
bool move_east(void);
bool move_north(void);
bool move_south(void);


/* Poll navswitch input and move character
 *  @brief: Doesn't allow movement is player is stunned
 */
void character_update(void);

#endif
