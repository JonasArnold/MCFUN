/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 02 - GPIO Assembler
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          18.02.20018
 *
 * $Id: mainML.S 40 2018-03-14 15:03:42Z zajost $
 *
 *--------------------------------------------------------------------
 */

    .equ GPIOB_PDOR, 0x400FF040 // Port B, Port Data Output Register
    .equ GPIOB_PSOR, 0x400FF044 // Port B, Port Set Output Register
    .equ GPIOB_PCOR, 0x400FF048 // Port B, Port Clear Output Register
    .equ GPIOB_PTOR, 0x400FF04C // Port B, Port Toggle Output Register
    .equ GPIOB_PDIR, 0x400FF050 // Port B, Port Data Input Register
    .equ GPIOB_PDDR, 0x400FF054 // Port B, Port Data Direction Register

    .equ GPIOC_PDOR, 0x400FF080 // Port C, Port Data Output Register
    .equ GPIOC_PSOR, 0x400FF084 // Port C, Port Set Output Register
    .equ GPIOC_PCOR, 0x400FF088 // Port C, Port Clear Output Register
    .equ GPIOC_PTOR, 0x400FF08C // Port C, Port Toggle Output Register
    .equ GPIOC_PDIR, 0x400FF090 // Port C, Port Data Input Register
    .equ GPIOC_PDDR, 0x400FF094 // Port C, Port Data Direction Register

    .equ GPIOD_PDOR, 0x400FF0C0 // Port D, Port Data Output Register
    .equ GPIOD_PSOR, 0x400FF0C4 // Port D, Port Set Output Register
    .equ GPIOD_PCOR, 0x400FF0C8 // Port D, Port Clear Output Register
    .equ GPIOD_PTOR, 0x400FF0CC // Port D, Port Toggle Output Register
    .equ GPIOD_PDIR, 0x400FF0D0 // Port D, Port Data Input Register
    .equ GPIOD_PDDR, 0x400FF0D4 // Port D, Port Data Direction Register

    .equ PORTB_PCR1, 0x4004A004 // Port B, Port Control Register Pin 1
    .equ PORTB_PCR2, 0x4004A008 // Port B, Port Control Register Pin 2
    .equ PORTB_PCR3, 0x4004A00C // Port B, Port Control Register Pin 3

    .equ PORTC_PCR8, 0x4004B020 // Port C, Port Control Register Pin 8
    .equ PORTC_PCR9, 0x4004B024 // Port C, Port Control Register Pin 9
    .equ PORTC_PCR10, 0x4004B028 // Port C, Port Control Register Pin 10

    .syntax unified
    .arch armv7-m

    .text
    .thumb

    .thumb_func
    .align 2
    .globl   main
    .weak    main
    .type    main, %function

main:
/*--------PORTx_PCRn Bit 8 to H = GPIO--------*/
    ldr r0, =0x01       /* bit mask 0b00000111*/
    lsl r0, 8           /* shift mask 8x to the left 0b011100000000*/
    ldr r1, =PORTC_PCR8 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or)*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

	ldr r1, =PORTC_PCR9 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or)*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

    ldr r1, =PORTC_PCR10 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or)*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

    ldr r1, =PORTB_PCR1 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or)*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

    ldr r1, =PORTB_PCR2 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or)*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

    ldr r1, =PORTB_PCR3 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or)*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

/*--------PORTx_PCRn bit 0,1 to H = enable pull up, select pull up--------*/
    ldr r0, =0x03       /* bit mask 0b00000011*/
    ldr r1, =PORTB_PCR1 /* load address r1 = 0x4004A004*/
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    orr r2, r0          /* manipulate data with mask (or), set first two lsbs with 1*/
    str r2, [r1]        /* store data in address, *r1 = r2 */

    ldr r1, =PORTB_PCR2 /* load address */
    ldr r2, [r1]        /* save value of address */
    orr r2, r0          /* manipulate data with mask */
    str r2, [r1]        /* store data in address */

    ldr r1, =PORTB_PCR3 /* load address */
    ldr r2, [r1]        /* save value of address */
    orr r2, r0          /* manipulate data with mask */
    str r2, [r1]        /* store data in address */

/*-------GPIOx_PDDR bit 8, 9, 10 to H = 8, 9, 10 are outputs-------*/
    ldr r0, =0x07       /* bit mask 0b00000111*/
    lsl r0, 8           /* shift mask 8x to the left 0b011100000000*/
    ldr r1, =GPIOC_PDDR /* load address */
    ldr r2, [r1]        /* save value of address */
    orr r2, r0          /* manipulate data with mask (or), set correspondig bits to 1*/
    str r2, [r1]        /* store data in address,  */

loop1:
    ldr r1, =GPIOB_PDIR /* load address (input register) */
    ldr r2, [r1]        /* save value of address r2 = *r1 */
    ldr r5, =0x0E       /* load bit mask 0b00001110*/
    and r5, r2          /* manipulate data with mask (and) */
    lsl r5, 7           /* shift data 7x to the left */
    ldr r3, =GPIOC_PDOR /* load address (output register) */
    str r5, [r3]        /* store data in address */
    b loop1             /* jump to loop1 */


end:    // loop forever
    b      end


