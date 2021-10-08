/** @file   wall.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#ifndef WALL_H
#define WALL_H

#include "system.h"

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
#define EAST_MOVING_WALL(X)     { (X), (0), (EAST), (EAST_WALL_BOUNDARY) }   //Move wall right
#define WEST_MOVING_WALL(X)     { (X), (4), (WEST), (EAST_WALL_BOUNDARY) }   //Move wall left
#define NORTH_MOVING_WALL(X)    { (X), (6), (NORTH), (SOUTH_WALL_BOUNDARY) } //Move wall up
#define SOUTH_MOVING_WALL(X)    { (X), (0), (SOUTH), (SOUTH_WALL_BOUNDARY) } //Move wall down

// Enum for increment direction of walls
typedef enum
{
   EAST          = 1,
   SOUTH,
   WEST,
   NORTH,
   OUT_OF_BOUNDS = 0
} direction;
// As they had shaaring values had to change for switch statement

// Type definition for a wall
typedef struct
{
   uint8_t   bit_data;      // 0b11000111 -- (1= Non-passible, 0=Passible)
   uint8_t   pos;           // 0, 1, 2, 3, ...
   direction dir;           // direction is enum, NORTH, EAST, SOUTH, WEST
   uint8_t   boundary_cond; // If pos>coundary_cond for wall deletion
} Wall_t;                   // Wall object

// Set seed for wall generation
void wall_init(uint16_t seed);

// Create a wall and spawn it in starting position
void wall_create(void);

// Toggle wall visibilty on display
void toggle_wall(bool display_on);

// Shift wall in increment direction (contained within the wall)
void move_wall(void);

//Update current wall position or create new wall
void wall_update(void);

#endif
