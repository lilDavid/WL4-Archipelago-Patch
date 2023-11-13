
; GameSelect() case 2
.org 0x80799E0
.word PyramidScreen

; SelectDmapOamCreate() returning
.org 0x807CA64
.area 0x807CA6C-.
        ldr r0, =PyramidScreenCreateReceivedItemOAM | 1
        bx r0
    .pool
.endarea


.autoregion
.align 2


.importobj "obj/game_loop/level_select.o"


; Receive multiworld items (level select)
PyramidScreen:
        push {r4}

    .ifdef DEBUG
        bl Debug_SetFlagsWithL
    .endif

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


; Create the sprite for the item received.
PyramidScreenCreateReceivedItemOAM:
        ldr r0, =MultiworldState
        ldrb r0, [r0]
        cmp r0, #2
        bne @@Return

        ldr r3, =ucCntObj
        ldrb r5, [r3]
        ldr r4, =OamBuf
        lsl r0, r5, #3
        add r4, r4, r0

        ldr r0, =attr0_square | attr0_4bpp | attr0_y(104)
        ldr r2, =attr2_palette(0xF) | attr2_priority(0) | attr2_id(0x200)

        ldr r6, =IncomingItemID
        ldrb r6, [r6]
        get_bit r1, r6, ItemBit_Junk
        cmp r1, #0
        bne @@JunkItem

    ; Jewel Pieces or CD
        add r5, #1

        get_bit r1, r6, ItemBit_Ability
        cmp r1, #0
        bne @@JewelPieceOrAbility

        get_bit r1, r6, ItemBit_CD
        cmp r1, #0
        bne @@CD

    @@JewelPieceOrAbility:
        ldr r1, =attr1_size(1) | attr1_x(120 - 8)
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
        b @@Return

    @@CD:
        sub r0, #8
        ldr r1, =attr1_size(2) | attr1_x(120 - 16)
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]
        b @@Return

    @@JunkItem:
        get_bits r1, r6, 3, 0
        lsl r1, #2
        ldr r7, =@@JunkJumpTable
        add r1, r7
        ldr r1, [r1]
        mov pc, r1

    .align 4
    @@JunkJumpTable:
        .word @@FullHealthItem
        .word @@BigBoardTrap  ; Wario transform
        .word @@Heart
        .word @@BigBoardTrap  ; Lightning damage

    @@FullHealthItem:
        ldr r1, =attr1_size(1) | attr1_x(120 - 8)
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]

        ldr r0, =attr0_wide | attr0_4bpp | attr0_y(104 - 8)
        mov r1, #attr1_size(0) | attr1_x(120 - 8)
        add r2, #2
        strh r0, [r4, #8]
        strh r1, [r4, #10]
        strh r2, [r4, #12]

        add r5, #2
        b @@Return

    @@Heart:
        ldr r1, =attr1_size(1) | attr1_x(120 - 8)
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]

        add r5, #1
        b @@Return

    @@BigBoardTrap:
        sub r0, #4
        ldr r1, =attr1_size(1) | attr1_x(120 - 12)
        strh r0, [r4]
        strh r1, [r4, #2]
        strh r2, [r4, #4]

        ldr r0, =attr0_tall | attr0_4bpp | attr0_y(104 - 4)
        ldr r7, =attr1_size(0) | attr1_x(120 + 4)
        add r2, #2
        strh r0, [r4, #8]
        strh r7, [r4, #10]
        strh r2, [r4, #12]

        ; Wario is padded on the left. Lightning on the right.
        cmp r6, #ItemID_Lightning
        beq @@BigBoardSpriteBottom
        sub r1, #8

    @@BigBoardSpriteBottom:
        ldr r0, =attr0_wide | attr0_4bpp | attr0_y(104 - 4 + 16)
        add r2, #1
        strh r0, [r4, #16]
        strh r1, [r4, #18]
        strh r2, [r4, #20]

        add r5, #3

    @@Return:
        strb r5, [r3]  ; Write object count back

    ; Return from SelectDmapOamCreate
        pop {r4-r7}
        pop {r0}
        bx r0
    .pool


; Load the background for the vanilla autosave tutorial.
LoadMessageBG:
        ldr r0, =REG_BG3CNT
        ldr r1, =bg_reg_32x32 | bg_sbb(0x1E) | bg_4bpp | bg_cbb(2) | bg_priority(0)
        strh r1, [r0]

        ; Miraculously, BGP 6 color 2 isn't used at all as far as I can tell
        ldr r0, =0x50000C4
        ldr r1, =0xFFFF
        strh r1, [r0]

        ldr r0, =REG_DMA3SAD
        ldr r1, =SaveTutorialTilemap
        str r1, [r0]
        ldr r1, =0x600F000
        str r1, [r0, #4]
        ldr r1, =dma_enable | dma_halfwords(0x800)
        str r1, [r0, #8]
        ldr r1, [r0, #8]

        ldr r1, =PortalTileset2
        str r1, [r0]
        ldr r1, =0x6008000
        str r1, [r0, #4]
        ldr r1, =dma_enable | dma_words(0x1000)
        str r1, [r0, #8]
        ldr r1, [r0, #8]

        mov pc, lr
    .pool


; Load the original pyramid background 3 graphics.
LoadPyramidBG3:
        ldr r0, =REG_BG3CNT
        ldr r1, =bg_reg_32x32 | bg_sbb(0x1E) | bg_4bpp | bg_cbb(0) | bg_priority(0)
        strh r1, [r0]

        ldr r0, =REG_DMA3SAD
        ldr r1, =PortalTilemap3
        str r1, [r0]
        ldr r1, =0x600F000
        str r1, [r0, #4]
        ldr r1, =dma_enable | dma_halfwords(0x800)
        str r1, [r0, #8]
        ldr r1, [r0, #8]

        mov pc, lr
    .pool


PassagePaletteTable:
    .halfword 0x7B3E, 0x723C, 0x6576, 0x58B0, 0x4C07  ; Entry passage
    .halfword 0x5793, 0x578D, 0x4B20, 0x2E40, 0x1160  ; Emerald passage
    .halfword 0x6B5F, 0x529F, 0x253F, 0x14B4, 0x14AE  ; Ruby passage
    .halfword 0x6BDF, 0x23DF, 0x139B, 0x1274, 0x0DAE  ; Topaz passage
    .halfword 0x7F5A, 0x7E94, 0x7D29, 0x50A5, 0x38A5  ; Sapphire passage
    .halfword 0x579F, 0x3B1F, 0x1A7F, 0x05DE, 0x00FB  ; Golden pyramid
    .halfword 0x3D9C, 0x327D, 0x2B28, 0x6A3B, 0x6DED  ; Archipelago item
    .halfword 0x0000, 0x6BDF, 0x3FBF, 0x22FA, 0x0DAF  ; Garlic (Dash Attack)
    .halfword 0x0000, 0x0000, 0x0000, 0x05DE, 0x24C5  ; Helmet (Head Smash)


; Set the end of object palette 4 to the colors matching the passage in r0
SetTreasurePalette:
        ldr r1, =PassagePaletteTable
        lsl r2, r0, #2
        add r0, r2, r0
        lsl r0, r0, #1
        add r0, r1, r0

    ; DMA transfer - 5 halfwords from palette table entry
        ldr r1, =REG_DMA3SAD
        str r0, [r1]
        ldr r0, =ObjectPalette4 + 0x296 - 0x280
        str r0, [r1, #4]
        ldr r0, =dma_enable | dma_halfwords(5)
        str r0, [r1, #8]
        ldr r0, [r1, #8]

        mov pc, lr
    .pool


; Load the text for the next item received message.
PyramidScreenShowReceivedItem:
        push {r6, lr}
        ldr r6, =0x9000  ; Next tile

        ; "Received "
        mov r2, #9  ; a3 String length
        lsl r3, r2, #6
        mov r1, r6  ; a2
        add r6, r3  ; Set next tile
        ldr r0, =StrItemReceived  ; a1
        call_using r3, MojiCreate

        ; "from "
        mov r2, #5  ; a3 String length
        lsl r3, r2, #6
        mov r1, r6  ; a2
        add r6, r3  ; Set next tile
        ldr r0, =StrItemFrom  ; a1
        call_using r3, MojiCreate

        ; Player name
        ldr r0, =IncomingItemSender
        bl W4strlen
        mov r2, r0  ; a3 String length
        lsl r3, r2, #6
        mov r1, r6  ; a2 Current tile
        add r6, r3  ; Set next tile
        ldr r0, =IncomingItemSender  ; a1
        call_using r3, MojiCreate

        ; Space filler
        ldr r2, =0xA180
        sub r2, r6
        lsr r2, r2, #6  ; a3 String length
        mov r1, r6  ; a2 Current tile
        ldr r0, =StrScreenFiller  ; a1
        call_using r3, MojiCreate

    ; Sprite

        ; Decode
        ldr r3, =REG_DMA3SAD
        ldr r0, =IncomingItemID
        ldrb r0, [r0]

        get_bit r1, r0, ItemBit_Junk
        cmp r1, #1
        beq @@JunkItem

        get_bit r1, r0, ItemBit_Ability
        cmp r1, #1
        beq @@Ability

        ; Major item
        get_bits r2, r0, 4, 2  ; Passage
        lsl r2, r2, #5  ; r2: Passage * 32
        get_bit r1, r0, ItemBit_CD
        cmp r1, #1
        beq @@CD

    ; Jewel piece

        ; DMA in the palette
        ldr r1, =PortalPaletteDTable  ; Palette set
        add r1, r1, r2  ; Passage palette
        str r1, [r3]
        ldr r1, =ObjectPaletteF
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_halfwords(16)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        ; DMA in the tiles
        get_bits r1, r0, 1, 0
        lsl r1, #1  ; Quadrant * 2
        ldr r0, =@@JewelPieceNEOffset  ; Table
        add r0, r0, r1  ; Entry
        ldrh r1, [r0]  ; Tile offset
        ldr r2, =PortalOBJTileset
        add r1, r2, r1  ; Tile address
        str r1, [r3]
        ldr r1, =0x6014000
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_words(8 * 34)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        b @@Return

    @@CD:
        ldr r1, =PortalPaletteETable
        sub r1, #0x20  ; Palette set
        add r1, r1, r2  ; Passage palette
        str r1, [r3]
        ldr r1, =ObjectPaletteF
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_halfwords(16)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        ldr r1, =PortalOBJTileset + tile_no_4b(0x4A)  ; Tile address
        str r1, [r3]
        ldr r1, =0x6014000
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_words(8 * 100)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        b @@Return

    @@Ability:
        cmp r0, #ItemID_GroundPound
        beq @@GroundPound
        cmp r0, #ItemID_Grab
        beq @@Grab
        b @@OtherAbility

    @@GroundPound:
        ldr r1, =WarioAbilities
        ldrb r1, [r1]
        get_bit r1, r1, MoveBit_GroundPoundSuper
        cmp r1, #0
        beq @@OtherAbility
        mov r0, MoveBit_GroundPoundSuper
        b @@OtherAbility

    @@Grab:
        ldr r1, =WarioAbilities
        ldrb r1, [r1]
        get_bit r1, r1, MoveBit_GrabHeavy
        cmp r1, #0
        beq @@OtherAbility
        mov r0, MoveBit_GrabHeavy
        b @@OtherAbility

    @@OtherAbility:
        get_bits r0, r0, 2, 0

        ; Palette
        ldr r1, =@@AbilityPaletteTable
        lsl r2, r0, #2
        add r1, r2
        ldr r1, [r1]
        str r1, [r3]  ; REG_DMA3SAD
        ldr r1, =ObjectPaletteF
        str r1, [r3, #4]  ; REG_DMA3DAD
        ldr r1, =dma_enable | dma_halfwords(16)
        str r1, [r3, #8]  ; REG_DMA3CNT

        ; Graphic
        ldr r1, =AbilityIconTilesTop
        lsl r2, r0, #6
        add r1, r2
        str r1, [r3]  ; REG_DMA3SAD
        ldr r2, =0x6014000
        str r2, [r3, #4]  ; REG_DMA3DAD
        ldr r2, =dma_enable | dma_halfwords(2 * sizeof_tile / 2)
        str r2, [r3, #8]  ; REG_DMA3CNT
        ldr r2, =sizeof_tile * 16
        add r1, r2
        str r1, [r3]  ; REG_DMA3SAD
        ldr r2, =0x6014000 + tile_coord_4b(0, 1)
        str r2, [r3, #4]  ; REG_DMA3DAD
        ldr r2, =dma_enable | dma_halfwords(2 * sizeof_tile / 2)
        str r2, [r3, #8]  ; REG_DMA3CNT

        b @@Return

    @@JunkItem:
        get_bits r1, r0, 3, 0
        lsl r1, #2
        ldr r2, =@@JunkJumpTable
        add r1, r2
        ldr r6, [r1]
        mov pc, r6

    .align 4
    @@JunkJumpTable:
        .word @@FullHealthItem
        .word @@BigBoardTrap  ; Wario transform
        .word @@Heart
        .word @@BigBoardTrap  ; Lightning damage

    @@FullHealthItem:
        ldr r1, =CommonRoomEntityPalettes4 + 0x40  ; 3rd palette (normally OBP6)
        str r1, [r3]
        ldr r1, =ObjectPaletteF
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_halfwords(16)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        ; Heart graphic
        ldr r1, =BasicElementTiles + tile_coord_4b(24, 7)
        str r1, [r3]
        ldr r2, =0x6014000
        str r2, [r3, #4]
        ldr r0, =dma_enable | dma_words(8 * 34)
        str r0, [r3, #8]
        ldr r6, [r3, #8]

        ; Crown graphic
        ldr r1, =BasicElementTiles + tile_coord_4b(4, 1)
        str r1, [r3]
        add r2, #0x40
        str r2, [r3, #4]
        ldr r0, =dma_enable | dma_words(8 * 2)
        str r0, [r3, #8]
        ldr r0, [r3, #8]

        b @@Return

    @@Heart:
        ldr r1, =CommonRoomEntityPalettes4 + 0x60  ; 4th palette (normally OBP7)
        str r1, [r3]
        ldr r1, =ObjectPaletteF
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_halfwords(16)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        ldr r1, =BasicElementTiles + tile_coord_4b(10, 3)
        str r1, [r3]
        ldr r2, =0x6014000
        str r2, [r3, #4]
        ldr r0, =dma_enable | dma_words(8 * 34)
        str r0, [r3, #8]
        ldr r6, [r3, #8]

        b @@Return

    @@BigBoardTrap:
        ldr r1, =BigBoardEntityPalettes + 0x20  ; Second palette of list
        str r1, [r3]
        ldr r1, =ObjectPaletteF
        str r1, [r3, #4]
        ldr r1, =dma_enable | dma_halfwords(16)
        str r1, [r3, #8]
        ldr r1, [r3, #8]

        ; Get trap types
        ldr r6, =@@WarioFormTrapOffsets
        cmp r0, #ItemID_Lightning
        bne @@Upper3x2
        add r6, #4

    @@Upper3x2:
        ldr r1, =BigBoardEntityTiles
        ldrh r2, [r6]
        add r2, r1
        str r2, [r3]
        ldr r0, =0x6014000
        str r0, [r3, #4]
        ldr r2, =dma_enable | dma_words(8 * 35)
        str r2, [r3, #8]
        ldr r2, [r3, #8]

        ; Lower 4x1
        ldrh r2, [r6, #2]
        add r2, r1
        str r2, [r3]
        add r0, #0x60
        str r0, [r3, #4]
        ldr r2, =dma_enable | dma_words(8 * 4)
        str r2, [r3, #8]
        ldr r2, [r3, #8]

    @@Return:
        pop {r6, lr}
    .pool

    .align 2
    ; Offsets from PortalOBJTileset
    @@JewelPieceNEOffset:   .halfword tile_no_4b(0x31)
    @@JewelPieceSEOffset:   .halfword tile_no_4b(0x35)
    @@JewelPieceSWOffset:   .halfword tile_no_4b(0x39)
    @@JewelPieceNWOffset:   .halfword tile_no_4b(0x3D)
    @@CDOffset:             .halfword tile_no_4b(0x4A)

    .align 4
    @@AbilityPaletteTable:
        .word PassageTreasurePalettes             ; Ground Pound
        .word PassageTreasurePalettes + 4 * 0x20  ; Swim
        .word ExtraAbilityPalettes + 0x20         ; Head Smash
        .word PassageTreasurePalettes + 4 * 0x20  ; Grab
        .word ExtraAbilityPalettes                ; Dash Attack
        .word PassageTreasurePalettes + 0x20      ; Enemy Jump
        .word PassageTreasurePalettes             ; Super Ground Pound
        .word PassageTreasurePalettes + 2 * 0x20  ; Heavy Grab

    ; 3x2 top offset, 4x1 bottom offset
    @@WarioFormTrapOffsets: .halfword tile_coord_4b(22, 4), tile_coord_4b(28, 5)
    @@DamageTrapOffsets:    .halfword tile_coord_4b(12, 4), tile_coord_4b(25, 4)


.endautoregion
