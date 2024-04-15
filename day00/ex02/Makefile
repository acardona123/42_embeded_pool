
F_CPU		=	16000000UL
MCU_TYPE	=	atmega328p

CC			=	avr-gcc

all: hex flash

hex:
	$(CC) -mmcu=$(MCU_TYPE) -DF_CPU=$(F_CPU) -Os main.c -o main.bin
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -p m328p -c arduino -b 115200 -e -u -P /dev/ttyUSB0 -U flash:w:main.hex:i -v
 

clean:
	rm -rf main.bin main.hex