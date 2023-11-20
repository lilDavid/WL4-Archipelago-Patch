.gba

input equ "Wario Land 4.gba"
output equ "build/baserom.gba"
.open input,output,0x08000000

UnusedRamStart equ 0x03006280
UnusedRomStart equ 0x0878F97C

.include "src/symbols/vanilla_labels.asm"
.include "src/symbols/randomizer_variables.asm"

ITEM_NONE equ 0xFF

; Allocate space at ROM end
.org UnusedRomStart
.region 0x0E000000-.
    .arm
    .align 4
    PlayerName: .fill 64, 0
    PlayerID: .halfword 0
    DeathLinkFlag: .byte 0

    ItemLocationTable:              .fill 6 * 4 * 7,     ITEM_NONE
    ItemExtDataTable:               .fill 6 * 4 * 7 * 4, 0
    StartingInventoryItemStatus:    .fill 6 * 6,         0
    StartingInventoryJunkCounts:    .fill 4,             0
    StartingInventoryWarioAbilities:               .byte 0
    .thumb
.endregion

.include "src/patches.asm"
.include "src/hooks.asm"
.include "src/limit_abilities.asm"

.include "src/lib.asm"
.autoregion
.importobj "obj/init.o"
.importobj "obj/items/item_table.o"
.importobj "obj/items/multiworld.o"
.importobj "obj/items/collect_junk.o"
.importobj "obj/items/collection_indicator.o"
.importobj "obj/shuffle/boxes.o"
.importobj "obj/shuffle/save_data.o"
.importobj "obj/game_loop/passage_select.o"
.importobj "obj/game_loop/level_select.o"
.importobj "obj/game_loop/game_main.o"
.importobj "obj/game_loop/level_results.o"
.importobj "obj/graphics.o"
.endautoregion

.include "src/data/graphics.asm"
.include "src/data/strings.asm"

.close
