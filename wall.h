/** @file   wall.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#ifndef WALL_H
#define WALL_H

#include "system.h"

/*  Create wall bitmap with a hole
 *  @param SIZE size of the whole in pixels eg. binary 0b00000001 to 0b00001111
 *  @param SHIFT position of the whole within the wall eg. integer 0 to 7 for column
 */
#define GENERATE_HOLE(SIZE, SHIFT)    (~(SIZE << (SHIFT)))

// Wall movement restrictions
#define NORTH_WALL_BOUNDARY    0
#define EAST_WALL_BOUNDARY     4
#define SOUTH_WALL_BOUNDARY    6
#define WEST_WALL_BOUNDARY     0

// Wall generation constants
#define NUM_OF_DIRECTIONS      4
#define MAX_HOLE_SIZE          3
#define ROW_SIZE               LEDMAT_COLS_NUM
#define COLUMN_SIZE            LEDMAT_ROWS_NUM

/* Initialisation MACROs for each wall type
 * Each entry represents starting state of each wall type
 * @param bitmap for the wall
 */
#define EAST_MOVING_WALL(WALL_BIT_DATA)     { (WALL_BIT_DATA), (0), (EAST_WALL_BOUNDARY), (COLUMN), (EAST) } // Initial EAST wall
#define WEST_MOVING_WALL(WALL_BIT_DATA)     { (WALL_BIT_DATA), (4), (EAST_WALL_BOUNDARY), (COLUMN), (WEST) } // Initial WEST wall
#define NORTH_MOVING_WALL(WALL_BIT_DATA)    { (WALL_BIT_DATA), (6), (SOUTH_WALL_BOUNDARY), (ROW), (NORTH) }  // Initial NORTH wall
#define SOUTH_MOVING_WALL(WALL_BIT_DATA)    { (WALL_BIT_DATA), (0), (SOUTH_WALL_BOUNDARY), (ROW), (SOUTH) }  // Initial SOUTH wall


// Declaration of all possible wall movement directions
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


/* Structure containing aspects of moving wall
 */
typedef struct
{
	uint8_t        bit_data;          // eg. 0b11000111 -- (1= wall, 0=hole)
	uint8_t        pos;               // Current column/row
	uint8_t        boundary_cond;     // If pos>coundary_cond for wall deletion
	WALL_TYPE      wall_type;         // COLUMN/ROW/OUT_OF_BOUNDS
	WALL_DIRECTION direction;         // The direction of movement of wall
} WallStruct;


/*  Initialises module
 *  @params initial_seed: uses srand() from <stdlib.h> to set initial seed
 *    for pseudorandom number generator (PRNG)
 *  @brief: Given deterministic nature of PRNG's, seed must vary game-to-game */
void wall_init(uint8_t initial_seed);


/*  Resets and randomises ACTIVE_WALL
 *  @brief: starting random seed is initialised in wall_init() with srand()
 *          uses helper function decide_wall_type() to create wall
 */
void wall_create(void);


/* Returns the current active wall as WallStruct
 * @return WallStruct active wall*/
WallStruct get_active_wall(void);


/*  Toggles display state of ACTIVE_WALL
 *  @param display_on: if 0 = then ACTIVE_WALL in wall.c isn't displayed
 */
void toggle_wall(bool display_on);


/*  Moves active wall in direction defined by wall
 *  @brief: moves wall, sets WALL_TYPE to OUT_OF_BOUNDS is greater than ACTIVE_WALL.boundary_cond */
void move_wall(void);


#endif
