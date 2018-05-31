#include "msp430g2553.h"
unsigned int i = 0;
    void main(void)
    {
      WDTCTL = WDTPW + WDTHOLD; // Stop the Watch dog

      //------------------- Configure the Clocks -------------------//

      if (CALBC1_1MHZ==0xFF)   // If calibration constant erased
         {
            while(1);          // do not load, trap CPU!!
         }

      DCOCTL  = 0;             // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;   // Set range
      DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

      //---------------- Configuring the LED's ----------------------//

      P1DIR  |=  BIT0 ;  // P1.0 output
      P1OUT  &= ~BIT0;  // P1.0 = 0



      //------------ Configuring MAX485 Control Lines ---------------//

      P2DIR  |= BIT0 + BIT1 + BIT2 + BIT3; // P2.0 -> DE,P2.1 -> ~RE output
      P2OUT   = 0x00;        // P2.0 and P2.1 = 0

      //--------- Setting the UART function for P1.1 & P1.2 ---------//

      P1SEL  |=  BIT1 + BIT2;           // P1.1 UCA0RXD input
      P1SEL2 |=  BIT1 + BIT2;           // P1.2 UCA0TXD output

      //------------ Configuring the UART(USCI_A0) ----------------//

      UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
      UCA0BR0   =  104;                 // 104 From datasheet table-
      UCA0BR1   =  0;                   // -selects baudrate =9600,clk = SMCLK
      UCA0MCTL  =  UCBRS_1;             // Modulation value = 1 from datasheet
      UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0
      //---------------- Enabling the interrupts ------------------//

      IE2 |= UCA0RXIE;                  // Enable the  interrupt
      _BIS_SR(GIE);                     // Enable the global interrupt

     //----------------- Put MAX485 in Transmit Mode ---------------//

      P2OUT |= BIT0 + BIT1;  // P2.0 ->  DE high,P2.1 -> ~RE high
//        for(i = 0 ; i<100; i++)
//        {    UCA0TXBUF = 'A';       // Transmit a byte
//        _delay_cycles(500000);
//        }   _BIS_SR(LPM0_bits + GIE); // Going to LPM0
    }

#pragma vector = USCIAB0RX_VECTOR
    __interrupt void ReceiveInterrupt(void)
    {
      P1OUT  ^= BIT0;     //light up P1.0 Led on RX
      IFG2 &= ~UCA0RXIFG; // Clear RX flag
    }
