ASM = src/items/item_table.asm \
      src/items/multiworld.asm \
      src/items/collect_treasure.asm \
      src/items/collect_junk.asm \
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

GRAPHICS = data/graphics/ability_get.bin \
	       data/graphics/ability_icons.bin \
	       data/graphics/ap_logo.bin

build/basepatch.bsdiff: build/baserom.gba
	bsdiff "Wario Land 4.gba" build/baserom.gba build/basepatch.bsdiff

build/baserom.gba: $(ASM) $(GRAPHICS)
	@mkdir -p build
	armips src/basepatch.asm -sym build/baserom.sym
	grep -Ev '[0-9A-F]{8} [@.].*' build/baserom.sym > build/basepatch.sym

data/graphics/%.bin: data/graphics/%.png data/graphics/%.txt
	python3 make_graphics.py $@
