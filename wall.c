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

// Active moving wall
static Wall_t ACTIVE_WALL;


// Sets seed for wall generation
void wall_init()
{
   srand(rand());
   //Reset wall if active wall exists
   ACTIVE_WALL.wall_type = OUT_OF_BOUNDS;
   ACTIVE_WALL.bit_data  = 0;
}


//Returns the current active wall as Wall_t
Wall_t get_active_wall()
{
   return ACTIVE_WALL;
}


/* Creates adds a wall and replaces with oldest one */
Wall_t decide_wall_type(uint8_t direction_seed, uint8_t hole_size_seed, uint8_t hole_shift_seed)
{
   Wall_t  wall_n;
   uint8_t wall_bitmap;
   uint8_t wall_direction = (direction_seed) % 4 + 1;
   uint8_t hole_size      = (hole_size_seed) % 3 + 1;
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

   wall_bitmap = GENERATE_HOLE(hole_size, hole_shift);

   switch (wall_direction)
   {
   case NORTH:     // NORTH moving wall
      wall_n = (Wall_t)NORTH_MOVING_WALL(wall_bitmap);
      break;

   case EAST: // EAST moving wall
      wall_n = (Wall_t)EAST_MOVING_WALL(wall_bitmap);
      break;

   case SOUTH:    // SOUTH moving wall
      wall_n = (Wall_t)SOUTH_MOVING_WALL(wall_bitmap);
      break;

   case WEST: // WEST moving wall
      wall_n = (Wall_t)WEST_MOVING_WALL(wall_bitmap);
      break;

   default: // Won't ever occur but just incase - EAST moving wall
      wall_n = (Wall_t)EAST_MOVING_WALL(wall_bitmap);
      break;
   }

   return wall_n;
}


// Randomly generate a wall and spawn it in starting position
// Avoids possibility of wall being reset prematurely
// Won't matter atm, but could be useful when varying difficulty
void wall_create(void)
{
   // Possible seed using srand based off the tick when game activated via button/menu
   // For greater variation

   // Randomly select wall-type using default seed
   uint8_t direction_seed  = rand();
   uint8_t hole_size_seed  = rand();
   uint8_t hole_shift_seed = rand();

   // Generate wall type
   ACTIVE_WALL = decide_wall_type(direction_seed, hole_size_seed, hole_shift_seed);

   //Display wall
   toggle_wall(true);
}


/*  Toggles display state of ACTIVE_WALL
 *  @param display_on, if 0 display is removed
 */
void toggle_wall(bool display_on)
{
   // Only one active wall so only need bool input

   uint8_t    position = ACTIVE_WALL.pos;
   uint8_t    pattern  = (display_on) ? ACTIVE_WALL.bit_data : 0;
   uint8_t    index;
   Position_t character = get_character_pos();

   switch (ACTIVE_WALL.wall_type)
   {
   case ROW:
      for (index = 0; index <= DISPLAY_WIDTH; index++)
      {
         bool state = (BIT(index) & pattern) != 0;
         if ((index != character.x) || (position != character.y))
         {
            display_pixel_set(index, position, state);
         }
      }
      break;

   case COLUMN:
      for (index = 0; index <= DISPLAY_HEIGHT; index++)
      {
         bool state = (BIT(index) & pattern) != 0;
         if ((position != character.x) || (index != character.y))
         {
            display_pixel_set(position, index, state);
         }
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
   toggle_wall(false);
   // if dir is EAST or SOUTH, pos++
   ACTIVE_WALL.pos += (ACTIVE_WALL.direction % 2 == 0) ? 1: -1;

   if (ACTIVE_WALL.pos > ACTIVE_WALL.boundary_cond)
   {
      ACTIVE_WALL.wall_type = OUT_OF_BOUNDS;
      return;
   }

   toggle_wall(true);    // Won't change anything if ACTIVE_WALL is NULL
}
