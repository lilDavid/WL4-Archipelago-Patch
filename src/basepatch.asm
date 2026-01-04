.gba

input equ "Wario Land 4.gba"
output equ "build/baserom.gba"
.open input,output,0x08000000

UnusedRamStart equ 0x03006280
UnusedRomStart equ 0x0878F97C

.include "src/symbols/vanilla_labels.asm"
.include "src/symbols/randomizer_variables.asm"

JUNK_MAX equ 6
ITEM_NONE equ 0xFF

PASSAGES equ 6
LEVELS_PER_PASSAGE equ 5
CHECKS_PER_LEVEL equ (4 + 1 + 1 + 2 + 11)  ; 4 gem pieces, up to 1 CD, keyzer, up to 2 health refills, up to 11 diamonds
TOTAL_LOCATIONS equ (PASSAGES * LEVELS_PER_PASSAGE * CHECKS_PER_LEVEL)

; Allocate space at ROM end
.org UnusedRomStart
.region 0x0E000000-.
    .arm
    ; Seed info
    .align 4
    PlayerName: .fill 64, 0
    PlayerID:   .halfword 0
    SeedName:   .fill 64, 0

    ; Location setup
    .align 4
    ItemLocationTable:              .fill TOTAL_LOCATIONS,     ITEM_NONE
    .align 4
    MultiworldDataTable:            .fill TOTAL_LOCATIONS * 4, 0
    StartingInventoryItemStatus:    .fill PASSAGES * 6,        0
    StartingInventoryJunkCounts:    .fill JUNK_MAX,            0
    StartingInventoryWarioAbilities:                     .byte 0

    ; Settings
    SendMultiworldItemsImmediately: .byte 0
    GoalType:                       .byte 0
    GoldenTreasuresNeeded:          .byte 0
    TrapBehavior:                   .byte 0
    DiamondShuffle:                 .byte 0
    .thumb
.endregion

.sym off

.include "src/patches.asm"
.include "src/hooks.asm"
.include "src/limit_abilities.asm"

.autoregion
.importobj "obj/init.o"
.importobj "obj/items/item_table.o"
.importobj "obj/items/multiworld.o"
.importobj "obj/items/collect_junk.o"
.importobj "obj/items/collection_indicator.o"
.importobj "obj/shuffle/rando_sprite_util.o"
.importobj "obj/shuffle/item.o"
.importobj "obj/shuffle/boxes.o"
.importobj "obj/shuffle/diamonds.o"
.importobj "obj/shuffle/keyzer.o"
.importobj "obj/shuffle/save_data.o"
.importobj "obj/shuffle/golden_treasure_hunt.o"
.importobj "obj/game_loop/passage_select.o"
.importobj "obj/game_loop/level_select.o"
.importobj "obj/game_loop/game_main.o"
.importobj "obj/game_loop/level_results.o"
.importobj "obj/cutscene_skips.o"
.importobj "obj/misc.o"
.importobj "obj/gfx/items.o"
.importobj "obj/gfx/cosmetic.o"
.endautoregion

.include "src/data/graphics.asm"
.include "src/data/credits.asm"

.sym on

.include "src/data/strings.asm"

.close
