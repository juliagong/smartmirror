NAME = main
SMART_DISPLAY = smart_display
OBJECTS = custom_font_draw.o font.o module.o module_display.o output_formatter.o profile.o sensors.o settings.o
HEADER_DIR = ./headers/ 

CFLAGS  = -I$(CS107E)/include -I$(HEADER_DIR) -g -Wall -Wpointer-arith
CFLAGS += -O3 -std=c99 -ffreestanding
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -mpoke-function-name
LDFLAGS = -nostdlib -T memmap -L. -L$(CS107E)/lib
LDLIBS  = -lmypi -lpi -lgcc

all : $(NAME).bin $(OBJECTS)

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(OBJECTS) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

%.list: %.o
	arm-none-eabi-objdump  --no-show-raw-insn -d $< > $@

install: $(NAME).bin
	rpi-install.py -p $<

clean:
	rm -f *.o *.bin *.elf *.list *~

testmodule: tests/test_module.bin
	rpi-install.py -p $<

testprofile: tests/test_profile.bin
	rpi-install.py -p $<

testdisplay: tests/test_module_display.bin
	rpi-install.py -p $<

testsensors: tests/test_sensors.bin
	rpi-install.py -p $<

testsettings: tests/test_settings.bin
	rpi-install.py -p $<

smartdisplay: $(SMART_DISPLAY).bin
	rpi-install.py -p $<

.PHONY: all clean install

.PRECIOUS: %.o %.elf

# empty recipe used to disable built-in rules for native build
%:%.c
%:%.o

define CS107E_ERROR_MESSAGE
ERROR - CS107E environment variable is not set.

Please set it to point to the `cs107e.github.io/cs107e` directory using the
command `export CS107E=<replace with path to your cs107e.github.io directory>/cs107e`.

To have this done automatically, add the above command to your shell
environment configuration file (e.g. ~/.bashrc)
endef

ifndef CS107E
$(error $(CS107E_ERROR_MESSAGE))
endif
