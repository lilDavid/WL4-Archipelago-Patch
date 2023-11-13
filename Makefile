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

ASM = src/util/vanilla_labels.asm \
	  src/util/randomizer_variables.asm \
	  src/util/macros.asm \
	  src/util/bitfields.asm \
	  src/items/item_table.asm \
	  src/items/collect_treasure.asm \
	  src/items/collection_indicator.asm \
	  src/shuffle/randomize_boxes.asm \
	  src/shuffle/limit_abilities.asm \
	  src/shuffle/ability_ui.asm \
	  src/shuffle/save_data.asm \
	  src/game_loop/level_select.asm \
	  src/game_loop/game_main.asm \
	  src/game_loop/level_results.asm \
	  src/init.asm \
	  src/patches.asm \
	  src/lib.asm \
	  src/graphics.asm \
	  src/string_data.asm

OBJ = obj/init.o \
	  obj/game_loop/level_select.o \
	  obj/game_loop/game_main.o \
	  obj/items/multiworld.o \
	  obj/items/item_table.o \
	  obj/items/collect_junk.o \
	  obj/shuffle/save_data.o \

GRAPHICS = data/graphics/ability_get.bin \
		   data/graphics/ability_icons.bin \
		   data/graphics/ap_logo.bin

.PHONY: all clean debug remake remake-debug

all: basepatch

basepatch: out/basepatch.bsdiff

debug: CFLAGS += -DDEBUG -g
debug: ARMIPSFLAGS += -definelabel DEBUG 1
debug: basepatch

out/basepatch.bsdiff: out/baserom.gba
	bsdiff "Wario Land 4.gba" out/baserom.gba out/basepatch.bsdiff

out/baserom.gba: $(ASM) $(OBJ) $(GRAPHICS)
	@mkdir -p out
	armips src/basepatch.asm -sym out/baserom.sym $(ARMIPSFLAGS)
	grep -Ev '[0-9A-F]{8} [@.].*' out/baserom.sym > out/basepatch.sym

obj/%.o: src/%.c include/*.h
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

data/graphics/%.bin: data/graphics/%.png data/graphics/%.txt
	python3 make_graphics.py $@

remake: clean all

remake-debug: clean debug

clean:
	rm -rf obj out
