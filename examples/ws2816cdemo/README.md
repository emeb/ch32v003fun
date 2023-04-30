# WS2816C SPI DMA example
This example demonstrates the use of the 24-bit WS2812b driver to control 48-bit
WS2816c LEDs as found on the ch32v003f4u6_with_wsleds board.

## Detail
The ch32v003f4u6_with_wsleds board include six WS2716c LEDs which provide 16-bit
variable brightness on the R, G and B leds for a total 48-bit frame per device.
It's still possible to communicate with these using the WS2812b driver as long
as you send two 24-bit packets per LED.

When the `WSRBG` macro is not used then the format of the data is as follows:

* 1st packet: 0x00GGGGRR
* 2nd packet: 0x00RRBBBB

per LED, so 12 words total are required to fully update the array of six.

Another peculiarity of the WS2812b driver code is that there are four unused
words in positions 2-5, so the actual data sent to the driver is:

* LED 0 word 0
* LED 0 word 1
* unused
* unused
* unused
* unused
* LED 1 word 0
* LED 1 word 1
* LED 2 word 0
* LED 2 word 1
* LED 3 word 0
* LED 3 word 1
* LED 4 word 0
* LED 4 word 1
* LED 5 word 0
* LED 5 word 1

for a total of 16 words sent to the driver.

## Use
Flash this code to the ch32v003f4u6_with_wsleds board. Observe that the white
PC0 led flashes about 1Hz and the six RGB LEDs light up in six different colors
that slowly rotate around the color wheel.
