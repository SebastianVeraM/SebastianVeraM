/*
 * ThaiKauri_ATTiny85.cpp
 *
 * Created: 30/07/2022 06:43:11 p. m.
 * Author : Sebastian Vera Morales
* MC: ATTiny85
* LED strip: Model 5050 (Common Anode)
* Transistor: IRFZ44N
 */ 

#define F_CPU 8000000UL								// Defining a 8 MHz clock frequency 
#include <avr/io.h>
#include <avr/interrupt.h>		
#include <util/delay.h>

void brightnessLED();
void fireSpear();
void waterShield();
void grassMask();
void madness();

ISR (PCINT0_vect)									
{											
	if (PINB & (1 << PINB2))
	{
		while(PINB & (1 << PINB2))
		{
			fireSpear();
		}
	}
	else if (PINB & (1 << PINB3))
	{
		while(PINB & (1 << PINB3))
		{
			grassMask();
		}
	}
	/*
		else if (PINB & (1 << PINB5))
	{
		while(PINB & (1 << PINB5))
		{
			waterShield();
		}
	}
	else if (PINA & ((1 << PINA0)&(1 << PINA1)&(1 << PINA2)))
	{
		while(PINA & ((1 << PINA0)&(1 << PINA1)&(1 << PINA2)))
		{
			madness();
		}
	}
*/
	reti();
}
void config()
{
	// Each register controls an specific pins PB0, PB1 and PB4 will be used as PWM outputs. OC0A, OC0B and 0C1B
	// Each timer configuration is independent, so (the prescaler, the waveform and the compare output mode) for every timer to be used
	// Timer0 configurations is different from Timer1
	cli();											// Clear interruptions
	
	// Timer0 configuration
	// TCCR0A register configuration
	TCCR0B |= (0<<WGM02);							// The fast PWM waveform was chosen
	TCCR0A |= (1<<WGM01)|(1<<WGM00);
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0);				// Inverting compare match for OC0A
	TCCR0A |= (1<<COM0B1)|(0<<COM0B0);				// Inverting compare match for OC0B
	TCCR0B |= (0<<CS02)|(0<<CS01)|(1<<CS00);		// No prescaler was chosen
	
	// Timer1 configuration
	// GTCCR and TCCR1 register configuration
	GTCCR |= (1<<PWM1B);							// Enables the PWM output
	GTCCR |= (0<<COM1B1)|(1<<COM1B0);				// Inverting compare match for OC1B
	TCCR1 |= (0<<CS12)|(0<<CS11)|(1<<CS10);			// No prescaler was chosen
	
	// OCR registers settings
	OCR0A = 0;										// Register where the value to compare with TCNT0 is stored and sent to OC0A -> PB0
	OCR0B = 0;										// Register where the value to compare with TCNT0 is stored and sent to OC0B -> PB1
	OCR1B = 0;										// Register where the value to compare with TCNT1 is stored and sent to OC1B -> PB4
	
	// Pin Change Interrupts configuration
	GIMSK |= (1<<PCIE);								// Pin Change Interrupts from vector PCIE are enabled, where PCINT2,PCINT3,PCINT5 belong
	PCMSK |= (1<<PCINT2)|(1<<PCINT3)|(1<<PCINT5);	// Pin Change Interrupts from PCINT2,PCINT3,PCINT5 are only enabled
	
	sei();											// Set interruptions
	
	DDRB |= (0<<DDB2)|(0<<DDB3)|(0<<DDB5);			// Pins PB2, PB3 and PB5 as inputs
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB4);			// Pins PB0, PB1 and PB4 enabled as PWM outputs
	
}


void loop()
{	
	brightnessLED();
}

void brightnessLED()
{
	int t = 2;
	for (int i = 0;i < 255; i++)					// For cycle to change the led brightness
	{
		OCR0A = i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN	
		OCR1A = i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
	
	for (int i = 255;i > 0; i--)					// For cycle to change the led brightness
	{
		OCR0A = i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
}

void fireSpear()
{
	int t = 2;
	for (int i = 0;i < 255; i++)					// For cycle to change the led brightness
	{
		OCR0A = 0;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = 255;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
	
	for (int i = 255;i > 0; i--)					// For cycle to change the led brightness
	{
		OCR0A = 255 - i;							// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = 255;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
}
void waterShield()
{
	int t = 2;
	for (int i = 0;i < 255; i++)					// For cycle to change the led brightness
	{
		OCR0A = 255;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = 255 - i;							// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
	
	for (int i = 255;i > 0; i--)					// For cycle to change the led brightness
	{
		OCR0A = 255;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = 0;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
}
void grassMask()
{
	int t = 2;
	for (int i = 0;i < 255; i++)					// For cycle to change the led brightness
	{
		OCR0A = 128;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = 128;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
	
	for (int i = 255;i > 0; i--)					// For cycle to change the led brightness
	{
		OCR0A = 128;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = 128;								// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
}

void madness()
{
	int t = 0;
	for (int i = 0;i < 255; i++)					// For cycle to change the led brightness
	{
		OCR0A = 255 - i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = 128 - i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
	
	for (int i = 255;i > 0; i--)					// For cycle to change the led brightness
	{
		OCR0A = 128 - i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PB2 | BLUE
		OCR0B = 255 - i;									// Register where the compare value is stored and PWM signal will be sent from OC0B -> PA7 | GREEN
		OCR1A = i;									// Register where the compare value is stored and PWM signal will be sent from OC0A -> PA6 | RED
		_delay_ms(t);
	}
}

int main(void)
{
	config();	
	while (1)
	{
		loop();
	}
	return(0);
}