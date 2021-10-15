/** @file   character.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   3 Oct 2021
 *  @brief  Character profile
 */
 
#include "character.h"
#include "display.h"
#include "navswitch.h"

// Character struct
PlayerInfoStruct CHARACTER_POS;
bool CHARACTER_STUN = false;


/* Initialisation for character module
 * @param life_count: creates player with respective number of lives
 * @brief: Character creation, spawns character at default coordinates 
*/
void character_init(uint8_t life_count)
{
   CHARACTER_POS = (PlayerInfoStruct){
      .x = DEFAULT_X, .y = DEFAULT_Y, .lives = life_count
   };
   display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);

   if (get_stun_condition())  //Prevent character being stunned on respawn
   {
      toggle_stun(false);
   }
}


/* Sets the character display state to false
*/
void character_disable()
{
   display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, false);
}


/*  Changes the stun state of the player
 *  @param stun_set: bool variable to which CHARACTER_STUN is set
 */
void toggle_stun(bool stun_set)
{
   CHARACTER_STUN = stun_set;
}

/* Returns the stun state of the player
*/
bool get_stun_condition()
{
   return CHARACTER_STUN;
}


/* Return current character coordinates
 *  @return PlayerInfoStruct with coordinates and number of lives 
 */
PlayerInfoStruct get_character_pos(void)
{
   return CHARACTER_POS;
}


/* Move character 1 unit west*/
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


/* Move character 1 unit east*/
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


/* Move character 1 unit north*/
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


/* Move character 1 unit south*/
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


/* Poll navswitch input and move character
*  @brief: Doesn't allow movement is player is stunned 
*/
void character_update()
{
   navswitch_update(); // Update navswitch input

   //Restores character state if passed by wall
   if (!display_pixel_get(CHARACTER_POS.x, CHARACTER_POS.y))
   {
      display_pixel_set(CHARACTER_POS.x, CHARACTER_POS.y, true);
   }
   // Move character in direction of navswitch input
   // Doesn't allow movement if character is stunned
   if (!CHARACTER_STUN)
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


/* Reduces player lives by 1
*  @return: returns true is player lives is reduced to 0
*/
bool decrease_player_lives()
{
   CHARACTER_POS.lives--;
   return(CHARACTER_POS.lives == 0);
}
