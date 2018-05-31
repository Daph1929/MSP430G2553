#include <msp430g2553.h>


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= ((1<<6) | (1<<0));
    P1REN |= (1<<3);
    //printf("entered condition");

//    P1DIR = 0X41;    //Declare PIN0 AND PIN1 OF PORT 1 AS OUTPUT
//
//    P1OUT = 0X01;
    while(1){

//      P1OUT^= 0x41;
//     _delay_cycles(500000);

        if(0x08 & P1IN)
        {
        P1OUT &= ~(1<<6);
        P1OUT |= (1<<0);
               }
        else
        {
        P1OUT |= (1<<6);
        P1OUT &= ~(1<<0);
        }

    }

    return 0;
}
