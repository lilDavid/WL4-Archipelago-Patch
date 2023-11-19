.gba


; TKakeraIconDsp_main()
.org 0x8078E68
.word @Hook_TKakeraIconDsp_main_init  ; case 0
.word @Hook_TKakeraIconDsp_main_update  ; case 1
.word @Hook_TKakeraIconDsp_main_update  ; case 2
.word @Hook_TKakeraIconDsp_main_update  ; case 3
.word @Hook_TKakeraIconDsp_main_update  ; case 4


; TCardIconDsp_main()
hook_branch 0x80790B6, 0x80790CC, 0x80790D8, @Hook_TCardIconDSp_main_init
hook 0x8079112, 0x8079126, CDIcon_Update


.autoregion


.importobj "obj/items/collection_indicator.o"


; Replacement for the initialization function of the jewel piece collection
; indicator. This is set up to handle both jewel pieces and the progressive
; abilities.
@Hook_TKakeraIconDsp_main_init:
        bl GemIcons_Init
        ldr r0, =0x8079064
        mov pc, r0
    .pool


; Replacement for the update function of the jewel piece icon. Like
; ReadJewelPieces above, this uses the LastCollectedItemStatus rather than any
; of the level status variables.
@Hook_TKakeraIconDsp_main_update:
        bl GemIcons_Update
        ldr r0, =0x8079064
        mov pc, r0

    .pool


; Replacement for the initialization for the CD collection indicator. The main
; difference from vanilla is that this uses the LastCollectedItemID and
; LastCollectedItemStatus.
@Hook_TCardIconDSp_main_init:
        push {lr}
        bl CDIcon_Init

    ; Load collected CD
        ldr r0, =LastCollectedItemID
        ldrb r1, [r0]

        ldr r0, =LastCollectedItemStatus
        ldrb r3, [r0]
        cmp r3, #0
        beq @@Timeout

        mov r2, #1
        mov r0, #2
        b @@Return

    @@Timeout:
        mov r2, #2
        ldr r0, =0x80790CC
        mov lr, r0

    @@Return:
        ldr r3, =Scbuf_ucStatus
        ldrb r1, [r3]
        pop {pc}

    .pool


.endautoregion
