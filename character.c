/** @file   character.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
#include "character.h"
#include "display.h"
#include "navswitch.h"

//Global character position
struct position_s pos = { .x = DEFAULT_X, .y = DEFAULT_Y };


void character_init()
{
   display_pixel_set(pos.x, pos.y, 1);
}


void move_left()
{
   if (LEFT_BOUNDARY < pos.x)
   {
      display_pixel_set(pos.x, pos.y, 0);
      pos.x -= 1;
      display_pixel_set(pos.x, pos.y, 1);
   }
}


void move_right()
{
   if (RIGHT_BOUNDARY > pos.x)
   {
      display_pixel_set(pos.x, pos.y, 0);
      pos.x += 1;
      display_pixel_set(pos.x, pos.y, 1);
   }
}


void move_up()
{
   if (UP_BOUNDARY < pos.y)
   {
      display_pixel_set(pos.x, pos.y, 0);
      pos.y -= 1;
      display_pixel_set(pos.x, pos.y, 1);
   }
}


void move_down()
{
   if (DOWN_BOUNDARY > pos.y)
   {
      display_pixel_set(pos.x, pos.y, 0);
      pos.y += 1;
      display_pixel_set(pos.x, pos.y, 1);
   }
}


void character_update()
{
   navswitch_update();

   //Move character in direction of navswitch input
   if (navswitch_push_event_p(NAVSWITCH_NORTH))
   {
      move_up();
   }
   else if (navswitch_push_event_p(NAVSWITCH_SOUTH))
   {
      move_down();
   }
   else if (navswitch_push_event_p(NAVSWITCH_EAST))
   {
      move_right();
   }
   else if (navswitch_push_event_p(NAVSWITCH_WEST))
   {
      move_left();
   }
}
