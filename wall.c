/** @file   wall.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */
#include "wall.h"
#include "display.h"
#include <stdlib.h>

// Active moving wall
static Wall_t active_wall;

/* Creates adds a wall and replaces with oldest one */
Wall_t decide_wall_type(uint8_t hole_size_seed, uint8_t hole_shift_seed)
{
   Wall_t  wall_n;
   uint8_t wall_bitmap;
   bool    wall_direction = (hole_shift_seed) % 2;
   uint8_t hole_size      = (hole_size_seed) % 3 + 1;
   uint8_t hole_shift     = hole_shift_seed % (DISPLAY_HEIGHT - hole_size);

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
   wall_n      = wall_direction? (Wall_t)UP_COL_WALL(wall_bitmap) : (Wall_t)DOWN_COL_WALL(wall_bitmap);

   return wall_n;
}


// Randomly generate a wall and spawn it in starting position
void wall_create(void)
{
   // Possible seed using srand based off the tick when game activated via button/menu
   // For greater variation

   // Randomly select wall-type using default seed
   uint8_t hole_size_seed  = rand();
   uint8_t hole_shift_seed = rand();

   // Generate wall type
   active_wall = decide_wall_type(hole_size_seed, hole_shift_seed);

   //Display wall
   toggle_wall(active_wall, true);
}


/*  Toggles display state of a wall
 *  @param wall,
 *  @param display_on, if 0 display is removed
 */
void toggle_wall(Wall_t col_wall, bool display_on)
{
   uint8_t wall_pos = col_wall.pos;
   uint8_t pattern  = (display_on) ? col_wall.bit_data : 0;
   uint8_t col_index;

   for (col_index = 0; col_index <= LEDMAT_ROWS_NUM; col_index++)
   {
      bool state = (BIT(col_index) & pattern) != 0;
      display_pixel_set(wall_pos, col_index, state);
   }
}


/*  Moves active wall in direction defined by wall
 *  @brief, moves wall, sets walls=None if out_of_bounds
 */
void move_wall()
{
   toggle_wall(active_wall, false);
   active_wall.pos += active_wall.dir;
   switch (active_wall.dir) //Stop wall from moving further than border
   {
   case EAST:
   case WEST: // Underflow will cause west movement position to be maximum
      if (active_wall.pos > EAST_WALL_BOUNDARY)
      {
         active_wall.dir = OUT_OF_BOUNDS;
         return;
      }
      break;

   default:
      break;
   }
   toggle_wall(active_wall, true);
}
