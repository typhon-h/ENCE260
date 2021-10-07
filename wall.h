/** @file   wall.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#ifndef WALL_H
#define WALL_H

#include "display.h"

/*  Create wall bitmap with a hole
 *  @param SIZE size of the whole in pixels //binary 0b00000001 to 0b00001111
 *  @param SHIFT position of the whole within the wall // integer 0 to 7
 */
#define GENERATE_HOLE(SIZE, SHIFT)    (~(SIZE << (SHIFT)))

//Wall movement restrictions
#define NORTH_WALL_BOUNDARY    0
#define EAST_WALL_BOUNDARY     4
#define SOUTH_WALL_BOUNDARY    6
#define WEST_WALL_BOUNDARY     0

// Wall movement patterns
#define UP_COL_WALL(X)      { (X), (0), (EAST) }  //Move wall right
#define DOWN_COL_WALL(X)    { (X), (4), (WEST) }  //Move wall left
#define UP_ROW_WALL(X)      { (X), (6), (NORTH) } //Move wall up
#define DOWN_ROW_WALL(X)    { (X), (0), (SOUTH) } //Move wall down

// Enum for increment direction of walls
typedef enum
{
   NORTH         = -1,
   EAST          = 1,
   SOUTH         = 1,
   WEST          = -1,
   OUT_OF_BOUNDS = 0
} direction;


// Type definition for a wall
typedef struct
{
   uint8_t   bit_data; //0b 11000111 -- (1= Non-passible, 0=Passible)
   uint8_t   pos;      //0, 1, 2, 3, ...
   direction dir;      // direction is enum, NORTH, EAST, SOUTH, WEST
} Wall_t;              // Wall object


// Create a wall and spawn it in starting position
void wall_create(void);

// Toggle wall visibilty on display
void toggle_wall(Wall_t col_wall, bool display_on);

// Shift wall in increment direction (contained within the wall)
void move_wall(void);

#endif
