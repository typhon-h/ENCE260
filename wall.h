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

// Wall movement restrictions
#define NORTH_WALL_BOUNDARY    0
#define EAST_WALL_BOUNDARY     4
#define SOUTH_WALL_BOUNDARY    6
#define WEST_WALL_BOUNDARY     0

/* Initialisation MACROs for each wall type 
 * Each entry represents starting state of each wall type
 *    X ~ wall bit_data 
*/ 
#define EAST_MOVING_WALL(X)     { (X), (0), (EAST_WALL_BOUNDARY), (COLUMN), (EAST) } // Initial EAST wall
#define WEST_MOVING_WALL(X)     { (X), (4), (EAST_WALL_BOUNDARY), (COLUMN), (WEST) } // Initial WEST wall
#define NORTH_MOVING_WALL(X)    { (X), (6), (SOUTH_WALL_BOUNDARY), (ROW), (NORTH) }  // Initial NORTH wall
#define SOUTH_MOVING_WALL(X)    { (X), (0), (SOUTH_WALL_BOUNDARY), (ROW), (SOUTH) }  // Initial SOUTH wall


// declaration of all possible wall movement directions
typedef enum
{
   NORTH = 1,
   SOUTH,
   WEST,
   EAST
} WALL_DIRECTION;


// WALL_TYPE defiinition, wall is either along a COLUMN, ROW or it's OUT_OF_BOUNDS
typedef enum
{
   COLUMN, ROW, OUT_OF_BOUNDS  
} WALL_TYPE;


/* structure containing aspects of moving wall
*/ 
typedef struct
{
   uint8_t         bit_data;      // 0b11000111 -- (1= Non-passible, 0=Passible)
   uint8_t         pos;           // 0, 1, 2, 3, ...
   uint8_t         boundary_cond; // If pos>coundary_cond for wall deletion
   WALL_TYPE       wall_type;     // WALL_TYPE are COLUMN and ROW
   WALL_DIRECTION  direction;     // The direction of movement of wall

} WallStruct;       // Wall object


// Set random seed for wall generation, reset global ACTIVE_WALL
void wall_init(uint8_t initial_seed);

//Return ACTIVE_WALL information as WallStruct type
WallStruct get_active_wall(void);

// Reset and randomise the global WallStruct ACTIVE_WALL
void wall_create(void);

// Toggle wall visibilty on display
void toggle_wall(bool display_on);

// Move wall in it's direction (contained within the WallStruct)
void move_wall(void);

#endif
