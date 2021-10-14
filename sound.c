#include "system.h"
#include "../../extra/tweeter.h"
#include "../../extra/mmelody.h"
#include "pio.h"
#include "sound.h"

//Extracted from ../apps/space12.c
#define PIEZO1_PIO           PIO_DEFINE(PORT_D, 4) //Pin 1
#define PIEZO2_PIO           PIO_DEFINE(PORT_D, 6) //Pin 3

// Need to get rid of this magic number = TWEETER_TASK_RATE
static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (5000);
static tweeter_t     tweeter;
static mmelody_t     melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;

// Activate speaker
void toggle_speaker(bool state)
{
    //Implements dual pin push/pull polling
   pio_output_set(PIEZO1_PIO, state);
   pio_output_set(PIEZO2_PIO, !state);
}


void speaker_update() // Check if the speaker needs to be on and toggle it
{
   bool state = tweeter_update(tweeter);

   toggle_speaker(state);
}


void sound_update() // Update current sound
{
   mmelody_update(melody);
}


void sound_stop() // Stop playing current sound
{
   mmelody_play(melody, "");
}


void sound_play(char song[]) //Play song
{
    mmelody_play(melody, song);
}

void beep() // Single beep used for testing
{
    mmelody_play(melody, "C,C+,C");
}

void BPM_change(uint16_t tune_bpm_rate) // Update BPM of melody
{
	mmelody_speed_set(melody, tune_bpm_rate);
}

// Initialize sound drivers
void sound_init(uint16_t tweeter_task_rate, uint16_t tune_task_rate, uint16_t tune_bpm_rate)
{
    // Speaker object init
   tweeter = tweeter_init(&tweeter_info, tweeter_task_rate, scale_table);

   //Speaker pins init
   pio_config_set(PIEZO1_PIO, PIO_OUTPUT_LOW);
   pio_config_set(PIEZO2_PIO, PIO_OUTPUT_LOW);

   // Melody object init
   melody = mmelody_init(&melody_info, tune_task_rate,
                         (mmelody_callback_t)tweeter_note_play, tweeter);
   mmelody_speed_set(melody, tune_bpm_rate);
}
