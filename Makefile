ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPRO)
endif
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

ARMIPSFLAGS=
CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
CFLAGS = -mthumb -mthumb-interwork -Wall -Wextra -std=gnu11 -O1 \
		 -Iinclude -I$(DEVKITPRO)/libgba/include -Lgba

ASM = src/symbols/vanilla_labels.asm \
	  src/symbols/randomizer_variables.asm \
	  src/patches.asm \
	  src/hooks.asm \
	  src/limit_abilities.asm \
	  src/data/graphics.asm \
	  src/data/strings.asm

OBJ = obj/init.o \
	  obj/game_loop/passage_select.o \
	  obj/game_loop/level_select.o \
	  obj/game_loop/game_main.o \
	  obj/game_loop/level_results.o \
	  obj/items/collection_indicator.o \
	  obj/items/multiworld.o \
	  obj/items/item_table.o \
	  obj/items/collect_junk.o \
	  obj/shuffle/boxes.o \
	  obj/shuffle/save_data.o \
	  obj/misc.o \
	  obj/graphics.o \

INCLUDE = include/unsorted/functions.h \
          include/unsorted/macros.h \
          include/unsorted/types.h \
          include/unsorted/variables.h \
          include/entity.h \
          include/graphics.h \
          include/item_table.h \
          include/item.h \
          include/multiworld.h \

GRAPHICS = data/graphics/ability_get.bin \
		   data/graphics/ability_icons.bin \
		   data/graphics/ap_logo.bin

.PHONY: all clean debug remake remake-debug

all: basepatch

basepatch: build/basepatch.bsdiff

debug: CFLAGS += -DDEBUG -g
debug: ARMIPSFLAGS += -definelabel DEBUG 1
debug: basepatch

build/basepatch.bsdiff: build/baserom.gba
	bsdiff "Wario Land 4.gba" build/baserom.gba build/basepatch.bsdiff
	grep -Ev '[0-9A-F]{8} [@.].*' build/baserom.sym > build/basepatch.sym

build/baserom.gba: src/basepatch.asm $(ASM) $(OBJ) $(GRAPHICS)
	@mkdir -p build
	armips src/basepatch.asm -sym build/baserom.sym $(ARMIPSFLAGS)

obj/%.o: src/%.c $(INCLUDE)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

data/graphics/%.bin: data/graphics/%.png data/graphics/%.txt
	python3 make_graphics.py $@

remake: clean all

remake-debug: clean debug

clean:
	rm -rf obj build
