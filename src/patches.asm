.gba

; Single-line patches.


; Start with access to the four main passages.
; SelectDMapInit()
.org 0x807B110
    b 0x0807b204


; PauseInit(): Show the player which boxes they've opened, not what they have.
.org 0x8088BEA
    ldrb r1, [r1, #1]
.org 0x8088CDE
    ldrb r0, [r1, #1]
.org 0x8088D12
    ldrb r0, [r1, #1]
.org 0x8088D46
    ldrb r0, [r1, #1]
.org 0x8088D7A
    ldrb r0, [r1, #1]


; Skip cutscenes
; --------------

; Intro cutscene
.org 0x8000312  ; MainGameLoop(): Prevent cutscene starting
    mov r0, #1
.org 0x8091944  ; GameReady(): Stop title music
    nop
.org 0x8091DA8  ; ReadySet_SelectKey(): Don't play car engine sound
    .word 0x8091DD8

; Jewel cutscene and jewel door opening
.org 0x8080FA8  ; DoraGetItemHantei()
    nop

; Boss defeat cutscene
.org 0x8056AA8       ;  EntityAI_0xCF_boss_jewel()
.area 0x8056AD8 - .  ; Treasure coming out of wall and rising offscreen
    ; This ends the cutscene with the fade out before it gets anywhere,
    ; incidentally freezing the smoke from the boss dying in the process.
    ; The animation could be made more elegant, but this will do.
        ldr r1, =sGameSeq
        mov r0, #6
        strh r0, [r1]
        ldr r0, =0x3000021  ; ucGmapSubSeq
        mov r2, #0
        strb r2, [r0]
        ldr r1, =0x3000048  ; ucSTEndType
        mov r0, #5
        strb r0, [r1]

        mov r3, #1
        b @@End
    .pool
    @@End:
.endarea

.org 0x80004C2  ; MainGameLoop(): Treasure going to pyramid
    mov r0, #0x1D

; Post-boss cutscenes
.org 0x8079FDC  ; MainGameLoop(): Pyramid appears
    mov r0, #1
.org 0x807A034  ; MainGameLoop(): Pyramid opens
    nop
