#include "msp.h"
#include <stdio.h>

/**
 * main.c
 */


int main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P1->DIR = ~(uint8_t) BIT1;  //Set P1.1 as INPUT, the rest as output            //
	P1->REN = BIT1;             //Enable pull-up resistor on P1.1

	P1->IES = BIT1;             //Interrupt on high-to-low transition
	P1->IFG = 0;                //Clear all PORT1 interrupt flags
	P1->IE  = BIT1;             //Enable interrupt for P1.1

	//Enable PORT1 interrupt. Interrupts for the PORT1 are in the NVIC INTERRUPT
	//INPUT #35, and the #35 is in the ISER #1, because each ISER handles 32 bits.
	//The #35 bit in the ISER[1] is in the #3 bit. An easy way to do this is using &
	//and left shift 1
	NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

	__enable_irq();             //Enable global interrupt

	for(;;);                    //Endless loop
}

void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

       // Toggling the output on the LED
       if(P1->IFG & BIT1)
           P1->OUT ^= BIT0;

       // Delay for switch debounce
       for(i = 0; i < 10000; i++)

       P1->IFG &= ~BIT1;
}
