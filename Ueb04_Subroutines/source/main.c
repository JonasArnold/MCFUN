/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 02 - GPIO C
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          18.02.2020
 *
 *--------------------------------------------------------------------
 */
#include "MK22F51212.h"
#include <cr_section_macros.h>

#define UEBUNG 1     // 1 or 2


#if UEBUNG == 1
__TEXT(Flash2)
void Subroutine(void)
{
  __asm("nop");   // inline assembler command (to prevent optimization of routine)
}

__TEXT(Flash2)
void main(void)
{
  Subroutine();

  // Never leave main
  for (;;){}
}
#endif


#if UEBUNG == 2
__TEXT(Flash2)
int Inc(int aa)
{
  int cc;
  cc = aa + 1;
  return cc;
}

__TEXT(Flash2)
void main(void)
{
  int a=5;
  int res;

  res = Inc(a);

  // Never leave main
  for (;;){}
}
#endif

