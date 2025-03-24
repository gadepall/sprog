;Delay routine for blinking LED
#define __SFR_OFFSET 0
#include <avr/io.h>

.global delay
.section .text

delay:
 ldi r19, 0x40
PAUSE:	;this is delay (function)
lp2:	;loop runs 64 times
		IN r16, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
		ldi r17, 0b00000010
		AND r16, r17 ;need second bit
		BREQ PAUSE 
		OUT TIFR0, r17	;set tifr flag high
	dec r19
	brne lp2
	ret
	ret
	ret
