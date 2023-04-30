// Could be defined here, or in the processor defines.
#define SYSTEM_CORE_CLOCK 48000000
#define APB_CLOCK SYSTEM_CORE_CLOCK

#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>

#define WS2812DMA_IMPLEMENTATION
#define NR_LEDS 16

#include "ws2812b_dma_spi_led_driver.h"
#include "color_utilities.h"

#if 1
// Convert from 8-bit to 16 bit values

// table of 8-bit values for conversion
uint32_t clut[] =
{
	0x000000ff,	// Red
	0x0000ff00, // Green
	0x0000ffff, // Yellow
	0x00ff0000, // Blue
	0x00ff00ff,	// Magenta	
	0x00ffff00,	// Cyan
};

// this version converts from 8 to 16 (filling lower 8 with zeros
// and skips the two unused LED positions at locations 2-5
uint32_t dsv;
uint32_t WS2812BLEDCallback( int ledno )
{
	// permute ledno to skip unused 2-5
	if((ledno >1)&&(ledno< 6))
		return 0;
	else if(ledno>5)
		ledno -= 4;
	
	if(!(ledno & 1))
	{
		// new red and green
		dsv = clut[ledno>>1];
		return (dsv&0xff) | ((dsv&0xff00)<<8);
	}
	
	// old blue
	return (dsv&0xff0000)>>8;
}
#else
// step thru table of 16-bit color values for all 8 positions

// without WSRBG set:
// G = word0 bits 23:8, R = word0 bits 7:0, word1 bits 23:16, B = word1 bits 15:0
uint32_t clut[] =
{
	0x000000ff,		// LED 0 word 0	- Bright Red
	0x00ff0000,		// LED 0 word 1
	0x00000000,		// unused
	0x00000000,		// unused
	0x00000000,		// unused
	0x00000000,		// unused
	0x00ffff00,		// LED 1 word 0 - Bright Green
	0x00000000,		// LED 1 word 1
	0x00000000,		// LED 2 word 0 - Bright Blue
	0x0000ffff,		// LED 2 word 1
	0x00000000,		// LED 3 word 0	- Dim Red
	0x00ff0000,		// LED 3 word 1
	0x0000ff00,		// LED 4 word 0	- Dim Green
	0x00000000,		// LED 4 word 1
	0x00000000,		// LED 5 word 0 - Dim Blue
	0x000000ff,		// LED 5 word 1
};

// this version just sends raw data from the LUT
// used for probing the behavior
uint32_t WS2812BLEDCallback( int ledno )
{
	return clut[ledno];
}
#endif


int32_t count = 0;
int main()
{
	SystemInit48HSI();
	
	SetupDebugPrintf();
	printf("\r\n\nws2 test...\n\r");
	
	WS2812BDMAInit( );

	// GPIO C0 Push-Pull
	GPIOC->CFGLR &= ~(0xf<<(4*0));
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*0);

	while(1)
	{
		// Wait for LEDs to totally finish.
		GPIOC->BSHR = 1;
		Delay_Ms( 250 );
		GPIOC->BSHR = (1<<16);
		Delay_Ms( 250 );

		while( WS2812BLEDInUse );

		//count = (count + 1) & 0xff;
		printf("count = %lu\n\r", count);
		
		WS2812BDMAStart( NR_LEDS );
	}
}

