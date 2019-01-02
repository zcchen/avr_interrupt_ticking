# Program Name
PRG             = main

# MCU Setting
#MCU_TARGET      = atmega2560
MCU_TARGET      = atmega328p
F_CPU           = 16000000

# MCU Fuse setting
#MCU_HFUSE       = 0xD8
    # for arduino mega2560
MCU_HFUSE       = 0xDE
    # for arduino uno
MCU_LFUSE       = 0xFF
MCU_EFUSE       = 0xFD
    # BOD, typical: 2.7V

# Upload Setting
#PROGRAMMER      = usbasp       # for atmega2560
PROGRAMMER      = arduino     # for atmega328p
#PROGRAMMER      = wiring       # for atmega2560
BAUDRATE        = 115200
PORT_NUM        = /dev/ttyACM0

# Code Setting
BUILD_DIR       = build
SRC_DIR         = src
HEAD_DIR        = include
TEST_DIR        = tests

PRG_SOURCE      = $(SRC_DIR)/$(PRG).c
LIB_SOURCES     = $(filter-out $(PRG_SOURCE), $(wildcard $(SRC_DIR)/*.c))
TEST_SOURCES    = $(addprefix $(TEST_DIR)/, $(TEST).c)

PRG_OBJECT      = $(BUILD_DIR)/$(PRG).o
LIB_OBJECTS     = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SOURCES))
TEST_OBJECTS    = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test-%.o, $(TEST_SOURCES))

# AVR Compiler Setting
CC_PREFIX       = avr-
CFLAGS_AVR      = -mmcu=$(MCU_TARGET) -DF_CPU=$(F_CPU)UL
LDFLAGS_AVR     = 
AVRDUDE         = avrdude -v -C /etc/avrdude.conf -c ${PROGRAMMER} \
		  -P ${PORT_NUM} -p ${MCU_TARGET} -b ${BAUDRATE}

# Compiling Setting
OPTIMIZE        = -Os
DEFS            =
LIBS            =
CC              = $(CC_PREFIX)gcc
CXX             = $(CC_PREFIX)g++
OBJCOPY         = $(CC_PREFIX)objcopy
OBJDUMP         = $(CC_PREFIX)objdump
STD_FLAGS       = -std=c99
CXXFLAGS        =
override CFLAGS  = -g -Wall $(STD_FLAGS) $(OPTIMIZE) $(DEFS) -I${HEAD_DIR} $(CFLAGS_AVR) -Werror=overflow
override LDFLAGS = -Wl,-Map,${BUILD_DIR}/$(PRG).map

TEST            := 

.PHONY: all test test_help test_setting clean build-dir upload clean fuse

all: build-dir $(PRG).elf $(PRG).hex

test: test_help build-dir test_setting testing

$(PRG).hex: $(PRG).elf
	$(OBJCOPY) -O ihex -R .eeprom $(BUILD_DIR)/$(^) $(BUILD_DIR)/$(@)
$(PRG).elf: $(PRG_OBJECT) $(LIB_OBJECTS)
	$(CC) $(CFLAGS) $(STD_FLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$(@) $(^)

test_help:
	@echo "-------- Make test Help --------"
	@echo "make test TEST=<test_filename>"
	@echo "================================"

test_setting:
	$(eval CC_PREFIX := )
	$(eval CFLAGS_AVR := -DDEBUG)
	$(eval LDFLAGS_AVR := )
testing: $(LIB_OBJECTS) $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(STD_FLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$(@) $^

$(BUILD_DIR)/test-%.o: $(TEST_DIR)/%.c
	#@$(CC) -E $(CFLAGS) $(CCFLAGS) $(LDFLAGS) -c -o $(@).E $(<)
	$(CC) $(CFLAGS) $(STD_FLAGS) $(LDFLAGS) -c -o $(@) $(<)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	#@$(CC) -E $(CFLAGS) $(CCFLAGS) $(LDFLAGS) -c -o $(@).E $(<)
	$(CC) $(CFLAGS) $(STD_FLAGS) $(LDFLAGS) -c -o $(@) $(<)

build-dir:
	-mkdir -p ${BUILD_DIR}

clean:
	-rm -rf ${BUILD_DIR}

upload: $(PRG).hex #fuse
	$(AVRDUDE) -U flash:w:./${BUILD_DIR}/$(PRG).hex:i

fuse:
	$(AVRDUDE) -U lfuse:w:$(MCU_LFUSE):m -U hfuse:w:$(MCU_HFUSE):m -U efuse:w:$(MCU_EFUSE):m
