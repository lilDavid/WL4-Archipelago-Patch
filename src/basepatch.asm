.gba

input equ "Wario Land 4.gba"
output equ "out/baserom.gba"
.open input,output,0x08000000

UnusedRamStart equ 0x03006280
UnusedRomStart equ 0x0878F97C

.include "src/util/vanilla_labels.asm"
.include "src/util/randomizer_variables.asm"
.include "src/util/macros.asm"
.include "src/util/bitfields.asm"


; Allocate space at ROM end
.org UnusedRomStart
.region 0x0E000000-.
    .arm
    PlayerName: .fill 64, 0
    PlayerID: .halfword 0
    DeathLinkFlag: .byte 0

    .align 4
    ItemLocationTable:              .fill 6 * 4 * 7,     ItemID_None
    ItemExtDataTable:               .fill 6 * 4 * 7 * 4, 0
    StartingInventoryItemStatus:    .fill 6 * 6,         0
    StartingInventoryJunkCounts:    .fill 4,             0
    StartingInventoryWarioAbilities:               .byte 0
    .thumb
.endregion

.autoregion :: .importobj "obj/items/item_table.o" :: .endautoregion
.autoregion :: .importobj "obj/items/multiworld.o" :: .endautoregion
.include "src/items/collect_treasure.asm"
.autoregion :: .importobj "obj/items/collect_junk.o" :: .endautoregion
.include "src/items/collection_indicator.asm"
.include "src/shuffle/boxes.asm"
.include "src/shuffle/limit_abilities.asm"
.include "src/game_loop/level_select.asm"
.include "src/shuffle/save_data.asm"
.include "src/game_loop/passage_select.asm"
.include "src/game_loop/game_main.asm"
.include "src/game_loop/level_results.asm"
.include "src/init.asm"
.include "src/patches.asm"
.include "src/lib.asm"
.include "src/data/graphics.asm"
.include "src/data/strings.asm"

.close
