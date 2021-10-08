/** @file   wall.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */

#include "wall.h"
#include "display.h"
#include <stdlib.h>

// Active moving wall
static Wall_t ACTIVE_WALL;

static uint8_t RANDOM_SEED; // Just so we have randomness rn


/* Creates adds a wall and replaces with oldest one */
Wall_t decide_wall_type(uint8_t direction_seed, uint8_t hole_size_seed, uint8_t hole_shift_seed)
{
   Wall_t  wall_n;
   uint8_t wall_bitmap;
   uint8_t wall_direction = (direction_seed) % 4;        
   uint8_t hole_size      = (hole_size_seed) % 3 + 1;   
   uint8_t WALL_SIZE = (wall_direction < 2) ? 8: 6;
   uint8_t hole_shift     = hole_shift_seed % (WALL_SIZE - hole_size);

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

   wall_bitmap = GENERATE_HOLE(hole_size, hole_shift);
   
   switch (wall_direction)
   {
   case 0: // EAST moving wall
      wall_n = (Wall_t)UP_COL_WALL(wall_bitmap);
      break;

   case 1: // WEST moving wall
      wall_n = (Wall_t)DOWN_COL_WALL(wall_bitmap);
      break;

   case 2: // NORTH moving wall
      wall_n = (Wall_t)UP_ROW_WALL(wall_bitmap);
      break;
   case 3: // SOUTH moving wall
      wall_n = (Wall_t)DOWN_ROW_WALL(wall_bitmap);
      break;

   default: // Won't ever occur but just incase - EAST moving wall
      wall_n = (Wall_t)UP_COL_WALL(wall_bitmap);
      break;
   }

   return wall_n;
}

// Randomly generate a wall and spawn it in starting position
// Avoids possibility of wall being reset prematurely
// Won't matter atm, but could be useful when varying difficulty
void wall_create(void)
{
   if (ACTIVE_WALL.dir == OUT_OF_BOUNDS) 
   { 
      // Possible seed using srand based off the tick when game activated via button/menu
      // For greater variation

      // Randomly select wall-type using default seed
      srand(RANDOM_SEED);
      uint8_t direction_seed  = rand();
      uint8_t hole_size_seed  = rand();
      uint8_t hole_shift_seed = rand();

      // Generate wall type
      ACTIVE_WALL = decide_wall_type(direction_seed, hole_size_seed, hole_shift_seed);
      RANDOM_SEED++;

      //Display wall
      toggle_wall(true);
   }
}


/*  Toggles display state of ACTIVE_WALL
 *  @param display_on, if 0 display is removed
 */
void toggle_wall(bool display_on)
{
   // Only one active wall so only need bool input
   if (ACTIVE_WALL.dir == OUT_OF_BOUNDS) {  // Checks if ACTIVE_WALL is OUT_OF_BOUNDS 
      return;
   }

   uint8_t position = ACTIVE_WALL.pos;
   uint8_t pattern = (display_on) ? ACTIVE_WALL.bit_data : 0;
   uint8_t index;

   switch (ACTIVE_WALL.dir)
   {
   case NORTH:
   case SOUTH:
      for (index = 0; index <= LEDMAT_COLS_NUM; index++)
      {
         bool state = (BIT(index) & pattern) != 0;
         display_pixel_set(index, position, state);
      }
      break;

   case EAST:
   case WEST:
      for (index = 0; index <= LEDMAT_ROWS_NUM; index++)
      {
         bool state = (BIT(index) & pattern) != 0;
         display_pixel_set(position, index, state);
      }
      break;
   
   default:
      break;
   }
}


/*  Moves active wall in direction defined by wall
 *  @brief, moves wall, sets walls=NULL if out_of_bounds
 */
void move_wall()
{
   if (ACTIVE_WALL.dir != OUT_OF_BOUNDS) { // If ACTIVE_WALL is not NULL

      toggle_wall(false);
      // if dir is EAST or SOUTH, pos++
      ACTIVE_WALL.pos += (ACTIVE_WALL.dir <= 2) ? 1: -1;

      if (ACTIVE_WALL.pos > ACTIVE_WALL.boundary_cond)
      {
         ACTIVE_WALL.dir = OUT_OF_BOUNDS;
         return;
      }

      toggle_wall(true); // Won't change anything if ACTIVE_WALL is NULL
   }
}
