###############################################################################
# Makefile for the project Wireless-RGB
###############################################################################

## General Flags
PROJECT = SWUSART-TESTER
MCU = atmega128
TARGET = swusart_tester.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99            -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Objects that must be built in order to link
OBJECTS = swusart_tester.o swusart.o single_wire_UART.o

## Build
all: $(TARGET) swusart_tester.hex swusart_tester.eep

single_wire_UART.o: singlewire/single_wire_UART.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

swusart.o: swusart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

swusart_tester.o: swusart_tester.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
clean:
	-rm -rf $(OBJECTS) swusart_tester.elf dep/* swusart_tester.hex swusart_tester.eep

# Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

install:
	avrprog2 --flash w:swusart_tester.hex -v


