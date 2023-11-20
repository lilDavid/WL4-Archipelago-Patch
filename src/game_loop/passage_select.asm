
; GameSelect() case 2
.org 0x80799E0
.word @Hook_GameSelect2

; SelectDmapOamCreate() returning
.org 0x807CA64
.area 0x807CA6C-.
        ldr r0, =@Hook_SelectDmapOamCreate | 1
        bx r0
    .pool
.endarea


.autoregion
.align 2


.importobj "obj/game_loop/passage_select.o"


; Receive multiworld items (level select)
@Hook_GameSelect2:
        bl PassageSelect_Rando
        cmp r0, #1
        beq @@SkipCase2
    ; Run case 2
        ldr r0, =0x8079AE0
        mov pc, r0
    @@SkipCase2:
        ldr r0, =0x807A36A
        mov pc, r0
    .pool


; Create the sprite for the item received.
@Hook_SelectDmapOamCreate:
        bl PassageSelect_CreateReceivedOAM
    ; Return from SelectDmapOamCreate
        pop {r4-r7}
        pop {r0}
        bx r0
    .pool


.endautoregion
