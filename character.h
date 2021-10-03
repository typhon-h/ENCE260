/** @file   character.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
#ifndef CHARACTER_H
#define CHARACTER_H

#include "display.h"

//Character movement restrictions
#define LEFT_BOUNDARY     1 //Column
#define RIGHT_BOUNDARY    3 //Column
#define UP_BOUNDARY       1 //Row
#define DOWN_BOUNDARY     5 //Row

//Default starting coordinates
#define DEFAULT_X         2
#define DEFAULT_Y         3

struct position_s
{
   uint8_t x, y;
};

//Create the character at the default starting position
void character_init(void);

//Move the character in the given direction, restricted by boundary
void move_left(void);
void move_right(void);
void move_up(void);
void move_down(void);

//Poll navswitch input and update character position
void character_update(void);

#endif
