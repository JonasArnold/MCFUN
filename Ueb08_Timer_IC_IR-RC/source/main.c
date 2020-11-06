/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 07.3 - Buzzer with timer output compare
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          04.04.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "ftm0.h"
#include "ir.h"
#include "term.h"
#include "sound.h"
#include "soundPlayer.h"

const char sound1[] = "Kn-Rider:d=4,o=6,b=90:16d.5,32d#.5,32d.5,8a.5,16d.,32d#.,32d.,8a.5,16d.5,32d#.5,32d.5,16a.5,16d.,2c,16d.5,32d#.5,32d.5,8a.5,16d.,32d#.,32d.,8a.5,16d.5,32d#.5,32d.5,16a.5,16d.,2d#,a4,32a#.4,32a.4,d5,32d#.5,32d.5,2a5,16c.,16d.";
const char sound2[] = "StarWars/Imp:d=4,o=5,b=112:8d.,16p,8d.,16p,8d.,16p,8a#4,16p,16f,8d.,16p,8a#4,16p,16f,d.,8p,8a.,16p,8a.,16p,8a.,16p,8a#,16p,16f,8c#.,16p,8a#4,16p,16f,d.,8p,8d.6,16p,8d,16p,16d,8d6,8p,8c#6,16p,16c6,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#4,16p,2c#";
const char sound3[] = "Theme:d=4,o=6,b=200:8d,8e,2f,8g,8a,g,f,e,f,g,a,g,p,8f,8g,a,p,8g,8f,e,f,e,d,p,8e,8c,d,8p,p,8d,8e,f,p,8e,8f,g,f,g,a,g,f,d";
const char sound4[] = "ImBlue:d=4,o=6,b=63:16b,16d#,16g#,16b,16c#7,16f#,16a#,8b,16g#,16b,16d#7,16e7,16g#,16d#7,16c#7,16b,16d#,16g#,16b,16c#7,16f#,16a#,8b,16g#,16b,16d#7,16e7,16g#,16d#7,16c#7,16b,16d#,16g#,16b,16c#7,16f#,16a#,8b,16g#,16b,16d#7,16e7,16g#,16d#7,16c#7,16b,16d#,16g#,16b,16a#,16c#,16f#,8g#,16b5,16f#,8g#,16f#,16g#,16a#,16b,16d#,16g#,16b,16c#7,16f#,16a#,8b,16g#";
const char sound5[] = "AkteX:d=4,o=5,b=140:b4,f#,e,f#,a,1f#,2p.,b4,f#,e,f#,b,1f#,2p.,d6,c#6,b,a,b,1f#,2p.,d6,c#6,b,a,c#6,1f#,2p.,b4,f#,e,f#,a,1f#,2p.,b,1f#,b4";


/**
 * This function implements the function of the different keycodes.
 */
void ProcessInfraredCommand(char key)
{
  switch (key)
  {
    case Key1: soundBeep(1000, 100); break;
    case Key2: soundBeep(2000, 100); break;
    case Key3: soundBeep(3000, 100); break;

    case Key4: soundPlay(sound1); break;
    case Key5: soundPlay(sound2); break;
    case Key6: soundPlay(sound3); break;
    case Key7: soundPlay(sound4); break;
    case Key8: soundPlay(sound5); break;

    case KeyOK : soundStop(); break;

    case KeyStar: soundBeep(4000, 100); break;
  }
}



/**
 * Audio player with remote control :-)
 */
void main(void)
{
  ftm0Init();
  termInit(TERMINAL_BAUDRATE);
  soundInit();
  soundPlayerInit();
  soundBeep(4000, 100);
  EnableDebugLeds();
  irInit();

  while(TRUE)
  {
    termDoWork();

    char key = irGetKey();
    if (key) ProcessInfraredCommand(key);
  }

  // Never leave main
  for (;;){}
}
