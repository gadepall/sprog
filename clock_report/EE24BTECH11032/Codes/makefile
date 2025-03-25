# Target MCU
MCU = atmega328p

# Clock frequency
F_CPU = 16000000UL

# Compiler and linker
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Programmer settings (modify based on your programmer)
PROGRAMMER = arduino
PORT = /dev/ttyUSB0  # Change for Windows (COMx) or Mac (/dev/cu.usbserial-XXXX)

# Compiler flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os 

# Source files
SRC = clock.c
OBJ = $(SRC:.c=.o)
TARGET = clock.elf
HEX = clock.hex

# Default target
all: $(HEX)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the object files into an ELF file
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Convert ELF to HEX for flashing
$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex $(TARGET) $(HEX)

# Flash the program to the microcontroller
flash: $(HEX)
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(HEX):i

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET) $(HEX)
