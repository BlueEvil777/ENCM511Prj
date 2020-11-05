#include "IOs.h"
#include "TimeDelay.h"
#include "xc.h"

void IOinit(){
    TRISAbits.TRISA2 = 1;//Sets RA2 as input for button one
    TRISAbits.TRISA4 = 1;//Sets RA4 as input for button two
    TRISBbits.TRISB4 = 1;//Sets RB4 as input for button three
    TRISBbits.TRISB8 = 0;//sets RB8 as output for LED
    
    CNPU1bits.CN0PUE = 1; //enables pull-up on RA4 (pin 9)
    CNPU1bits.CN1PUE = 1; //enables pull-up on RB4 (pin 10)
    CNPU2bits.CN30PUE = 1; //enables pull-up on RA2 (PIN 7)
}

void IOcheck(){
    uint16_t ledStatus = 1;
    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)){//if only first button is pressed
        LATBbits.LATB8 = ledStatus;//turn led on or off 
        Delay_time(1000);//wait for one second
        ledStatus = !ledStatus;//change led state
    }
    
    while((PORTAbits.RA2 == 1) && (PORTAbits.RA4) == 0 && (PORTBbits.RB4 == 1)){//if only second button is pressed
        LATBbits.LATB8 = ledStatus;//turn led on or off 
        Delay_time(2000);//wait for two seconds
        ledStatus = !ledStatus;//change led state
    }
    
    while((PORTAbits.RA2 == 1) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 0)){
        LATBbits.LATB8 = ledStatus;//turn led on or off 
        Delay_time(3000);//wait for three seconds
        ledStatus = !ledStatus;//change led state
    }
    //cases where multiple buttons are pressed
    while((PORTAbits.RA2 == 0) && ((PORTAbits.RA4 == 0) || (PORTBbits.RB4 == 0))){//if 2 buttons are pressed and one of which is RA2 button
        LATBbits.LATB8 = 1;//light stays on
    }
    
    while((PORTAbits.RA4 == 0) && ((PORTAbits.RA2 == 0) || (PORTBbits.RB4 == 0))){//if 2 buttons are pressed and one of which is RA4 button
        LATBbits.LATB8 = 1;//light stays on
    }
    
    if((PORTAbits.RA2) == 1 && (PORTAbits.RA4) == 1 && (PORTBbits.RB4 == 1)){//if no buttons are pressed
        LATBbits.LATB8 = 0;//light is off
    }
    else{
        LATBbits.LATB8 = 0;//light is off
    }
}
