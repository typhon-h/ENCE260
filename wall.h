/** @file   wall.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */
 
#ifndef WALL_H
#define WALL_H

#include "display.h"
#include "character.h"

#define ONE_HOLE(X) (~(1<<(X)))
#define TWO_HOLE(X) (~(3<<(X)))
#define THREE_HOLE(X) (~(7<<(X))) 

#define MAX_COL_WALLS 2
#define MAX_ROW_WALLS 2


typedef enum {
	NORTH = -1, 
	EAST = 1, 
	SOUTH = 1, 
	WEST = -1,
    OUT_OF_BOUNDS = 0
} direction;

typedef struct {
	uint8_t bit_data;  //0b 11000111 -- (1= Non-passible, 0=Passible)
	uint8_t pos;	   //0, 1, 2, 3, ... 
	direction dir;          // direction is enum, NORTH, EAST, SOUTH, WEST     
} Wall_t; // Wall object 

#define UP_COL_WALL( X ) {(X),(0), (EAST)}
#define DOWN_COL_WALL(X) {(X), (4), (WEST)}
#define UP_ROW_WALL(X) {(X), (6), (NORTH)}
#define DOWN_ROW_WALL(X) {(X), (0), (SOUTH)}


void wall_create(void);
void toggle_wall(Wall_t col_wall, bool display_on);
void move_col_walls(position_t character_pos);

#endif
