.gba

; GameSelectSeisan() case 7 - Show item text box
hook 0x8080C5C, 0x8080C6C, @Hook_GameSelectSeisan7

; GameSelectSeisan() case 8 - Let the player flip through items quickly
org 0x8080CD8
    mov r0, #15

; GameSelectSeisan() case 9 - Repeatedly show text box
.org 0x8080AB0
.word @Hook_GameSelectSeisan9


.autoregion


.importobj "obj/game_loop/level_results.o"


.align 2
; Initialize the former save tutorial, or end the level if nothing to show.
@Hook_GameSelectSeisan7:
        push {lr}
        bl LevelResults_ShowNextItem
        pop {r1}
        cmp r0, #0
        bne @@Return
    @@JumpToFadeOut:
        ldr r1, =0x8080D03
    @@Return:
        bx r1

    .pool


@Hook_GameSelectSeisan9:
        bl ResultsScreen_SentItemTextBox
        cmp r0, #7
        beq @@FadeOut
        ldr r0, =0x8080D45
        bx r0
    @@FadeOut:
        ldr r0, =0x8080D03
        bx r0
    .pool


.endautoregion
