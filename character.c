/** @file   character.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
#include "character.h"
#include "display.h"
#include "navswitch.h"

// Global character position
position_t character_pos = { .x = DEFAULT_X, .y = DEFAULT_Y };


// Display character at default coordinates
void character_init()
{
   display_pixel_set(character_pos.x, character_pos.y, true);
}


/** Return current character coordinates
 *  @return position_t with .x .y values */
position_t get_character_pos(void)
{
   return character_pos;
}


/** Move character 1 unit west*/
void move_west()
{
   if (WEST_CHARACTER_BOUNDARY < character_pos.x)
   {
      display_pixel_set(character_pos.x, character_pos.y, false);
      character_pos.x -= 1;
      display_pixel_set(character_pos.x, character_pos.y, true);
   }
}


/** Move character 1 unit east*/
void move_east()
{
   if (EAST_CHARACTER_BOUNDARY > character_pos.x)
   {
      display_pixel_set(character_pos.x, character_pos.y, false);
      character_pos.x += 1;
      display_pixel_set(character_pos.x, character_pos.y, true);
   }
}


/** Move character 1 unit north*/
void move_north()
{
   if (NORTH_CHARACTER_BOUNDARY < character_pos.y)
   {
      display_pixel_set(character_pos.x, character_pos.y, false);
      character_pos.y -= 1;
      display_pixel_set(character_pos.x, character_pos.y, true);
   }
}


/** Move character 1 unit south*/
void move_south()
{
   if (SOUTH_CHARACTER_BOUNDARY > character_pos.y)
   {
      display_pixel_set(character_pos.x, character_pos.y, false);
      character_pos.y += 1;
      display_pixel_set(character_pos.x, character_pos.y, true);
   }
}


/** Poll navswitch input and move character
 *  in corresponding direction*/
void character_update()
{
   navswitch_update();

   //Restore character state if passed by wall
   if (!display_pixel_get(character_pos.x, character_pos.y))
   {
      display_pixel_set(character_pos.x, character_pos.y, true);
   }
   //Move character in direction of navswitch input
   if (navswitch_push_event_p(NAVSWITCH_NORTH))
   {
      move_north();
   }
   else if (navswitch_push_event_p(NAVSWITCH_SOUTH))
   {
      move_south();
   }
   else if (navswitch_push_event_p(NAVSWITCH_EAST))
   {
      move_east();
   }
   else if (navswitch_push_event_p(NAVSWITCH_WEST))
   {
      move_west();
   }
}

