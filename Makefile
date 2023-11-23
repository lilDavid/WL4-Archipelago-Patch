ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPRO)
endif
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

ARMIPSFLAGS =
CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
CFLAGS = -mthumb -mthumb-interwork -Wall -Wextra -std=gnu11 -O1 \
		 -Iinclude -I$(DEVKITPRO)/libgba/include -Lgba

SRC = src
SRCS_ASM = $(shell find $(SRC) -type f -name '*.asm')
SRCS_C = $(shell find $(SRC) -type f -name '*.c')

OBJ = obj
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS_C))

INCLUDE = include
HEADERS = $(shell find $(INCLUDE) -type f -name '*.h')

DATA = data
PNGS = $(shell find $(DATA) -type f -name '*.png')
BINS = $(patsubst $(DATA)/%.png, $(DATA)/%.bin, $(PNGS))

.PHONY: all clean debug remake remake-debug

all: build/basepatch.bsdiff

debug: CFLAGS += -DDEBUG -g
debug: ARMIPSFLAGS += -definelabel DEBUG 1
debug: build/basepatch.bsdiff

build/basepatch.bsdiff: build/baserom.gba
	bsdiff "Wario Land 4.gba" build/baserom.gba build/basepatch.bsdiff
	grep -Ev '[0-9A-F]{8} [@.].*' build/baserom.sym > build/basepatch.sym

build/baserom.gba: $(SRCS_ASM) $(OBJS) $(BINS)
	@mkdir -p build
	armips src/basepatch.asm -sym build/baserom.sym $(ARMIPSFLAGS)

obj/%.o: src/%.c $(HEADERS)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

data/graphics/%.bin: data/graphics/%.png data/graphics/%.txt
	python3 make_graphics.py $@

remake: clean all

remake-debug: clean debug

clean:
	rm -rf obj build
