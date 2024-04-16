NAME		=	main

F_CPU		=	16000000UL
MCU_TYPE	=	atmega328p

CC			=	avr-gcc
CFLAGS		=	-mmcu=$(MCU_TYPE) -DF_CPU=$(F_CPU) -Os
DEPSFLAG	=	-MMD

FILES		=	main

SRC_DIR		=	.
SRC			=	$(addprefix $(SRC_DIR)/, $(addsuffix .c, $(FILES)))

OBJ_DIR		=	obj
OBJ			=	$(addprefix $(OBJ_DIR)/, $(FILES:=.o))
DEPS		=	$(addprefix $(OBJ_DIR)/, $(FILES:=.d))


all: hex flash

clean:
	rm -rf $(OBJ_DIR) $(NAME).bin $(NAME).hex

-include $(DEPS)

hex: $(NAME).hex

$(NAME).hex: $(NAME).bin
	avr-objcopy -O ihex $(NAME).bin $(NAME).hex

$(NAME).bin: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME).bin
	avr-objcopy -O ihex $(NAME).bin $(NAME).hex

flash: hex
	avrdude -p m328p -c arduino -b 115200 -e -u -P /dev/ttyUSB0 -U flash:w:$(NAME).hex:i

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPSFLAG) -c $< -o $@

.PHONY : all hex clean flash