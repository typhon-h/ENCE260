 /** @file   wall.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#include "wall.h"
#include "display.h"
#include <stdlib.h>
#include "character.h"
#include "game_manager.h"

// Global variable used to store wall information
static WallStruct ACTIVE_WALL;


/*  Initialises module
 *  @params initial_seed: uses srand() from <stdlib.h> to set initial seed
 *                        for pseudorandom number generator (PRNG)
 *   @brief: Given deterministic nature of PRNG's, seed must vary game-to-game
 */
void wall_init(uint8_t initial_seed)
{
   srand(initial_seed);
   //Reset wall if active wall exists
   ACTIVE_WALL.wall_type = OUT_OF_BOUNDS;
   ACTIVE_WALL.bit_data  = 0;
}


// Returns the current active wall as WallStruct
WallStruct get_active_wall()
{
   return ACTIVE_WALL;
}


/*  Creates wall based on randomly generated integer inputs
 *  @params: All inputs are random uint8_t integers
 *   @brief: Each input decides a different aspect of wall
 */
WallStruct decide_wall_type(uint8_t direction_seed, uint8_t hole_size_seed, uint8_t hole_shift_seed)
{
   WallStruct  return_wall;
   uint8_t wall_bitmap;

   uint8_t wall_direction = (direction_seed) % 4 + 1;   // Random number in interval [1, 4], decides wall direction
   uint8_t hole_size      = (hole_size_seed) % 3 + 1;   // Random number in interval [1, 3], decides hole size

   // Randomly shift hole along the wall, must be less than (wall_size - hole_size)
   // (e.g. if ROW and hole_size is 3, shift must be less than 6-3)
   uint8_t wall_size      = (wall_direction <= 2) ? 6: 8;
   uint8_t hole_shift     = hole_shift_seed % (wall_size - hole_size);

   switch (hole_size)      //Convert hole size to binary for bitmap
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
      hole_size = 0b1; //Ensures a gap of 1 will always exist
      break;
   }

   wall_bitmap = GENERATE_HOLE(hole_size, hole_shift);   // Generates wall bit_data from MACRO function.

   // switch statement, creates wall based on direction (using MACROS in HEADER file)
   switch (wall_direction)
   {
   case NORTH:      // NORTH moving wall, ROW, starts at row 6
      return_wall = (WallStruct) NORTH_MOVING_WALL(wall_bitmap);
      break;

   case EAST:       // EAST moving wall, COLUMN, starts at column 6
      return_wall = (WallStruct) EAST_MOVING_WALL(wall_bitmap);
      break;

   case SOUTH:      // SOUTH moving wall, ROW, starts at row 0
      return_wall = (WallStruct) SOUTH_MOVING_WALL(wall_bitmap);
      break;

   case WEST:       // WEST moving wall, COLUMN, starts at column 4
      return_wall = (WallStruct) WEST_MOVING_WALL(wall_bitmap);
      break;

   default:       // Won't occur, but just incase == EAST moving wall
      return_wall = (WallStruct) EAST_MOVING_WALL(wall_bitmap);
      break;
   }

   return return_wall;
}


/*  Resets and randomises ACTIVE_WALL
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
   ACTIVE_WALL = decide_wall_type(direction_seed, hole_size_seed, hole_shift_seed);

   toggle_wall(true);    //Display wall
}


/*  Toggles display state of ACTIVE_WALL
 *  @param display_on: if 0 = then ACTIVE_WALL in wall.c isn't displayed
 */
void toggle_wall(bool display_on)
{
   // If display is off, pattern if 0
   uint8_t   pattern  = (display_on) ? ACTIVE_WALL.bit_data : 0;
   uint8_t   index;
   uint8_t   position = ACTIVE_WALL.pos; // Position of the wall

   CharacterInfoStruct character = get_character_info();

   switch (ACTIVE_WALL.wall_type)
   {
   // If wall_type is ROW, position is in y-axis
   case ROW:
      // FOR loop goes through each bit in wall
      for (index = 0; index <= DISPLAY_WIDTH; index++)
      {
         bool state = (BIT(index) & pattern) != 0;  // Gets the nth bit of the walls bit_data
         if ((index != character.x) || (position != character.y)) // Wont display over character
         {
            display_pixel_set(index, position, state);  // display the state of each pixel in wall
         }
      }
      break;

   // If wall_type is COLUMN, position is in x-axis
   case COLUMN:
      // FOR loop goes through each bit in wall
      for (index = 0; index <= DISPLAY_HEIGHT; index++)
      {
         bool state = (BIT(index) & pattern) != 0;  // Gets the nth bit of the walls bit_data
         if ((position != character.x) || (index != character.y)) // Wont display over character
         {
            display_pixel_set(position, index, state);  // display the state of each pixel in wall
         }
      }
      break;

   default:
      break;
   }
}


/*  Moves active wall in direction defined by wall
 *  @brief: moves wall, sets WALL_TYPE to OUT_OF_BOUNDS is greater than ACTIVE_WALL.boundary_cond
 */
void move_wall()
{
   // Toggle wall display state to off before moving it
   toggle_wall(false);

   // As the enums SOUTH / EAST = 2 / 4, the below expression will increase position.
   // As NORTH and WEST are directions of decreasing position, position will decreases
   ACTIVE_WALL.pos += (ACTIVE_WALL.direction % 2 == 0) ? 1: -1;

   // Positions are unsigned, thus only one boundary check is needed
   // (as for negative moving directions pos = 0 -> 255, = above boundary)
   if (ACTIVE_WALL.pos > ACTIVE_WALL.boundary_cond)
   {
      ACTIVE_WALL.wall_type = OUT_OF_BOUNDS;
      return;
   }

   toggle_wall(true);    // Won't change anything if ACTIVE_WALL is NULL
}
