/** @file   wall.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#include "system.h"
#include "wall.h"
#include "display.h"
#include <stdlib.h>
#include "character.h"


// Global variable used to store wall information
static WallStruct active_wall;


/*  Initialises module
 *  @params initial_seed: uses srand() from <stdlib.h> to set initial seed
 *                        for pseudorandom number generator (PRNG)
 *   @brief: Given deterministic nature of PRNG's, seed must vary game-to-game
 */
void wall_init(uint8_t initial_seed)
{
	srand(initial_seed);
	// Reset wall if active wall exists (game reset)
	active_wall.wall_type = OUT_OF_BOUNDS;
	active_wall.bit_data  = 0;
}


/*  Creates wall based on randomly generated integer inputs
 *  @params: All inputs are random uint8_t integers which seed wall direction, hole size, and hole position
 *  @brief: Each input decides a different aspect of wall
 *  @return: Randomly generated WallStruct
 */
static WallStruct decide_wall_type(uint8_t direction_seed, uint8_t hole_size_seed, uint8_t hole_shift_seed)
{
	WallStruct new_wall;
	uint8_t    wall_bitmap;

	WALL_DIRECTION_t wall_direction = (direction_seed) % NUM_OF_DIRECTIONS + 1;         // Random number in interval [1, NUM_OF_DIRECTIONS], decides wall direction
	uint8_t hole_size      = (hole_size_seed) % MAX_HOLE_SIZE + 1;             // Random number in interval [1, MAX_HOLE_SIZE], decides hole size

	// Randomly shift hole along the wall, must be less than (wall_size - hole_size)
	// (e.g. if ROW and hole_size is 3, shift must be less than 6-3)
	uint8_t wall_size  = ((wall_direction == NORTH || wall_direction == SOUTH) ? ROW_SIZE : COLUMN_SIZE) + 1;
	uint8_t hole_shift = hole_shift_seed % (wall_size - hole_size);

	// Convert hole size to binary for bitmap
	// For better dynamic size handling a decimal to binary function will need to be implemented
	switch (hole_size)
	{
	case 1:
		hole_size = 0b1;
		break;

	case 2:
		hole_size = 0b11;
		break;

	case 3:
		hole_size = 0b111;
		break;

	default:
		hole_size = 0b1;                 //Ensures a gap of 1 will always exist
		break;
	}

	wall_bitmap = GENERATE_HOLE(hole_size, hole_shift);          // Generates wall bit_data using the random parameters

	// Creates wall moving in given direction
	switch (wall_direction)
	{
	case NORTH:             // NORTH moving wall, ROW
		new_wall = (WallStruct)NORTH_MOVING_WALL(wall_bitmap);
		break;

	case EAST:              // EAST moving wall, COLUMN
		new_wall = (WallStruct)EAST_MOVING_WALL(wall_bitmap);
		break;

	case SOUTH:             // SOUTH moving wall, ROW
		new_wall = (WallStruct)SOUTH_MOVING_WALL(wall_bitmap);
		break;

	case WEST:              // WEST moving wall, COLUMN
		new_wall = (WallStruct)WEST_MOVING_WALL(wall_bitmap);
		break;

	default:              // East moving wall as failsafe
		new_wall = (WallStruct)EAST_MOVING_WALL(wall_bitmap);
		break;
	}

	return new_wall;
}


/*  Resets and randomises active_wall
 *  @brief: starting random seed is initialised in wall_init() with srand()
 *          uses helper function decide_wall_type() to create wall
 */
void wall_create(void)
{
	// Randomly select wall-type using default seed
	uint8_t direction_seed  = rand();
	uint8_t hole_size_seed  = rand();
	uint8_t hole_shift_seed = rand();

	// Generate wall type
	active_wall = decide_wall_type(direction_seed, hole_size_seed, hole_shift_seed);

	toggle_wall(true);           //Display wall
}


/* Returns the current active wall as WallStruct
 * @return WallStruct active wall*/
WallStruct get_active_wall()
{
	return active_wall;
}


/*  Toggles display state of active_wall
 *  @param display_on: if 0 = then active_wall in wall.c isn't displayed
 */
void toggle_wall(bool display_on)
{
	// If display is off, pattern if 0
	uint8_t pattern = (display_on) ? active_wall.bit_data : 0;
	uint8_t index;
	uint8_t position = active_wall.pos;          // Position of the wall

	CharacterInfoStruct character = get_character_info();

	switch (active_wall.wall_type)
	{
	// If wall_type is ROW, position is in y-axis
	case ROW:
		// Iterate each pixel
		for (index = 0; index <= DISPLAY_WIDTH; index++)
		{
			bool state = (BIT(index) & pattern) != 0;                                        // Gets the index-th bit of the walls bit_data
			if ((index != character.x) || (position != character.y))                         // Wont display over character
			{
				display_pixel_set(index, position, state);                                   // display the state of each pixel in wall
			}
		}
		break;

	// If wall_type is COLUMN, position is in x-axis
	case COLUMN:
		// Iterate each pixel
		for (index = 0; index <= DISPLAY_HEIGHT; index++)
		{
			bool state = (BIT(index) & pattern) != 0;                                        // Gets the index-th bit of the walls bit_data
			if ((position != character.x) || (index != character.y))                         // Wont display over character
			{
				display_pixel_set(position, index, state);                                   // display the state of each pixel in wall
			}
		}
		break;

	default:
		break;
	}
}


/*  Moves active wall in direction defined by wall
 *  @brief: moves wall, sets WALL_TYPE to OUT_OF_BOUNDS is greater than active_wall.boundary_cond
 */
void move_wall()
{
	// Toggle wall display state to off before moving it
	toggle_wall(false);

	// SOUTH/EAST are positive increments NORTH/WEST are negative
	active_wall.pos += (active_wall.direction == SOUTH || active_wall.direction == EAST) ? STEP_SIZE: -STEP_SIZE;

	// Positions are unsigned, thus only one boundary check is needed
	// (as for negative moving directions pos = 0 -> 255, = above boundary)
	if (active_wall.pos > active_wall.boundary_cond)
	{
		active_wall.wall_type = OUT_OF_BOUNDS;
		return;
	}

	toggle_wall(true);           // Won't change anything if active_wall is NULL
}
