.gba

; Inline patches.

; Place CD icons on level select behind background 0
.org 0x80859B8  ; SelectMmapOamCreate
    mov r1, #(-0xF7) & 0xFF
.org 0x864115A
    .halfword 0x3016 | (1 << 10)


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

.org 0x808972A  ; PauseOamCreate(): Show CD with same rule above
    ldrb r0, [r1, #1]


; EntityAI_0x00_0x01_0x02_0x03_0x04_0x05_Q_K2_COM_takarabako(): Skip voice playing
.org 0x80292A0
    nop
    nop
    nop


; Skip cutscenes
; --------------

; Intro cutscene
.org 0x8000312  ; MainGameLoop(): Prevent cutscene starting
    mov r0, #1
.org 0x8091944  ; GameReady(): Stop title music
    nop
.org 0x8091A46  ; ReadyMain_SelectDef(): Skip difficulty selection
    b 0x8091A58

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

; Speed up Golden Diva intro
.org 0x804F2DE  ; Q_BOUND1_L_item_cat(): Move faster and skip attack
    add r0, r2, #7
.org 0x804F30E
    mov r0, #0x3B
.org 0x804F582  ; BOUND3_L_item_cat(): Faster absorbing animation
    sub r0, #8
.org 0x80634B0  ; Pool in Q_INITIAL_mouja(): Start at final position
   .word -0x18C
.org 0x80679B0  ; K0_WALK_mouja_ring(): Make them disappear with these changes
    blt 0x8067A50
.org 0x806C478  ; BossStage_Diva_Event(): Start wave effect sooner
    cmp r0, #5
.org 0x806C5E4  ; BossStage_Diva_Raster(): Make wave effect faster
    add r0, #4
