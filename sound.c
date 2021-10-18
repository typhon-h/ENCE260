/** @file   sound.c
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   16 Oct 2021
 *  @brief  Sound profiles
 */

#include "system.h"
#include "sound.h"
#include "../../extra/tweeter.h"
#include "../../extra/mmelody.h"
#include "pio.h"

// Speaker objects
static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE(TWEETER_SWITCH_RATE); // Initialize required PWM for notes
static tweeter_t       tweeter;
static tweeter_obj_t   tweeter_info;

// Melody objects
static mmelody_t     melody;
static mmelody_obj_t melody_info;


/* Initialisation for sound module
 * @param uint16_t rate melody task updates in hz
 * @brief: Initializes tweeter and melody objects
 */
void sound_init(uint16_t tune_task_rate)
{
	// Speaker object init
	tweeter = tweeter_init(&tweeter_info, TWEETER_SWITCH_RATE, scale_table);

	//Speaker pins init
	pio_config_set(SPEAKER_PIN1, PIO_OUTPUT_LOW);
	pio_config_set(SPEAKER_PIN2, PIO_OUTPUT_LOW);

	// Melody object init
	melody = mmelody_init(&melody_info, tune_task_rate,
	                      (mmelody_callback_t)tweeter_note_play, tweeter);
	mmelody_speed_set(melody, MELODY_BPM_DEFAULT);
}


/* Toggle piezo speaker
 * @brief: Switches high/low states on the pins
 * to implement a push/pull polling method
 */
static void toggle_speaker(bool state)
{
	pio_output_set(SPEAKER_PIN1, state);
	pio_output_set(SPEAKER_PIN2, !state);
}


/* Get required speaker state (on/off)
 * and toggle speaker accordingly
 */
void speaker_update()
{
	bool state = tweeter_update(tweeter);

	toggle_speaker(state);
}


/* Advance the current melody */
void sound_update()
{
	mmelody_update(melody);
}


/* Play a song
 * @param song: char array containing song to play in mmelody notation */
void sound_play(char song[])
{
	mmelody_play(melody, song);
}
