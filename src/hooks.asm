.gba


; ----------------------- Macros ----------------------


; Changes some code to a long jump. The code which this jumps to must jump back
; to the vanilla function where this was inserted.
; This macro will replace minimum 8 or 10 bytes depending on alignment, so the
; code that is jumped to may need to contain any instructions this displaces.
.macro jump_hook, Start, End, HackCode
    .org Start
    .area End-.
        ldr r0, =(HackCode) | 1
        bx r0
    .pool
    .endarea
.endmacro

; Changes some code to a long function call, followed by a jump to the specified
; address. The function must take no arguments and return void.
; This macro will replace minimum 12 or 14 bytes depending on alignment, so the
; function this calls may need to contain a copy of any code this displaces.
.macro call_hook_and_jump, Start, End, ReturnAddress, HackFunction
    .org Start
    .area End-.
        ldr r0, =(HackFunction) | 1
        bl _call_via_r0
        b ReturnAddress
    .pool
    .endarea
.endmacro

; Changes some code to a long function call. The function must take no arguments
; and return void.
; This macro will replace minimum 12 or 14 bytes depending on alignment, so the
; function this calls may need to contain a copy of any code this displaces.
.macro call_hook, Start, End, HackFunction
        call_hook_and_jump Start, End, End, HackFunction
.endmacro


; ------------------- Initialization ------------------


; HardwareInitialization()
call_hook 0x8000728, 0x8000738, InitializeVariables


; ------------------ Main game screen -----------------


; GameMain() Case 0
.org 0x801B8E4 :: .word @Hook_GameMain_LoadRoom
.org 0x801B928 :: pop {pc}  ; Instead of exiting the case, return like a function
; Case 2
.org 0x801B8EC
.word @Hook_GameMain_begin

; GameMain() case 2 (near the end)
call_hook 0x801BB7A, 0x801BB90, GameMain_RandoGraphics

; GameVBlkSet() parameter to SetVBlkInterruptHandler
.org 0x801BC08
        .word GameMain_ReceivedTextVBlk | 1

.autoregion
.align 2
@Hook_GameMain_LoadRoom:
        ; Store old pause flag
        ldr r0, =gPauseFlag
        ldrb r0, [r0]
        push {r0}

        ; "Call" case 0
        ldr r0, =0x801B908 | 1
        bl @@call

        pop {r0}
        cmp r0, #0
        beq @@skip
        bl ItemReloadInGameGraphics

    @@skip:
        ; increment sGameSeq and break, as usual
        ldr r0, =0x801B942
        mov pc, r0

    @@call:
        push {lr}
        bx r0
    .pool

; Receive multiworld items and collect junk (in level)
@Hook_GameMain_begin:
        bl GameMain_Rando
        ldr r0, =0x801B950
        mov pc, r0
    .pool
.endautoregion


; -------------- Passage select (pyramid) --------------


; GameSelect() case 2
.org 0x80799E0
.word @Hook_GameSelect2
.org 0x8079A90
.word @Hook_GameSelect2E

; SelectDmapOamCreate() returning
.org 0x807CA64
.area 0x807CA6C-.
        ldr r0, =@Hook_SelectDmapOamCreate | 1
        bx r0
    .pool
.endarea

; SelectVBlankIntr01 case 2
.org 0x807A574
        .word @Hook_SelectDmapVblk  ; Case 2

.autoregion
.align 2
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

@Hook_SelectDmapOamCreate:
        bl PassageSelect_CreateMessageBoxOAM
    ; Return from SelectDmapOamCreate
        pop {r4-r7}
        pop {r0}
        bx r0
    .pool

@Hook_GameSelect2E:
        bl EnterSoundRoom
        cmp r0, #1
        beq @@SkipCase2
    ; Run case 2E
        ldr r0, =0x807A2F4
        mov pc, r0
    @@SkipCase2:
        ldr r0, =0x807A36A
        mov pc, r0

@Hook_SelectDmapVblk:
        bl PassageSelect_Vblk
        ldr r0, =0x807A662
        mov pc, r0
    .pool

.endautoregion


; ------------- Level select (in passasge) -------------


jump_hook 0x8084B10, 0x8084B1C, @Hook_SelectMmapInit  ; SelectMmapInit()
jump_hook 0x8085C62, 0x8085C6C, @Hook_SelectMmapOamCreate  ; SelectMmapOamCreate()

.autoregion
.align 2
@Hook_SelectMmapInit:
        bl LevelSelect_InitIcons
        ldr r0, =0x8084B1C
        mov pc, r0
    .pool

@Hook_SelectMmapOamCreate:
        bl LevelSelect_CreateAbilityOAM
        mov r9, r4
        mov r10, r5
        pop {r4, r5, r6, r7}
        pop {r0}
        bx r0
    .pool
.endautoregion

; ---------------- Level results screen ----------------


; GameSelectSeisan()
; Case 4 - Speed up heart tally
.org 0x8080B80
    mov r0, #4
; Case 7 - Show item text box
call_hook 0x8080C5C, 0x8080C6C, @Hook_GameSelectSeisan7
; Case 8 - Let the player flip through items quickly
.org 0x8080CD8
    mov r0, #15
; Case 9 - Repeatedly show text box
.org 0x8080AB0
.word @Hook_GameSelectSeisan9

.autoregion
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


; ---------------- Collection indicator ----------------


; TKakeraIconDsp_main()
.org 0x8078E68
.word @Hook_TKakeraIconDsp_main_init  ; case 0
.word @Hook_TKakeraIconDsp_main_update  ; case 1
.word @Hook_TKakeraIconDsp_main_update  ; case 2
.word @Hook_TKakeraIconDsp_main_update  ; case 3
.word @Hook_TKakeraIconDsp_main_update  ; case 4

; TCardIconDsp_main()
call_hook_and_jump 0x80790B6, 0x80790CC, 0x80790D8, @Hook_TCardIconDSp_main_init
call_hook 0x8079112, 0x8079126, CDIcon_Update

.autoregion
.align 2
@Hook_TKakeraIconDsp_main_init:
        bl GemIcons_Init
        ldr r0, =0x8079064
        mov pc, r0
    .pool

@Hook_TKakeraIconDsp_main_update:
        bl GemIcons_Update
        ldr r0, =0x8079064
        mov pc, r0
    .pool

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
        ldr r3, =gCurrentSecondarySprite
        ldrb r1, [r3]
        pop {pc}

    .pool
.endautoregion

; ----------------------- Sprites ----------------------


; Change the boxes' opening routines to spawn their randomized item
call_hook 0x8029578, 0x8029592, SpawnRandomizedItemFromBox  ; 0x00 NE jewel box
call_hook 0x8029758, 0x8029772, SpawnRandomizedItemFromBox  ; 0x01 SE jewel box
call_hook 0x8029938, 0x8029952, SpawnRandomizedItemFromBox  ; 0x02 SW jewel box
call_hook 0x8029B18, 0x8029B32, SpawnRandomizedItemFromBox  ; 0x03 NW jewel box
call_hook 0x8029D06, 0x8029D24, SpawnRandomizedItemFromBox  ; 0x04 CD box
call_hook 0x8029F02, 0x8029F2A, SpawnRandomizedItemFromBox  ; 0x05 Full health item box

; Make the items do what they look like they do
call_hook 0x8029FBA, 0x802A012, CollectRandomItem  ; 0x86 NE jewel
call_hook 0x802A07E, 0x802A0D6, CollectRandomItem  ; 0x87 SE jewel
call_hook 0x802A142, 0x802A19A, CollectRandomItem  ; 0x88 SW jewel
call_hook 0x802A206, 0x802A25E, CollectRandomItem  ; 0x89 NW jewel
call_hook 0x802A2CA, 0x802A31E, CollectRandomItem  ; 0x8A CD
call_hook 0x802A38A, 0x802A3C4, CollectRandomItem  ; 0x8B Full health item

; Replace the items' graphics with the thing they give when collected
call_hook_and_jump 0x8029FA8, 0x8029FB8, 0x802A022, LoadRandomItemAnimation  ; 0x86 NE jewel
call_hook_and_jump 0x802A06C, 0x802A07C, 0x802A0E6, LoadRandomItemAnimation  ; 0x87 SE jewel
call_hook_and_jump 0x802A130, 0x802A140, 0x802A1AA, LoadRandomItemAnimation  ; 0x88 SW jewel
call_hook_and_jump 0x802A1F4, 0x802A204, 0x802A26E, LoadRandomItemAnimation  ; 0x89 NW jewel
call_hook_and_jump 0x802A2B8, 0x802A2C8, 0x802A32E, LoadRandomItemAnimation  ; 0x8A CD
call_hook_and_jump 0x802A378, 0x802A388, 0x802A3E6, LoadRandomItemAnimation  ; 0x8B Full health


; Replace sprite AI pointers
.org 0x878E800 + 6 * 4 :: .word RandoSpriteAI_Diamond | 1
.org 0x878E800 + 8 * 4 :: .word RandoSpriteAI_Keyzer | 1


; --------------------- Save data ----------------------


jump_hook 0x807845E, 0x8078468, @Hook_TTimeDsp_Main

; Override the end of this function to instead jump to our code
jump_hook 0x8074068, 0x8074070, @Hook_EXimage_Clear_Work_2Mode

; SeisanSave()
call_hook 0x80811BA, 0x8081284, CheckLocations

; BossSave()
call_hook 0x80813DC, 0x80813F8, CheckBossLocations
.org 0x80813F8
        ldr r3, =W4ItemStatus
        b 0x8081458
    .pool

; ItemGetFlgSet_LoadSavestateInfo2RAM()
call_hook_and_jump 0x8075E4C, 0x8075F10, 0x8075F38, SetItemCollection
jump_hook 0x8075F38, 0x8075F40, @Hook_LoadSaveStateInfo

.autoregion
.align 2

@Hook_EXimage_Clear_Work_2Mode:
        bl CreateStartingInventory
        pop {pc}  ; Returning from hooked function, LR already pushed
    .pool

@Hook_TTimeDsp_Main:
        bl BossDefeated_Save
        ldr r0, =0x80789EC | 1
        bx r0
    .pool

@Hook_LoadSaveStateInfo:
        bl ResetLevelVariables
        pop {r4, r5, r6}
        pop {r1}
        bx r1

.endautoregion


; -------------------- Fail states ---------------------

; GmWarioLifeZero()
call_hook 0x8075904, 0x8075910, WarioFailure

; TTimeDsp_main()
call_hook 0x8078670, 0x807867E, WarioFailure

; MainGameLoop()
call_hook_and_jump 0x8000518, 0x8000534, 0x800062E, @Hook_GiveUp

.autoregion
.align 2

@Hook_GiveUp:
        push {lr}

        bl GiveStoredDiamonds

        ldr r1, =GlobalGameMode
        mov r0, #1
        strh r0, [r1]
        ldr r1, =sGameSeq
        mov r0, #0x15

        pop {pc}
    .pool

.endautoregion

; ------------------ Cutscene Skips --------------------


.org 0x80799FC :: .word @Hook_GameSelect9
.org 0x8079A1C :: .word @Hook_GameSelect11
.org 0x8079A34 :: .word @Hook_GameSelect17
.org 0x8079A54 :: .word @Hook_GameSelect1F

.autoregion
.align 2
@Hook_GameSelect9:
        bl CutsceneSkip_EnterLevel
        ; Skip 4 bytes if true
        ldr r1, =0x8079C14
        lsl r2, r0, #2
        add r1, r2
        mov pc, r1
    .pool

@Hook_GameSelect11:
        bl CutsceneSkip_ThruPortal
        ; Skip 12 bytes if true
        ldr r1, =0x8079DB8
        lsl r2, r0, #1
        add r2, r0
        lsl r2, #2
        add r1, r2
        mov pc, r1
    .pool

@Hook_GameSelect17:
        bl CutsceneSkip_GiveUp_Clear
        ; Skip 4 bytes if true
        ldr r1, =0x8079ED4
        lsl r2, r0, #2
        add r1, r2
        mov pc, r1
        mov pc, r1
    .pool

@Hook_GameSelect1F:
        bl CutsceneSkip_GiveUp_Clear
        ; Skip 4 bytes if true
        ldr r1, =0x8079FAC
        lsl r2, r0, #2
        add r1, r2
        mov pc, r1
        mov pc, r1
    .pool

.endautoregion

; ----------------- 'Randomizer' title -----------------

call_hook_and_jump 0x8005F70, 0x8005F88, 0x800631E, LoadWarioLandLogo  ; TitleSubroutine()
