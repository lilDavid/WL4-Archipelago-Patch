.gba

input equ "Wario Land 4.gba"
output equ "out/basepatch.gba"
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
    PlayerName: .fill 64, 0
    PlayerID: .halfword 0
    DeathLinkFlag: .byte 0
.endregion

.include "src/items/item_table.asm"
.include "src/items/multiworld.asm"
.include "src/items/collect_treasure.asm"
.include "src/items/collect_junk.asm"
.include "src/items/collection_indicator.asm"
.include "src/boxes/randomize_boxes.asm"
.include "src/boxes/save_full_health.asm"
.include "src/move_shuffle/limit_abilities.asm"
.include "src/move_shuffle/ability_ui.asm"
.include "src/routines.asm"
.include "src/patches.asm"
.include "src/graphics.asm"
.include "src/messages/textboxes.asm"
.include "src/messages/draw_text.asm"
.include "src/messages/strings.asm"

.close
