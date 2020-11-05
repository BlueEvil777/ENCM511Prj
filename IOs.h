/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define IOS_PB1_PRESSED 0x1
#define IOS_PB1_RELEASED 0x2
#define IOS_PB1_DEBOUNCE 0x4
#define IOS_PB2_PRESSED 0x8
#define IOS_PB2_RELEASED 0x10
#define IOS_PB2_DEBOUNCE 0x20
#define IOS_PB3_PRESSED 0x40
#define IOS_PB3_RELEASED 0x80
#define IOS_PB3_DEBOUNCE 0x100

#define __set_bits(r,b) (r |= (b))
#define __clear_bits(r,b) (r &= (~(b)))
#define __read_bits(r,b) (r & b)

///////// Global Variables /////////////////

extern uint16_t IOS_FLAGS;

void IOinit();
void IO_LED();

#endif XC_HEADER_TEMPLATE_H

