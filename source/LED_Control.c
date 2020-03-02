#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "LED_Control.h"

// Macros
#define SCAN_OFFSET 544               // Offset for scan range
#define SCAN_DATA TSI0->DATA & 0xFFFF // Accessing the bits held in TSI0_DATA_TSICNT
#define MASK(x) (1UL << (x))

#define RED_LED_SHIFT   (18)	// on port B (PTB18)
#define GREEN_LED_SHIFT (19)	// on port B (PTB19)
#define BLUE_LED_SHIFT  (1)		// on port D (PTB1)

#define RED  (0)	// on port B (PTB18)
#define GREEN (1)	// on port B (PTB19)
#define BLUE  (2)  // on port D (PTB1)


void KL25Z_RGB_Flasher(void)
{
	// Enable clock to Port B and Port D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Make 3 pins GPIO
	//First pin to GPIO
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(1);

	//Second pin to GPIO
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(1);

	//Third pin to GPIO
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(1);

	// Set ports to outputs
	PTB->PDDR |= (MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT));
	PTD->PDDR |= MASK(BLUE_LED_SHIFT);

	//turn every light on
	PTB->PCOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PCOR |= MASK(BLUE_LED_SHIFT);

	//turn every led off
	PTB->PSOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PSOR |= MASK(BLUE_LED_SHIFT);
}

void delay(void)
{
	//(ms*cpufreq_Mhz*1000)/10.. CPUfreq = 48MHz

	int loopcount = 14000000;
	while(1)  //always true
	{
		loopcount--;  //decrement duration
		if(loopcount <= 0) //time reached
			break;
	}
}


void LED_off()
{
//turn every led off
	PTB->PSOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PSOR |= MASK(BLUE_LED_SHIFT);
}

//Turn Green On
void LED_cyclegreen_On()
{
	LED_off();
	PTB->PCOR = MASK(GREEN_LED_SHIFT);//on
}

//Turn Red On
void LED_cyclered_On()
{
	LED_off();
	PTB->PCOR = MASK(RED_LED_SHIFT);//on
}

//Turn Blue On
void LED_cycleblue_On()
{
	LED_off();
	PTD->PCOR = MASK(BLUE_LED_SHIFT);//on;
}


