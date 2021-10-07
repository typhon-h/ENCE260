/** @file   wall.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   5 Oct 2021
 *  @brief  Wall profiles
 */
#include "wall.h"
#include "display.h"
#include <stdlib.h>


/* Array containing all walls */
/* Walls are added by replacing oldest wall */
static Wall_t  COL_WALL_ARRAY[MAX_COL_WALLS]; // Initalise array of walls
static uint8_t OLDEST_WALL_INDEX = 0;         // Points to oldest wall
static uint8_t RANDOM_SEED       = 0;         // The starting seed for rand()


/* Creates adds a wall and replaces with oldest one */
Wall_t decide_wall_type(uint8_t hole_size_seed, uint8_t hole_shift_seed)
{
   Wall_t  wall_n;
   uint8_t wall_bitmap;
   bool    wall_direction = (hole_size_seed) % 2;
   uint8_t hole_size      = (hole_size_seed) % 3 + 1;
   uint8_t hole_shift     = hole_shift_seed % (7 - hole_size);

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
   // Randomly select wall-type
   srand(RANDOM_SEED);
   uint8_t hole_size_seed  = rand();
   uint8_t hole_shift_seed = rand();

   Wall_t wall = decide_wall_type(hole_size_seed, hole_shift_seed);

   COL_WALL_ARRAY[OLDEST_WALL_INDEX] = wall;
   toggle_wall(wall, 1);
   OLDEST_WALL_INDEX = (OLDEST_WALL_INDEX + 1) % MAX_COL_WALLS;
   RANDOM_SEED++;
}


/*  Toggles display state of a wall
 *  @param wall,
 *  @param display_on, if 0 display is removed
 */
void toggle_wall(Wall_t col_wall, bool display_on)
{
   uint8_t wall_pos = col_wall.pos;
   uint8_t pattern  = (display_on) ? col_wall.bit_data: 0;
   uint8_t col_index;

   for (col_index = 0; col_index <= SOUTH_WALL_BOUNDARY; col_index++)
   {
      bool state = ((BIT(col_index) & pattern) != 0) ? true: false;
      display_pixel_set(wall_pos, col_index, state);
   }
}


/*  Helper function to move_walls
 *  @param wall, a pointer to wall
 *  @brief, moves wall, sets walls=None if out_of_bounds
 */
void move_wall(Wall_t *wall)
{
   uint8_t new_pos = wall->pos + wall->dir;      // Calculates new position based on direction

   toggle_wall(*wall, false);
   if (new_pos > EAST_WALL_BOUNDARY)
   {
      wall->dir      = OUT_OF_BOUNDS;
      wall->bit_data = 0;
   }
   else
   {
      wall->pos = new_pos;
      toggle_wall(*wall, true);
   }
}


/* Increments the positions of all column walls by one*/
void move_col_walls()
{
   uint8_t wall_index;

   for (wall_index = 0; wall_index < MAX_COL_WALLS; wall_index++) // Cycles through STATIC wall array
   {
      move_wall((COL_WALL_ARRAY + wall_index));                   // Gives the pointer to given wall
   }
}
