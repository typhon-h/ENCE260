/** @file   wall.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#ifndef WALL_H
#define WALL_H

#include "system.h"
#include "character.h"

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

// Initialisation MACROs for each wall type
#define EAST_MOVING_WALL(X)     {(X), (0), (EAST_WALL_BOUNDARY), (COLUMN), (EAST)}   //Move wall right
#define WEST_MOVING_WALL(X)     {(X), (4), (EAST_WALL_BOUNDARY), (COLUMN), (WEST)}   //Move wall left
#define NORTH_MOVING_WALL(X)    {(X), (6), (SOUTH_WALL_BOUNDARY), (ROW), (NORTH)} //Move wall up
#define SOUTH_MOVING_WALL(X)    {(X), (0), (SOUTH_WALL_BOUNDARY), (ROW), (SOUTH)} //Move wall down


// ENUM representing wall movement direction
typedef enum
{
   NORTH = 1,
   SOUTH = 2,
   WEST  = 3,
   EAST  = 4
} WALL_DIRECTION;

// Represents wll_type (i.e. if its displayed along a coplumn or row)
typedef enum {COLUMN, ROW, OUT_OF_BOUNDS} WALL_TYPE;


// Type definition for a wall
typedef struct
{
   uint8_t        bit_data;      // 0b11000111 -- (1= Non-passible, 0=Passible)
   uint8_t        pos;           // 0, 1, 2, 3, ...
   uint8_t        boundary_cond; // If pos>coundary_cond for wall deletion
   WALL_TYPE      wall_type;     // WALL_TYPE are COLUMN and ROW
   WALL_DIRECTION direction;      // The direction of movement of wall
} Wall_t;                        // Wall object

// Set seed for wall generation
void wall_init(uint16_t seed);

// Create a wall and spawn it in starting position
void wall_create(void);

// Returns collsion statusw when given a position
bool collision_dectection(position_t position);

// Toggle wall visibilty on display
void toggle_wall(bool display_on);

// Shift wall in increment direction (contained within the wall)
void move_wall(void);

//Update current wall position or create new wall
void wall_update(void);

#endif
