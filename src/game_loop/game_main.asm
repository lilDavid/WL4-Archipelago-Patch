
; GameMain() case 2
.org 0x801B8EC
.word LevelScreen

; GameMain() case 2 (near the end)
hook 0x801BB7A, 0x801BB90, LoadTextSprites


.autoregion
.align 2


.importobj "obj/game_loop/game_main.o"


; Receive multiworld items and collect junk (in level)
LevelScreen:
        push {r4}

    .ifdef DEBUG
        bl Debug_PressSelectForFreeMovement
    .endif

    ; If Wario isn't in a playable state, don't bother yet
        ldr r0, =usWarStopFlg
        ldrh r0, [r0]
        cmp r0, #0
        bne @@Return

        bl ReceiveNextItem  ; a1

        cmp r0, #ItemID_None
        beq @@CollectJunk
        ; Set text timer
        ldr r1, =TextTimer
        mov r2, #120
        strb r2, [r1]

        mov r4, r0
        mov r1, #0  ; a2
        bl GiveItem
        bl LoadReceivedText

    ; If we get treasure, tell the player
        get_bit r0, r4, ItemBit_Junk
        cmp r0, #0
        bne @@CollectJunk

        mov r0, r4
        bl ItemReceivedFeedbackSound
        get_bit r0, r4, ItemBit_Ability
        cmp r0, #1
        beq @@Ability

        get_bits r0, r4, 4, 2
        bl SetTreasurePalette
        lsr r0, r4, #ItemBit_CD  ; a1
        mov r1, #1  ; a2
        bl SpawnCollectionIndicator
        b @@CollectJunk

    @@Ability:
        cmp r4, #ItemID_GroundPound
        beq @@GroundPound
        cmp r4, #ItemID_Grab
        beq @@Grab
        b @@OtherAbility

    @@GroundPound:
        bl MixTemporaryAbilities
        get_bit r0, r0, MoveBit_GroundPoundSuper
        cmp r0, #0
        beq @@FirstProgressive
        mov r0, #MoveBit_GroundPoundSuper
        b @@FinishProgressive

    @@Grab:
        bl MixTemporaryAbilities
        get_bit r0, r0, MoveBit_GrabHeavy
        cmp r0, #0
        beq @@FirstProgressive
        mov r0, #MoveBit_GrabHeavy
        b @@FinishProgressive

    @@FirstProgressive:
        get_bits r0, r4, 2, 0

    @@FinishProgressive:
        mov r4, #0
        b @@FinishAbility

    @@OtherAbility:
        get_bits r0, r4, 2, 0
        mov r4, #1

    @@FinishAbility:
        ldr r1, =AbilityPaletteTable
        add r1, r0
        ldrb r1, [r1]

        mov r0, r1
        bl SetTreasurePalette
        mov r0, r4
        mov r1, #1
        bl SpawnCollectionIndicator

    @@CollectJunk:
        bl CollectJunkItems

    @@Return:
        pop {r4}
        ldr r0, =0x801B950
        mov pc, r0
    .pool


LoadTextSprites:
        push {lr}

    ; Displaced instructions, up to enemies
        ldrb r0, [r4]
        cmp r0, #0
        bne @@EnemyDisplay
        call_using r0, GmWarioCreate
    @@EnemyDisplay:
        call_using r0, EnemyDisplayMain

        ldr r0, =MultiworldState
        ldrb r0, [r0]
        lsl r0, r0, #2
        ldr r1, =@@MWStateTable
        add r1, r0, r1
        ldr r1, [r1]
        mov pc, r1

    .align 4
    @@MWStateTable:
        .word @@Return
        .word @@Return
        .word @@DrawReceivedText
        .word @@DrawSentText

    @@DrawReceivedText:
        mov r0, #1
        b @@LoadText
    @@DrawSentText:
        mov r0, #0
    @@LoadText:
        bl CreateTextOAM

    ; Count the timer
        ldr r0, =TextTimer
        ldrb r1, [r0]
        cmp r1, #0
        beq @@Clear
        sub r1, #1
        strb r1, [r0]
        b @@Return
    @@Clear:
        ldr r0, =MultiworldState
        strb r1, [r0]

    @@Return:
    ; Displaced instructions, continued
        call_using r0, ClearOamBuf
        call_using r0, GmapSceneCreate

        pop {pc}
    .pool


; Create OAM data for text
; Parameters:
;   r0: No gaps between objects if 0; otherwise, add spaces around the third object
CreateTextOAM:
        push {r4-r6, lr}
        mov r6, r0
        ldr r0, =attr0_wide | attr0_4bpp | attr0_y(146)
        ldr r1, =attr1_size(1) | attr1_x(8)
        ldr r2, =attr2_palette(3) | attr2_priority(0) | attr2_id(0x10C)
        ldr r3, =ucCntObj
        ldr r4, =OamBuf

        ldrb r5, [r3]
        lsl r5, r5, #3
        add r4, r4, r5

    ; 1st
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
    ; 2nd
        add r1, #32
        add r2, #4
        add r4, #8
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
    ; 3rd
        cmp r6, #0
        beq @@NoSpace3
        add r1, #8
    @@NoSpace3:
        add r1, #32
        add r2, #4
        add r4, #8
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
        ; 4th
        cmp r6, #0
        beq @@NoSpace4
        add r1, #8
    @@NoSpace4:
        add r1, #32
        add r2, #0x130-0x114
        add r4, #8
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
    ; 5th
        add r1, #32
        add r2, #4
        add r4, #8
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
    ; 6th
        add r1, #32
        add r2, #0x20-4
        add r4, #8
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
    ; 7th
        add r1, #32
        add r2, #4
        add r4, #8
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]

        ldrb r5, [r3]
        add r5, #7
        strb r5, [r3]

        pop {r4-r6, pc}
    .pool


.definelabel @ObjectPalette3, 0x5000260

; Copy text sprites into the sprite table. On encountering 0xFE, blank spaces
; will be copied into the remaining space.
; Parameters:
;   r0: Pointer to 0xFE-terminated string
;   r1: Pointer to first letter destination
;   r2: Number of characters to copy.
; Returns:
;   r0: Pointer to byte after the last one loaded. If the end of the string was
;       hit, this will point to 0xFE.
LoadSpriteString:
        push {lr}
        push {r4-r6}
        mov r4, r0
        mov r5, r1
        mov r6, r2

    ; Override OBP3 color 2 with white.
    ; TODO: find where the overridden purple color is used and change methods if necessary
        ldr r1, =@ObjectPalette3 + 4
        ldr r0, =0x7FFF
        strh r0, [r1]

    @@LoadFromString:
        ldrb r0, [r4]
        cmp r0, #0xFE
        beq @@LoadCharacter
        add r4, r4, #1

    @@LoadCharacter:
        mov r1, r5
        bl LoadSpriteCharacter
        add r5, #sizeof_tile
        sub r6, r6, #1

    @@CheckNChars:
        cmp r6, #0
        bne @@LoadFromString
    @@Return:
        mov r0, r4
        pop {r4-r6}
        pop {pc}
    .pool

; Load a character into the sprite table.
; Parameters:
;   r0: Pointer to character
;   r1: Pointer to destination
LoadSpriteCharacter:
        lsl r0, r0, #2
        ldr r2, =LetterToSpriteTile
        add r0, r2, r0
        ldr r0, [r0]

        ldr r2, =REG_DMA3SAD
        str r0, [r2]
        mov r0, r1
        str r0, [r2, #4]
        ldr r0, =dma_enable | dma_words(8)
        str r0, [r2, #8]
        ldr r0, [r2, #8]

        mov pc, lr
    .pool


.endautoregion
