/*
 * EggTimer.c
 *
 * Created: 13/04/2022 10:53:07 pm
 * Author : qhs7289
 */ 


#include <avr/io.h>//AT90 Library
#include <avr/interrupt.h>//Interrupt library

#define startButton !(PINA & (1<<0))//Poll if push button is pushed
#define stopButton !(PINA & (1<<1))//Poll if push button is pushed
#define beeperON (PORTE |= (1<<2))//Turn beeper on
#define beeperOFF (PORTE &= ~(1<<2))//Turn beeper off
#define LED_on (PORTC |= (1<<0))//Turn LED on
#define LED_off (PORTC &= ~(1<<0))//Turn LED off

#define start 1//State 1
#define stop 0//State 2

unsigned char ticker, timer = 15;//counters for timing operations
char eggtimer = stop;//declare variable and set to the stop state

void setup(void);//Initialize setup function
ISR(TIMER0_COMPA_vect);//Initialize interrupt

int main(void)//Main function
{
	setup();//Run the setup once
    while (1)//Infinite loop
    {
		if(startButton)//If the start button is pressed
			eggtimer = start;//Set the state to start timer
		if(stopButton)//If the stop button is pressed
		{
			beeperOFF;//Turn the beeper off
			eggtimer = stop;//Set the time state to stop
			timer = 15;//Reset timer
		}
	}
}

void setup(void)//Setup function
{	
	DDRC = 0xFF;//Set first LED to an output
	DDRE = 0x03;//Set bit 0 and 1 to inputs
	PORTE = 0x01;//Set to buttons
	TIMSK0 = 0x02;//Enable output compare A interrupt
	TCCR0B = 0x04;//Set prescaler to 256
	sei();//Enable global interrupts
}

ISR(TIMER0_COMPA_vect)//Output compare A interrupt
{
	OCR0A = OCR0A + 250;//Have OCR0A set to 250 from previous value
	if (eggtimer == start)//If the egg timer is in the start state
	{
		ticker++;//increase ticker
		if(ticker > 110)//For the short blinking of the led @120ms every second
			LED_on;//Turn LED on
		if (ticker > 125)
			LED_off;//Turn LED off
		if (ticker == 125)//If the ticker is 125, one second has passed
			timer--;//Decrement timer by 1
		if(timer < 6 && ticker > 110)//When the timer is less than 6 seconds, and keeping the same beeper/led intervals
		{
			beeperON;//Turn beeper on
			if(timer < 1)//If timer is less than 1
			beeperON;//Keep the beeper on
			else
			if(ticker > 124)//Otherwise turn it off
			beeperOFF;
		}
	}
}