#include "msp430g2553.h"
#include <stddef.h>
#include <string.h>
const char string[] = { "Hello World\r\n" };
unsigned int i = 0; //Counter
char incoming = 'o';
char incomingData[20] ;
char *str;
int receivedData=0;
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop the Watch dog

  //------------------- Configure the Clocks -------------------//

   DCOCTL  = 0;             // Select lowest DCOx and MODx settings
   BCSCTL1 = CALBC1_1MHZ;   // Set range
   DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

  //---------------- Configuring the LED's ----------------------//

   P1DIR |= ((1<<6) | (1<<0)) | ~(1<<3); //0 and 6 output while 3 in input
   P1REN |= (1<<3);            //pull up enabled
   P1OUT |= (1<<3) ; //pull up high
   P1OUT &= ~((1<<0) | (1<<6));

   //---------------------configuring interrupts on port 1 pin3--------------//

   P1IE |= BIT3;  //interrupt on pin1.3 enabled
   P1IES |= BIT3; //high to low edge interrupt
   P1IFG &= ~BIT3; // make sure flag is cleared

  //--------- Setting the UART function for P1.1 & P1.2 --------//

   P1SEL  |=  BIT1 | BIT2;  // P1.1 UCA0RXD input
   P1SEL2 |=  BIT1 | BIT2;  // P1.2 UCA0TXD output

  //------------ Configuring the UART(USCI_A0) ----------------//

   UCA0CTL1 |=  0b11000001;  // USCI Clock = SMCLK,USCI_A0 disabled
   UCA0BR0   =  104;              // 104 From datasheet table-
   UCA0BR1   =  0;                // -selects baudrate =9600,clk = SMCLK
   UCA0MCTL  =  0x02;             // Modulation value = 1 from datasheet UCBRS_1

   //UCA0STAT |=  UCLISTEN;          // loop back mode enabled
    UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0

  //---------------- Enabling the interrupts ------------------//

 //  IE2 |= UCA0TXIE;                  // Enable the Transmit interrupt
   IE2 |= UCA0RXIE; // Enable the Receive  interrupt
 //  _BIS_SR(GIE);
   _BIS_SR(LPM0_bits + GIE);
    // uart_puts("hello\n");
 while(1){

     if(receivedData == 1){
         receivedData = 0;



     }
 }

}
  //-----------------------------------------------------------------------//
  //                Transmit and Receive interrupts                        //
  //-----------------------------------------------------------------------//

#pragma vector = USCIAB0TX_VECTOR
__interrupt void TransmitInterrupt(void)
{
    // UCA0TXBUF = incoming;
    UCA0TXBUF = incomingData[i++];
    if(i == sizeof incomingData - 1)
    {IE2 &= ~UCA0TXIE;i=0; memset(incomingData,0,sizeof(incomingData));}
//  UCA0TXBUF = string[i++]; // TX next character
//  if (i == sizeof string - 1) // TX over?
//     {IE2 &= ~UCA0TXIE;}//disable USCI_A0 TX interrupt



}
//
 #pragma vector = USCIAB0RX_VECTOR
  __interrupt void ReceiveInterrupt(void)
  {
//
//    //incoming = UCA0RXBUF;
      incomingData[i]=UCA0RXBUF;
      i++;
      if(UCA0RXBUF == '\r'){
          receivedData = 1;
          i=0;

          IE2 |= UCA0TXIE;
      }


//    if (UCA0RXBUF == 'a') // 'a' received?
//        {
//           i = 0;
//           UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
//           UCA0TXBUF = string[i++];
//
//        }

  }

#pragma vector = PORT1_VECTOR
__interrupt void port1Interrupt(void)
{
      P1OUT ^= BIT0;
      P1IFG &= ~BIT3;
      //IE2 |= UCA0TXIE;
}
