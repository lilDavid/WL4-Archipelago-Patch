.gba

; Replaces some code with a call to a mod function.
; Unlike the other two hook macros, this doesn't set the link register, so this
; is cheaper to use if the hook ends with a branch.
.macro hook_manual, Start, End, HackFunction
    .org Start
    .area End-.
        ldr r0, =HackFunction
        mov pc, r0
    .pool
    .endarea
.endmacro

; Replaces a branch with a call to a mod function.
; This is for when the replaced code ends with an unconditional jump so that the
; pool afterward in the original code can also be replaced.
.macro hook_branch, Start, End, ReturnAddress, HackFunction
    .org Start
    .area End-.
        ldr r0, =ReturnAddress | 1
        mov lr, r0
        ldr r0, =HackFunction
        mov pc, r0
    .pool
    .endarea
.endmacro

; Replace some code with a call to a mod function.
; This will return execution to the end of the code that was replaced.
.macro hook, Start, End, HackFunction
        hook_branch Start, End, End, HackFunction
.endmacro

; Load a function pointer into a register and call it. Since mod code is at the
; end of the ROM while vanilla code is at the beginning, this is needed for the
; large jumps between modded/vanilla subroutines
.macro call_using, register, Function
        ldr register, =@@Return | 1
        mov lr, register
        ldr register, =Function
        mov pc, register
    @@Return:
.endmacro

; Copy bits m-n inclusive from the source register into a destination register.
; They'll be shifted into the least significant position, and the most
; significant bits will all be cleared.
.macro get_bits, dst, src, msb, lsb
        lsl dst, src, #31-msb
        lsr dst, dst, #31 - (msb - lsb)
.endmacro

; Copy bit n from the source register into the least significant bit in the
; destination register. The most significant 31 bits will be cleared.
.macro get_bit, dst, src, bit
        get_bits dst, src, bit, bit
.endmacro

; Tile sizes and coordinates
sizeof_tile equ 0x20
.expfunc tile_no_4b(n), n * sizeof_tile
.expfunc tile_coord_4b(x, y), tile_no_4b(x + 32 * y)

; Items
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
