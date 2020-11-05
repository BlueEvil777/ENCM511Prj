#include "IOs.h"
#include "TimeDelay.h"

#define DEBOUNCE_COUNT 30 //ms

uint16_t IOS_FLAGS;

uint16_t btn1 = 0, btn2 = 0, btn3 = 0;

void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void){
    
    if(IFS1bits.CNIF == 1)
    {
        if(PORTAbits.RA2 == 0 && __read_bits(IOS_FLAGS, IOS_PB1_PRESSED) == 0 && __read_bits(IOS_FLAGS, IOS_PB1_RELEASED) == 0)
        {
            __set_bits(IOS_FLAGS,IOS_PB1_PRESSED);
        }
        else if(PORTAbits.RA2 == 1 && __read_bits(IOS_FLAGS, IOS_PB1_PRESSED) == 0 && __read_bits(IOS_FLAGS, IOS_PB1_RELEASED) == 0)
        {
            __set_bits(IOS_FLAGS,IOS_PB1_RELEASED);
        }
        else if(PORTAbits.RA4 == 0 && __read_bits(IOS_FLAGS, IOS_PB1_PRESSED) == 0 && __read_bits(IOS_FLAGS, IOS_PB2_RELEASED) == 0)
        {
            __set_bits(IOS_FLAGS,IOS_PB2_PRESSED);
        }
        else if(PORTAbits.RA4 == 1 && __read_bits(IOS_FLAGS, IOS_PB1_PRESSED) == 0 && __read_bits(IOS_FLAGS, IOS_PB2_RELEASED) == 0)
        {
            __set_bits(IOS_FLAGS,IOS_PB2_RELEASED);
        }
        else if(PORTBbits.RB4 == 0 && __read_bits(IOS_FLAGS, IOS_PB1_PRESSED) == 0 && __read_bits(IOS_FLAGS, IOS_PB3_RELEASED) == 0)
        {
            __set_bits(IOS_FLAGS,IOS_PB3_PRESSED);
        }
        else if(PORTBbits.RB4 == 1 && __read_bits(IOS_FLAGS, IOS_PB1_PRESSED) == 0 && __read_bits(IOS_FLAGS, IOS_PB3_RELEASED) == 0)
        {
            __set_bits(IOS_FLAGS,IOS_PB3_RELEASED);
        }
        
        IFS1bits.CNIF = 0;
    }
}

void IOinit(){
    TRISAbits.TRISA2 = 1;//Sets RA2 as input for button one
    TRISAbits.TRISA4 = 1;//Sets RA4 as input for button two
    TRISBbits.TRISB4 = 1;//Sets RB4 as input for button three
    TRISBbits.TRISB8 = 0;//sets RB8 as output for LED
    
    CNPU1bits.CN0PUE = 1; //enables pull-up on RA4 (pin 9)
    CNPU1bits.CN1PUE = 1; //enables pull-up on RB4 (pin 10)
    CNPU2bits.CN30PUE = 1; //enables pull-up on RA2 (PIN 7)
}

void IOcheckFlags()
{
    if(__read_bits(IOS_FLAGS,IOS_PB1_DEBOUNCE))
    {
        __clear_bits(IOS_FLAGS,IOS_PB1_DEBOUNCE|IOS_PB1_PRESSED|IOS_PB1_RELEASED);
    }
    else if(__read_bits(IOS_FLAGS,IOS_PB2_DEBOUNCE))
    {
        __clear_bits(IOS_FLAGS,IOS_PB2_DEBOUNCE|IOS_PB2_PRESSED|IOS_PB2_RELEASED);
    }
    else if(__read_bits(IOS_FLAGS,IOS_PB3_DEBOUNCE))
    {
        __clear_bits(IOS_FLAGS,IOS_PB3_DEBOUNCE|IOS_PB3_PRESSED|IOS_PB3_RELEASED);
    }
    else
    {
        if(__read_bits(IOS_FLAGS,IOS_PB1_PRESSED) | __read_bits(IOS_FLAGS,IOS_PB1_RELEASED))
        {
            if(__read_bits(IOS_FLAGS,IOS_PB1_PRESSED))
            {
                btn1 = 1;
            }
            else
            {
                btn1 = 0;
            }
            __set_bits(IOS_FLAGS,IOS_PB1_DEBOUNCE);
            Delay_time(DEBOUNCE_COUNT);
        }
        else if(__read_bits(IOS_FLAGS,IOS_PB2_PRESSED) | __read_bits(IOS_FLAGS,IOS_PB2_RELEASED))
        {
            if(__read_bits(IOS_FLAGS,IOS_PB2_PRESSED))
            {
                btn2 = 1;
            }
            else
            {
                btn2 = 0;
            }
            __set_bits(IOS_FLAGS,IOS_PB2_DEBOUNCE);
            Delay_time(DEBOUNCE_COUNT);
        }
        else if(__read_bits(IOS_FLAGS,IOS_PB3_PRESSED) | __read_bits(IOS_FLAGS,IOS_PB3_RELEASED))
        {
            if(__read_bits(IOS_FLAGS,IOS_PB3_PRESSED))
            {
                btn3 = 1;
            }
            else
            {
                btn3 = 0;
            }
            __set_bits(IOS_FLAGS,IOS_PB3_DEBOUNCE);
            Delay_time(DEBOUNCE_COUNT);
        }
    }
}

void IO_LED(){
    uint16_t ledStatus = 1, delayReduction = 0;
    if(__read_bits(IOS_FLAGS,IOS_PB1_DEBOUNCE | IOS_PB2_DEBOUNCE | IOS_PB3_DEBOUNCE))
    {
        delayReduction = DEBOUNCE_COUNT;
    }
    
    if((btn1) && !(btn2) && !(btn3)){//if only first button is pressed
        LATBbits.LATB8 = ledStatus;//turn led on or off 
        Delay_time(1000-delayReduction);//wait for one second
        ledStatus = !ledStatus;//change led state
    }
    
    else if((btn1) && !(btn2) && !(btn3)){//if only second button is pressed
        LATBbits.LATB8 = ledStatus;//turn led on or off 
        Delay_time(2000-delayReduction);//wait for two seconds
        ledStatus = !ledStatus;//change led state
    }
    
    else if(!(btn1) && (btn2) && !(btn3)){
        LATBbits.LATB8 = ledStatus;//turn led on or off 
        Delay_time(3000-delayReduction);//wait for three seconds
        ledStatus = !ledStatus;//change led state
    }
    //cases where multiple buttons are pressed
    else if(!(btn1) && !(btn2) && (btn3)){//if 2 buttons are pressed and one of which is RA2 button
        LATBbits.LATB8 = 1;//light stays on
    }
    
    else if((PORTAbits.RA4 == 0) && ((PORTAbits.RA2 == 0) || (PORTBbits.RB4 == 0))){//if 2 buttons are pressed and one of which is RA4 button
        LATBbits.LATB8 = 1;//light stays on
    }
    
    else if((PORTAbits.RA2) == 1 && (PORTAbits.RA4) == 1 && (PORTBbits.RB4 == 1)){//if no buttons are pressed
        LATBbits.LATB8 = 0;//light is off
    }
}
