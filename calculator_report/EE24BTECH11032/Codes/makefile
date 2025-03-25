# MCU and Programmer Configuration
MCU = atmega328p              # Change this according to your microcontroller
F_CPU = 16000000UL            # Set the CPU frequency
PROGRAMMER = arduino          # Change this based on your programmer
PORT = /dev/ttyUSB0           # Change this to match your serial port (e.g., COMx on Windows, /dev/ttyUSBx on Linux)

# Compiler and Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Compiler Flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os
LDFLAGS = -mmcu=$(MCU)

# Source Files
SRC = calc.c lcd.c  # Add lcd.c if it's a separate file
OBJ = $(SRC:.c=.o)
TARGET = calculator

# Compilation Rule
all: $(TARGET).hex

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flash the program using avrdude
flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(TARGET).hex:i

# Clean compiled files
clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex


