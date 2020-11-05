#include "IOs.h"
#include "TimeDelay.h"

#define DEBOUNCE_COUNT 30 //ms

uint16_t IOS_FLAGS = 0;

uint16_t btn1 = 0, btn2 = 0, btn3 = 0;

uint16_t delayReduction = 0;

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {

    if (IFS1bits.CNIF == 1) {
//        if (PORTAbits.RA2 == 0 && __read_bits(IOS_FLAGS, 0, IOS_PB1_PRESSED | IOS_PB1_DEBOUNCE) == 0) {
//            __set_bits(IOS_FLAGS, 0, IOS_PB1_PRESSED | IOS_PB1_DEBOUNCE);
//        } else if (PORTAbits.RA2 == 1 && __read_bits(IOS_FLAGS, 0, IOS_PB1_PRESSED | IOS_PB1_DEBOUNCE) == IOS_PB1_PRESSED) {
//            __set_bits(IOS_FLAGS, 0, IOS_PB1_DEBOUNCE);
//            __clear_bits(IOS_FLAGS, 0, IOS_PB1_PRESSED);
//        } else if (PORTAbits.RA4 == 0 && __read_bits(IOS_FLAGS, 0, IOS_PB2_PRESSED | IOS_PB2_DEBOUNCE) == 0) {
//            __set_bits(IOS_FLAGS, 0, IOS_PB2_PRESSED | IOS_PB2_DEBOUNCE);
//        } else if (PORTAbits.RA4 == 1 && __read_bits(IOS_FLAGS, 0, IOS_PB2_PRESSED | IOS_PB2_DEBOUNCE) == IOS_PB2_PRESSED) {
//            __set_bits(IOS_FLAGS, 0, IOS_PB2_DEBOUNCE);
//            __clear_bits(IOS_FLAGS, 0, IOS_PB2_PRESSED);
//        } else if (PORTBbits.RB4 == 0 && __read_bits(IOS_FLAGS, 0, IOS_PB3_PRESSED | IOS_PB3_DEBOUNCE) == 0) {
//            __set_bits(IOS_FLAGS, 0, IOS_PB3_PRESSED | IOS_PB3_DEBOUNCE);
//        } else if (PORTBbits.RB4 == 1 && __read_bits(IOS_FLAGS, 0, IOS_PB3_PRESSED | IOS_PB3_DEBOUNCE) == IOS_PB3_PRESSED) {
//            __set_bits(IOS_FLAGS, 0, IOS_PB3_DEBOUNCE);
//            __clear_bits(IOS_FLAGS, 0, IOS_PB3_PRESSED);
//        }
        if(((PORTAbits.RA2 && __read_bits(IOS_FLAGS,0,IOS_PB1_PRESSED))||!(PORTAbits.RA2 || __read_bits(IOS_FLAGS,0,IOS_PB1_PRESSED))) && __read_bits(IOS_FLAGS,0,IOS_PB1_DEBOUNCE) == 0)
        {
            __set_bits(IOS_FLAGS, 0, IOS_PB1_DEBOUNCE);
        }
        else if(((PORTAbits.RA4 && __read_bits(IOS_FLAGS,0,IOS_PB2_PRESSED))||!(PORTAbits.RA4 || __read_bits(IOS_FLAGS,0,IOS_PB2_PRESSED))) && __read_bits(IOS_FLAGS,0,IOS_PB2_DEBOUNCE) == 0)
        {
            __set_bits(IOS_FLAGS, 0, IOS_PB2_DEBOUNCE);
        }
        else if(((PORTBbits.RB4 && __read_bits(IOS_FLAGS,0,IOS_PB3_PRESSED))||!(PORTBbits.RB4 || __read_bits(IOS_FLAGS,0,IOS_PB3_PRESSED))) && __read_bits(IOS_FLAGS,0,IOS_PB3_DEBOUNCE) == 0)
        {
            __set_bits(IOS_FLAGS, 0, IOS_PB3_DEBOUNCE);
        }

        IFS1bits.CNIF = 0;
    }
}

void IOinit() {
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital

    TRISAbits.TRISA2 = 1; //Sets RA2 as input for button one
    TRISAbits.TRISA4 = 1; //Sets RA4 as input for button two
    TRISBbits.TRISB4 = 1; //Sets RB4 as input for button three
    TRISBbits.TRISB8 = 0; //sets RB8 as output for LED

    CNPU1bits.CN0PUE = 1; //enables pull-up on RA4 (pin 9)
    CNPU1bits.CN1PUE = 1; //enables pull-up on RB4 (pin 10)
    CNPU2bits.CN30PUE = 1; //enables pull-up on RA2 (PIN 7)

    IEC1bits.CNIE = 1;
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;
    CNEN2bits.CN30IE = 1;
    IPC4bits.CNIP = 1;
}

static void debounceBtn(const uint16_t btn_num) {
    uint16_t flagOffset = IOS_PB_FLAG_OFFSET + (btn_num - 1) * IOS_PB_FLAG_WIDTH;
    if (__read_bits(IOS_FLAGS, flagOffset, IOS_PB_DEBOUNCE)) {
        //Delay_time(DEBOUNCE_COUNT);
        __clear_bits(IOS_FLAGS, flagOffset, IOS_PB_DEBOUNCE);
        delayReduction += DEBOUNCE_COUNT;
        
        char btn_port = 0;
        switch(btn_num)
        {
            case 1:
                btn_port = !PORTAbits.RA2;
                break;
            case 2:
                btn_port = !PORTAbits.RA4;
                break;
            case 3:
                btn_port = !PORTBbits.RB4;
                break;
        }
        if(btn_port)
        {
            __set_bits(IOS_FLAGS,flagOffset,IOS_PB_PRESSED);
        }
        else
        {
            __clear_bits(IOS_FLAGS,flagOffset,IOS_PB_PRESSED);
        }
    }
}

void IOcheckFlags() {

    if (__read_bits(IOS_FLAGS, 0, IOS_PB1_DEBOUNCE | IOS_PB2_DEBOUNCE | IOS_PB3_DEBOUNCE)) {
        __set_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
        debounceBtn(1);
        debounceBtn(2);
        debounceBtn(3);
        
    }
}

void IO_LED() {

    switch (__read_bits(IOS_FLAGS, 0, IOS_PB1_PRESSED | IOS_PB2_PRESSED | IOS_PB3_PRESSED)) {
        case IOS_PB1_PRESSED://if only first button is pressed
            LATBbits.LATB8 = (PORTBbits.RB8 == 0); //turn led on or off 
            //Delay_time(500 - delayReduction); //wait for one second
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            break;

        case IOS_PB2_PRESSED:
            LATBbits.LATB8 = (PORTBbits.RB8 == 0); //turn led on or off 
            //Delay_time(2000 - delayReduction); //wait for two seconds
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            break;

        case IOS_PB3_PRESSED:
            LATBbits.LATB8 = (PORTBbits.RB8 == 0); //turn led on or off 
            //Delay_time(3000 - delayReduction); //wait for three seconds
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            break;

        case(IOS_PB1_PRESSED | IOS_PB2_PRESSED):
            LATBbits.LATB8 = 1; //light stays on
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            break;

        case(IOS_PB1_PRESSED | IOS_PB3_PRESSED):
            LATBbits.LATB8 = 1; //light stays on
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            break;

        case(IOS_PB2_PRESSED | IOS_PB3_PRESSED):
            LATBbits.LATB8 = 1; //light stays on
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            break;

        default:
            LATBbits.LATB8 = 0;
            if (__read_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE)) {

                __clear_bits(IOS_FLAGS, 0, IOS_NEW_BUTTON_STATE);
            }
            if ((IOS_FLAGS & IOS_PB_FLAGS) == 0) {
                Idle();
            }
    }
    delayReduction = 0;
}
