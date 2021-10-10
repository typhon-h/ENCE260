/** @file   character.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "system.h"

//Character movement restrictions
#define WEST_CHARACTER_BOUNDARY     0 //Column
#define EAST_CHARACTER_BOUNDARY     4 //Column
#define NORTH_CHARACTER_BOUNDARY    0 //Row
#define SOUTH_CHARACTER_BOUNDARY    6 //Row

//Default starting coordinates
#define DEFAULT_X                   2
#define DEFAULT_Y                   3

// Position coordinates structure
typedef struct
{
   uint8_t x, y;
} Position_t;

//Create the character at the default starting position
void character_init(void);

//Turn off character
void character_disable(void);

//Return current position of the character
Position_t get_character_pos(void);

//Move the character in the given direction, restricted by boundary
void move_west(void);
void move_east(void);
void move_north(void);
void move_south(void);

//Poll navswitch input and update character position
void character_update(void);

#endif
