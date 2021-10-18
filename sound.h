/** @file   sound.h
 *  @author Lucas Trickett, Harrison Tyson
 *  @date   16 Oct 2021
 *  @brief  Sound profiles
 */

#ifndef SOUND_H
#define SOUND_H

#include "system.h"
#include "../../extra/tweeter.h"

// Pins for speaker to plug into
#define SPEAKER_PIN1           PIO_DEFINE(PORT_D, 4) // Pin 1
#define SPEAKER_PIN2           PIO_DEFINE(PORT_D, 6) // Pin 3

// Music constants
#define TWEETER_SWITCH_RATE    5000                  // Rate speaker polls in hz
#define MELODY_BPM_DEFAULT     200                   //Default music speed


/* Initialisation for sound module
 * @param uint16_t rate melody task updates in hz
 * @brief: Initializes tweeter and melody objects
 */
void sound_init(uint16_t tune_task_rate);


/* Get required speaker state (on/off)
 * and toggle speaker accordingly
 */
void speaker_update(void);


/* Advance the current melody */
void sound_update(void);


/* Play a song
 * @param song: char array containing song to play in mmelody notation */
void sound_play(char song[]);


#endif
