;Delay routine for blinking LED
#define __SFR_OFFSET 0
#include <avr/io.h>

.global SendByte
.global LCD_Cmd
.global LCD_Char
.global SendNibble
.global LCD_Init
.section .text


LCD_Init:
 ldi r24, 0x33
 rcall LCD_Cmd
 ldi r24, 0x32
 rcall LCD_Cmd
 ldi r24, 0x28
 rcall LCD_Cmd
 ldi r24, 0x0C
 rcall LCD_Cmd
 ldi r24, 0x06
 rcall LCD_Cmd
 ldi r24, 0x01
 rcall LCD_Cmd
 ret
 
  
LCD_Char:
 sbi PORTB, 0
 rcall SendByte
 ret


LCD_Cmd:
 cbi PORTB, 0
 rcall SendByte
 ret
 
 
SendByte:
 rcall SendNibble
 lsl r24
 lsl r24
 lsl r24 
 lsl r24
 rcall SendNibble
 cbi PORTB, 5
 ret
 
 
 
SendNibble:
 
 mov r16, r24
 in r17, PORTB
 andi r17, 0xc3
 out PORTB, r17
 
 
 sbrc r16, 4  
 sbi PORTB, 2          ; Set PB2 (D4)

 sbrc r16, 5         
 sbi PORTB, 3          ; Set PB3 (D5)

 sbrc r16, 6         
 sbi PORTB, 4       

 sbrc r16, 7       
 sbi PORTB, 5        
 
 sbi PORTB, 1     
 rcall PulseEnableLine
 ret
 
