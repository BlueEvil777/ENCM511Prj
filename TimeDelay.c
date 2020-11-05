/* 
 * File:   TimeDelay.c
 * Author: lukap
 *
 * Created on October 19, 2020, 1:39 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "TimeDelay.h"

//method to start a timer for time_ms milliseconds
void Delay_time(uint16_t time_ms){
    T2CONbits.T32 = 1;//allows Timer2 and Timer3 to form a 32-bit timer
    T2CONbits.TSIDL = 0;//allows timer to continue while cpu is in idle mode.
    
    IPC1bits.T2IP = 7;//sets Timer2 interrupt priority to high
    IEC0bits.T2IE = 1;//enables timer interrupt
    IFS0bits.T2IF = 0;//clears Timer2 interrupt flag
    
    PR2 = time_ms * 16;//set PR2 to the appropriate number of clock cycles (PR2 = (time_ms * fclk)/ 2*1000)
    T2CONbits.TON = 1;//Starts 32 bit Timer2/3
    Idle();//cpu idles until interrupt
    return;
}
    
//Timer 2 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;//clears Timer2 interrupt flag
    T2CONbits.TON = 0;//turn off Timer2
    return;
}



