#include "system.h"
#include "pacer.h"
#include "display.h"
#include "character.h"


//Frequency of task execution in hz
#define DISPLAY_UPDATE_RATE      275
#define CHARACTER_UPDATE_RATE    20
#define PACER_RATE               500 // Total ticks in a second


int main(void)
{
   // Module initialization
   system_init();
   display_init();
   character_init();
   pacer_init(PACER_RATE);

   // Task delay times
   uint16_t character_time = PACER_RATE / CHARACTER_UPDATE_RATE;
   uint16_t display_time   = PACER_RATE / DISPLAY_UPDATE_RATE;
   uint16_t ticks          = 0;

   while (1)
   {
      pacer_wait();

      if (ticks == display_time) //Display refresh
      {
         display_update();
         display_time += PACER_RATE / DISPLAY_UPDATE_RATE;
      }

      if (ticks == character_time) //Character input polling
      {
         character_update();
         character_time += PACER_RATE / CHARACTER_UPDATE_RATE;
      }

      ticks++;
   }
}
