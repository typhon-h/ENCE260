/** @file   character.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
#include "character.h"
#include "display.h"
#include "navswitch.h"

// Character position
Position_t CHARACTER_POS;


// Display character at default coordinates
void character_init()
{
   CHARACTER_POS = (Position_t){
      .x = DEFAULT_X, .y = DEFAULT_Y
   };
   display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
}


//Turn off character
void character_disable()
{
   display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
}


/** Return current character coordinates
 *  @return Position_t with .x .y values */
Position_t get_character_pos(void)
{
   return CHARACTER_POS;
}


/** Move character 1 unit west*/
void move_west()
{
   if (WEST_CHARACTER_BOUNDARY < CHARACTER_POS.x)
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.x -= 1;
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
   }
}


/** Move character 1 unit east*/
void move_east()
{
   if (EAST_CHARACTER_BOUNDARY > CHARACTER_POS.x)
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.x += 1;
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
   }
}


/** Move character 1 unit north*/
void move_north()
{
   if (NORTH_CHARACTER_BOUNDARY < CHARACTER_POS.y)
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.y -= 1;
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
   }
}


/** Move character 1 unit south*/
void move_south()
{
   if (SOUTH_CHARACTER_BOUNDARY > CHARACTER_POS.y)
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.y += 1;
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
   }
}


/** Poll navswitch input and move character
 *  in corresponding direction*/
void character_update()
{
   navswitch_update();

   //Restore character state if passed by wall
   if (!display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y))
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
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
