/** @file   game.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   10 Oct 2021
 *  @brief  Main task control
 */

#include "system.h"
#include "pacer.h"
#include "display.h"
#include "character.h"
#include "wall.h"
#include "game_manager.h"

#include "tinygl.h"


//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE          275
#define INPUT_UPDATE_RATE            20
#define PACER_RATE                   500 // Total ticks in a second
#define MESSAGE_RATE                 25  // Tinygl text scroll speed

//Difficulty constants
#define WALL_SPEED_INCREMENT_RATE    15         // Delay before speed increment in seconds
#define WALL_SPEED_INCREMENT         1          // Amount wall speed increases by (walls/second)
#define DEFAULT_SPEED                1          // Default starting wall speed

static uint8_t WALL_SPEED      = DEFAULT_SPEED; // Default wall speed (walls/second)
uint16_t       game_start_time = 0;             // Used so speed increase counts time from game start



int main(void)
{
   // Module initialization
   system_init();
   display_init();
   pacer_init(PACER_RATE);

   tinygl_init(PACER_RATE);
   tinygl_text_speed_set(MESSAGE_RATE);

   game_init();



   // Task delay times
   uint16_t input_time           = PACER_RATE / INPUT_UPDATE_RATE;
   uint16_t display_time         = PACER_RATE / DISPLAY_UPDATE_RATE;
   uint16_t wall_time            = PACER_RATE / WALL_SPEED; // Wall speed
   uint16_t speed_increment_time = PACER_RATE * WALL_SPEED_INCREMENT_RATE;

   static uint16_t ticks = 0;

   while (1)
   {
      pacer_wait();

      if ((ticks % display_time) == 0) //Display refresh
      {
         display_update();
      }

      // Start game with pushbutton as it needs to be used
      if (get_game_state())   //Run game updates if game is active
      {
         //Ticks-1 so that wall speed isn't increased immediately on game start
         // 0 % anything = 0
         game_start_time = (game_start_time == 0)? ticks - 1: game_start_time;

         if (((ticks - game_start_time) % speed_increment_time) == 0)
         {
            WALL_SPEED += WALL_SPEED_INCREMENT;
            wall_time   = PACER_RATE / WALL_SPEED;
         }

         if ((ticks % wall_time) == 0)    //Move or create wall
         {
            wall_update();
            if (collision_dectection())
            {
               WALL_SPEED      = DEFAULT_SPEED;
               wall_time       = PACER_RATE / WALL_SPEED;
               game_start_time = 0;
               game_end();
               continue;
            }
         }


         if (ticks % input_time == 0)                //Character input polling
         {
            character_update();
            if (collision_dectection())
            {
               WALL_SPEED      = DEFAULT_SPEED;
               wall_time       = PACER_RATE / WALL_SPEED;
               game_start_time = 0;
               game_end();
               continue;
            }
         }
      }
      else
      {
         tinygl_update();             //Update scrolling text

         if (ticks % input_time == 0) //Game start input polling
         {
            game_state_update(ticks); //Use current tick for wall seed
         }
      }

      ticks++;
   }
}
