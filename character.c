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
bool       character_stun = false;


// Display character at default coordinates
void character_init(uint8_t life_count)
{
   CHARACTER_POS = (Position_t){
      .x = DEFAULT_X, .y = DEFAULT_Y, .lives = life_count
   };
   display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);

   if (get_stun_condition())  //Prevent character being stunned on respawn
   {
      toggle_stun(false);
   }
}


//Turn off character
void character_disable()
{
   display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
}


// Implements stun attribute in the game (after wall collsion)
void toggle_stun(bool stun_set)
{
   character_stun = stun_set;
}


bool get_stun_condition()
{
   return character_stun;
}


/** Return current character coordinates
 *  @return Position_t with .x .y values */
Position_t get_character_pos(void)
{
   return CHARACTER_POS;
}


/** Move character 1 unit west*/
bool move_west()
{
   if ((WEST_CHARACTER_BOUNDARY < CHARACTER_POS.x) && !display_pixel_get(CHARACTER_POS.x - 1, CHARACTER_POS.y))
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.x -= 1;
      if (!display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y))
      {
         display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
      }
      return 0;
   }
   return 1;
}


/** Move character 1 unit east*/
bool move_east()
{
   if ((EAST_CHARACTER_BOUNDARY > CHARACTER_POS.x) && !display_pixel_get(CHARACTER_POS.x + 1, CHARACTER_POS.y))
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.x += 1;
      if (!display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y))
      {
         display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
      }
      return 0;
   }
   return 1;
}


/** Move character 1 unit north*/
bool move_north()
{
   if ((NORTH_CHARACTER_BOUNDARY < CHARACTER_POS.y) && !display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y - 1))
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.y -= 1;
      if (!display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y))
      {
         display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
      }
      return 0;
   }
   return 1;
}


/** Move character 1 unit south*/
bool move_south()
{
   if ((SOUTH_CHARACTER_BOUNDARY > CHARACTER_POS.y) && !display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y + 1))
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
      CHARACTER_POS.y += 1;
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);

      return 0;
   }
   return 1;
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
   if (!character_stun)
   {
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
}


/* Returns high if CHARACTER_POS has zero lives */
bool decrease_player_lives()
{
   CHARACTER_POS.lives--;
   return(CHARACTER_POS.lives == 0);
}
