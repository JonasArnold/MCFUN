/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Nokia Ringtone player
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          04.04.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_SOUNDPLAYER_H_
#define SOURCES_SOUNDPLAYER_H_

bool soundIsPlaying(void);
void soundTooglePlayPause(void);
void soundStart(void);
void soundStop(void);
void soundLoad(const char *soundFile);
void soundPlay(const char *soundFile);
void soundPlayerInit(void);

#endif /* SOURCES_SOUNDPLAYER_H_ */
