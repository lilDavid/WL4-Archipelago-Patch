ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPRO)
endif
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
CFLAGS = -mthumb -mthumb-interwork -Wall -Wextra -std=gnu11 \
		 -Iinclude -I$(DEVKITPRO)/libgba/include -Lgba

ASM = src/util/vanilla_labels.asm \
	  src/util/randomizer_variables.asm \
	  src/util/macros.asm \
	  src/util/bitfields.asm \
	  src/items/item_table.asm \
	  src/items/multiworld.asm \
	  src/items/collect_treasure.asm \
	  src/items/collection_indicator.asm \
	  src/boxes/randomize_boxes.asm \
	  src/boxes/save_full_health.asm \
	  src/move_shuffle/limit_abilities.asm \
	  src/move_shuffle/ability_ui.asm \
	  src/routines.asm \
	  src/patches.asm \
	  src/graphics.asm \
	  src/messages/textboxes.asm \
	  src/messages/draw_text.asm \
	  src/messages/strings.asm

OBJ = obj/routines.o \
	  obj/items/item_table.o \
	  obj/items/collect_junk.o

GRAPHICS = data/graphics/ability_get.bin \
		   data/graphics/ability_icons.bin \
		   data/graphics/ap_logo.bin

basepatch: out/basepatch.bsdiff

out/basepatch.bsdiff: out/baserom.gba
	bsdiff "Wario Land 4.gba" out/baserom.gba out/basepatch.bsdiff

baserom: out/baserom.gba

out/baserom.gba: $(ASM) $(OBJ) $(GRAPHICS)
	@mkdir -p out
	armips src/basepatch.asm -sym out/baserom.sym
	grep -Ev '[0-9A-F]{8} [@.].*' out/baserom.sym > out/basepatch.sym

obj/%.o: src/%.c include/*.h
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

data/graphics/%.bin: data/graphics/%.png data/graphics/%.txt
	python3 make_graphics.py $@

clean:
	rm -rf obj out
