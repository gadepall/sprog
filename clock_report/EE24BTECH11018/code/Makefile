run:
	avr-gcc -mmcu=atmega328p -Os -c main.c -o code.o
	avr-gcc -mmcu=atmega328p -o code.elf code.o
	avr-objcopy -O ihex code.elf code.hex
