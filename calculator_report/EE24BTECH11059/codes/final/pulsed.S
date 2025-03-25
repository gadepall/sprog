;Delay routine for blinking LED
#define __SFR_OFFSET 0
#include <avr/io.h>

.global PulseEnableLine
.section .text

PulseEnableLine:
    dec r18
    brne PulseEnableLine
    cbi PORTB, 1        
    ret
 
