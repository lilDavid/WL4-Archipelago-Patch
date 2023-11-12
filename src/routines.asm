.gba


; Override the end of EXimage_Clear_Work_2Mode() to instead jump to our function
.org 0x8074068
.area 0x8074070-.
        ldr r0, =@Hook_EXimage_Clear_Work_2Mode | 1
        bx r0
    .pool
.endarea

; HardwareInitialization()
hook 0x8000728, 0x8000738, InitializeVariables

; GameSelect() case 2
.org 0x80799E0
.word PyramidScreen

; GameMain() case 2
.org 0x801B8EC
.word LevelScreen

; GameMain() case 2 (near the end)
hook 0x801BB7A, 0x801BB90, LoadTextSprites


.autoregion
.align 2


.macro @transfer_itemcount, offset
    ldrb r2, [r0, offset]
    strb r2, [r1, offset]
.endmacro


@Hook_EXimage_Clear_Work_2Mode:
    bl CreateStartingInventory
    pop {pc}  ; Returning from hooked function, LR already pushed


; Receive multiworld items (level select)
PyramidScreen:
        push {r4}

        ldr r0, =MultiworldState
        ldrb r0, [r0]
        cmp r0, #2
        beq @@ShowTextBox

        bl ReceiveNextItem  ; a1
        cmp r0, #ItemID_None
        beq @@RunCase2

        mov r4, r0
        mov r1, #0  ; a2
        bl GiveItem
        mov r0, r4
        bl ItemReceivedFeedbackSound

        bl LoadMessageBG
        bl PyramidScreenShowReceivedItem

        ldr r0, =TextTimer
        mov r1, #15
        strb r1, [r0]

    @@RunCase2:
        ldr r0, =0x8079AE0
        b @@Return

    @@ShowTextBox:
        ldr r0, =TextTimer
        ldrb r1, [r0]
        cmp r1, #0
        beq @@WaitForButton
        sub r1, #1
        strb r1, [r0]
        b @@SkipCase2

    @@WaitForButton:
        ldr r0, =usTrg_KeyPress1Frame
        ldrh r1, [r0]
        mov r0, #1
        and r0, r1
        cmp r0, #0
        beq @@SkipCase2

        ldr r0, =0x125
        call_using r1, m4aSongNumStart
        bl LoadPyramidBG3

        ldr r0, =MultiworldState
        mov r1, #0
        strb r1, [r0]

    @@SkipCase2:
        ldr r0, =0x807A36A
    @@Return:
        pop {r4}
        mov pc, r0
    .pool



; Receive multiworld items and collect junk (in level)
LevelScreen:
        push {r4}

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


.importobj "obj/routines.o"


.endautoregion
