#include "system.h"
#include "pacer.h"
#include "display.h"
#include "character.h"
#include "wall.h"
#include "game_manager.h"


//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE    275
#define INPUT_UPDATE_RATE      20
#define WALL_UPDATE_RATE       1                        // ****Liable to change****
#define PACER_RATE             500                      // Total ticks in a second

int main(void)
{
   // Module initialization
   system_init();
   display_init();
   pacer_init(PACER_RATE);
   game_init();

   // Task delay times
   uint16_t        input_time   = PACER_RATE / INPUT_UPDATE_RATE;
   uint16_t        display_time = PACER_RATE / DISPLAY_UPDATE_RATE;
   uint16_t        wall_time    = PACER_RATE / WALL_UPDATE_RATE; //Wall speed
   static uint16_t ticks        = 0;

   while (1)
   {
      pacer_wait();

      if ((ticks % display_time) == 0) //Display refresh
      {
         display_update();
      }

      // Start game with pushbutton as it needs to be used
      if (get_game_state())            //Run game updates if game is active
      {
         if ((ticks % wall_time) == 0) //Move or create wall
         {
            wall_update();
         }


         if (ticks % input_time == 0) //Character input polling
         {
            character_update();
         }
      }
      else
      {
         if (ticks % input_time == 0) //Game start input polling
         {
            game_state_update(ticks); //Use current tick for wall seed
         }
      }

      ticks++;
   }
}
