.gba

.autoregion


; 24 available level IDs, not all of which are used.
@levels equ 6 * 4


ItemBit_CD equ 5
ItemBit_Ability equ 6
ItemBit_Junk equ 7

.expfunc ItemID_Jewel(passage, quadrant), (passage << 2) | quadrant
.expfunc ItemID_CD(passage, level), (1 << ItemBit_CD) | (passage << 2) | level

.expfunc ItemID_Ability(ability), (1 << ItemBit_Ability) | ability
ItemID_GroundPound equ ItemID_Ability(MoveBit_GroundPound)
ItemID_Swim equ ItemID_Ability(MoveBit_Swim)
ItemID_HeadSmash equ ItemID_Ability(MoveBit_HeadSmash)
ItemID_Grab equ ItemID_Ability(MoveBit_Grab)
ItemID_DashAttack equ ItemID_Ability(MoveBit_DashAttack)
ItemID_EnemyJump equ ItemID_Ability(MoveBit_EnemyJump)

.expfunc ItemID_Junk(junk), (1 << ItemBit_Junk) | junk
ItemID_FullHealthItem equ ItemID_Junk(0)
ItemID_TransformTrap equ ItemID_Junk(1)
ItemID_Heart equ ItemID_Junk(2)
ItemID_Lightning equ ItemID_Junk(3)

ItemID_Archipelago equ 0xF0
ItemID_None equ 0xFF

; Wario's abilities
MoveBit_GroundPound equ 0
MoveBit_Swim equ 1
MoveBit_HeadSmash equ 2
MoveBit_Grab equ 3
MoveBit_DashAttack equ 4
MoveBit_EnemyJump equ 5
MoveBit_GroundPoundSuper equ 6
MoveBit_GrabHeavy equ 7


.align 4
.arm
ItemLocationTable:              .fill 6 * 4 * 7,     ItemID_None
ItemExtDataTable:               .fill 6 * 4 * 7 * 4, 0
StartingInventoryItemStatus:    .fill 6 * 6,         0
StartingInventoryJunkCounts:    .fill 4,             0
StartingInventoryWarioAbilities:               .byte 0
.thumb

.importobj "obj/items/item_table.o"


.endautoregion
