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


.align 2
; Collect the item in r0. If the item is this player's junk, it will be stored
; and given to Wario later.
;
; Parameters:
;   r0: Item ID
;   r1: Pointer to multiworld data
;
; See items.py
GiveItem:
        push {r4-r5, lr}

        cmp r0, #ItemID_None
        beq @@Return

    ; If another world's item, it'll be handled outside the game
        cmp r1, #0
        bne @@Return

        lsr r1, r0, #ItemBit_Junk
        cmp r1, #0
        bne @@Junk

    ; Progression item
        get_bit r1, r0, ItemBit_Ability
        cmp r1, #1
        beq @@WarioAbility

        ldr r4, =W4ItemStatus

        ; Get passage ID * 24 into r1
        get_bits r1, r0, 4, 2  ; r1 = passage ID
        lsl r3, r1, #1  ; passage*2
        add r3, r3, r1  ; passage*3
        lsl r1, r3, #3  ; passage*24

        ; Get bits 1:0 into r2
        get_bits r2, r0, 1, 0

        ; Check bit 5 to determine CD/Jewel piece
        lsr r3, r0, #ItemBit_CD
        cmp r3, #0
        bne @@CD

    ; Jewel piece
        add r1, r1, r4
        mov r5, #4

        ; r2 is the piece we have; bit to set in level status is 1 << r2 -> r3
        mov r3, #1
        lsl r3, r2

    ; Loop through level statuses to find the first one with this piece's bit
    ; unset. If all four are set, break out since this one's an extra.
    @@CheckJewel:
        ldr r2, [r1]
        mov r0, r2
        and r2, r3
        cmp r2, #0
        beq @@FoundJewel

    ; Next
        add r1, r1, #4
        add r5, r5, #-1
        cmp r5, #0
        beq @@Return
        b @@CheckJewel

    @@FoundJewel:
        orr r0, r3
        str r0, [r1]
        b @@Return

    @@CD:
        ; Load and update level's entry in level status table
        lsl r2, r2, #2
        add r1, r1, r2
        add r1, r1, r4
        ldr r2, [r1]
        mov r3, #0x10
        orr r2, r3
        str r2, [r1]
        b @@Return

    @@WarioAbility:
        ldr r4, =WarioAbilities
        ldrb r5, [r4]
        get_bits r1, r0, 2, 0
        mov r0, #1
        lsl r0, r1
        cmp r1, MoveBit_GroundPound
        beq @@ProgressiveAbility
        cmp r1, MoveBit_Grab
        beq @@ProgressiveAbility
        b @@SetBit

    @@ProgressiveAbility:
        mov r2, r0
        and r2, r5
        cmp r2, #0
        beq @@SetBit

    ; Switch to stronger version
        ; r1 = 0 or 3
        mov r0, #1
        lsr r1, #1  ; 0 or 1
        add r1, #6  ; 6 or 7
        lsl r0, r1

    @@SetBit:
        orr r5, r0
        strb r5, [r4]
        b @@Return

    @@Junk:
    ; Queue your junk item by incrementing the appropriate variable
        get_bits r1, r0, 3, 0
        ldr r2, =QueuedJunk
        add r1, r1, r2
        ldrb r2, [r1]
        add r2, r2, #1
        strb r2, [r1]

    @@Return:
        pop r4-r5, pc

    .pool


.endautoregion
