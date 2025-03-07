.gba

; Shuffle Wario's abilities

; The WarioAbilities bits are encoded as follows:
; 0: Ground Pound
; 1: Swimming
; 2: Break blocks with head
; 3: Grab light objects
; 4: Dash Attack
; 5: Stomp Jump
; 6: Super Ground Pound
; 7: Grab heavy objects

MoveBit_GroundPound equ 0
MoveBit_Swim equ 1
MoveBit_HeadSmash equ 2
MoveBit_Grab equ 3
MoveBit_DashAttack equ 4
MoveBit_EnemyJump equ 5
MoveBit_GroundPoundSuper equ 6
MoveBit_GrabHeavy equ 7

.macro get_wario_move, ability
        ldr r0, =WarioAbilities
        ldr r1, =AbilitiesInThisLevel
        ldrb r0, [r0]
        ldrb r1, [r1]
        orr r0, r1
        get_bit r0, r0, ability
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


jump_hook 0x8010AE6, 0x8010AF2, LimitWarioAbility_GroundPound       ; WarKeyJump()
jump_hook 0x80117B4, 0x80117BE, LimitWarioAbility_GroundPoundSuper  ; WarKeyHip()

jump_hook 0x8015E76, 0x8015E82, LimitWarioAbility_Swim_Underwater   ; WarKeySwim2()
jump_hook 0x8015FC4, 0x8015FE8, LimitWarioAbility_Swim_Surfaced     ; WarKeySwim3()
jump_hook 0x8016114, 0x801611C, LimitWarioAbility_Swim_FloatingB    ; WarKeySwStop()
jump_hook 0x801615C, 0x801618C, LimitWarioAbility_Swim_FloatingD    ; WarKeySwStop()
jump_hook 0x801624A, 0x8016254, LimitWarioAbility_Swim_GroundPound  ; WarKeySwHip()
.org 0x082DEF2C :: .word LimitWarioAbility_Swim_Fat | 1             ; Override function

jump_hook 0x806EE56, 0x806EE60, LimitWarioAbility_HeadSmash         ; WarUpPanel_Attack()

jump_hook 0x801F9E8, 0x801F9F2, LimitWarioAbility_Grab              ; It's complicated

jump_hook 0x80106C0, 0x80106D2, LimitWarioAbility_DashAttack_Left   ; WarKeyWalk()
jump_hook 0x8010640, 0x801064A, LimitWarioAbility_DashAttack_Right  ; WarKeyWalk()
.org 0x806ECFC :: .word LimitWarioAbility_DashAttack_Roll           ; WarSidePanelAttack() case 14
.org 0x806ED00 :: .word LimitWarioAbility_DashAttack_Roll           ; WarSidePanelAttack() case 15

jump_hook 0x8012C60, 0x8012C6C, LimitWarioAbility_EnemyJump         ; GmWarioChng()


.autoregion
.align 2


; Disable starting a ground pound unless unlocked
LimitWarioAbility_GroundPound:
        get_wario_move MoveBit_GroundPound
        cmp r0, #0
        beq @@NoGroundPound

    ; Replaced code
        mov r0, #0x80
        and r0, r2
        cmp r0, #0
        beq @@NoGroundPound

    ; Ground pound
        mov r0, #0x1B
        ldr r1, =0x8010BFE
        mov pc, r1

    @@NoGroundPound:
        ldr r1, =0x8010AF2
        mov pc, r1

    .pool


; Prevent ground pound from powering up without the upgrade
LimitWarioAbility_GroundPoundSuper:
        get_wario_move MoveBit_GroundPoundSuper
        cmp r0, #0
        beq @@NormalGroundPound

    ; Replaced code
        ldrb r0, [r4, #10]  ; Wario_ucTimer
        cmp r0, #0x17
        bls @@NormalGroundPound

    ; Super ground pound
        mov r0, #0x1C
        ldr r1, =0x8011886
        mov pc, r1

    @@NormalGroundPound:
        ldr r1, =0x80117BE
        mov pc, r1

    .pool


; Disable this state if swimming isn't unlocked
LimitWarioAbility_Swim_Underwater:
        get_wario_move MoveBit_Swim
        cmp r0, #0
        bne @@Swim
        mov r0, #5

        ldr r1, =0x8015F76
        mov pc, r0

    @@Swim:  ; Replaced code
        ldr r0, =usTrg_KeyPress1Frame
        ldrh r1, [r0]
        mov r0, #1
        and r0, r1
        cmp r0, #0
        beq  @@E94

        ldr r0, =0x8015E82
        mov pc, r0

    @@E94:
        ldr r0, =0x8015E94
        mov pc, r0

    .pool


; Prevent diving if swimming isn't unlocked
LimitWarioAbility_Swim_Surfaced:
        get_wario_move MoveBit_Swim
        ldr r1, =KeyPressContinuous
        ldrh r2, [r1]
        cmp r0, #0
        beq @@NoSwim

    ; Replaced code
        mov r0, #2
        and r0, r1
        cmp r0, #0
        beq @@CheckDirections

        ldr r1, =Wario
        ldrh r0, [r1, #0x14]  ; Wario_usPosY
        add r0, #0x10
        strh r0, [r1, #0x14]
        mov r0, #1

        ldr r1, =0x80160E8
        mov pc, r1

    @@CheckDirections:
        mov r0, #0x80
        and r0, r2
        cmp r0, #0
        beq @@NoSwim

    ; Swim
        ldr r0, =0x8015FE8
        mov pc, r0

    @@NoSwim:
        ldr r0, =0x8015FFC
        mov pc, r0

    .pool


; Prevent swimming with B if swimming isn't unlocked
LimitWarioAbility_Swim_FloatingB:
        get_wario_move MoveBit_Swim
        ldr r1, =usTrg_KeyPress1Frame
        ldrh r1, [r1]
        cmp r0, #0
        beq @@NoSwim

        mov r0, #2
        and r0, r1
        cmp r0, #0
        beq @@NoSwim

    ; Swim
        ldr r0, =0x801611C
        mov pc, r0

    @@NoSwim:
        ldr r0, =0x8016128
        mov pc, r0

    .pool


; Prevent directional input if swimming isn't unlocked
LimitWarioAbility_Swim_FloatingD:
        get_wario_move MoveBit_Swim
        cmp r0, #0
        beq @@NoSwim

    ; Replaced code
        ldr r0, =KeyPressContinuous
        ldrh r0, [r0]
        mov r1, #0x30
        and r1, r0
        cmp r1, #0
        beq @@CheckVert

        ldr r0, =Wario
        strh r1, [r0, #0xE]  ; Wario_usMukiX
        mov r0, #3
        b @@Swim

    @@CheckVert:
        mov r1, #0xC0
        and r1, r0
        cmp r1, #0
        beq @@NoSwim
        ldr r0, =Wario
        strh r1, [r0, #0x10]  ; Wario_usMukiY
        mov r0, #3

    @@Swim:
        ldr r1, =0x80161DA
        mov pc, r1

    @@NoSwim:
        ldr r0, =0x801618C
        mov pc, r0

    .pool


; If super-ground-pounding into water and swim isn't unlocked, freeze Wario in
; place to prevent softlocks
LimitWarioAbility_Swim_GroundPound:
        get_wario_move MoveBit_Swim
        cmp r0, #0
        bne @@ReplacedCode

    ; Mask away the left and right bits from KeyPressContinuous
        mov r0, #0x30
        mvn r0, r0
        and r4, r0

    @@ReplacedCode:
        mov r5, r4
        and r5, r2
        ldr r3, =Wario
        cmp r5, #0
        beq @@NoSwim

        ldr r0, =0x8016254
        mov pc, r0

    @@NoSwim:
        ldr r0, =0x8016278
        mov pc, r0

    .pool


; If Fat Wario falls into water and swim isn't unlocked, freeze him in place to
; prevent softlocks
LimitWarioAbility_Swim_Fat:
        push {lr}

        ldr r0, =0x80163F8 | 1  ; WarKeySwFat()
        bl @@call
        mov r3, r0

        get_wario_move MoveBit_Swim
        cmp r0, #0
        bne @@return

        ldr r0, =Wario
        mov r1, #0
        strh r1, [r0, #0x16]  ; Wario.sMvSpeedX

    @@return:
        mov r0, r3
        pop {pc}
    @@call:
        bx r0
    .pool


; Prevent Wario breaking blocks with his head.
; Bouncy Wario can still break blocks.
LimitWarioAbility_HeadSmash:
        ldr r0, =Wario
        ldrb r0, [r0]
        cmp r0, #7
        beq @@HeadSmash

        get_wario_move MoveBit_HeadSmash
        cmp r0, #0
        bne @@HeadSmash

        ldr r0, =0x806EE7C
        mov pc, r0

    @@HeadSmash:
        mov r0, sp
        ldr r1, =PanelPartWork_Broken_Main | 1
        bl @@call_via_r1
        mov r3, r0
        cmp r3, #1

        ldr r0, =0x806EE60
        mov pc, r0

    @@call_via_r1:
        bx r1

    .pool


; Prevent Wario from lifting or catching heavy enemies without two Progressive
; Grabs, or any enemies without any grabs
LimitWarioAbility_Grab:
        ldrb r0, [r6]
        cmp r0, #0
        beq @@CheckWeight
        ldr r0, =0x801FA5A
        mov pc, r0

    @@CheckWeight:
        ldr r1, =gSpriteData
        mov r0, #0x2C
        mul r0, r7
        add r0, r0, r1
        ldrh r1, [r0]
        mov r0, #0x20
        and r0, r1
        cmp r0, #0
        beq @@CheckGrab

    ; If heavy
        get_wario_move MoveBit_GrabHeavy
        cmp r0, #0
        beq @@NoGrab

    @@CheckGrab:
        get_wario_move MoveBit_Grab
        cmp r0, #0
        bne @@Grab

    @@NoGrab:
        mov r0, #1
        b @@Return

    @@Grab:
        ldr r0, =WarioLift
        ldrb r0, [r0]

    @@Return:
        ldr r1, =0x801F9F2
        mov pc, r1

    .pool


; Prevent Wario from starting a dash attack without the item (going left)
LimitWarioAbility_DashAttack_Left:
        sub r0, r1, #2
        strh r0, [r6, #0x16]  ; Wario_sMvSpeedX
        lsl r0, r0, #0x10
        asr r0, r0, #0x10
        mov r1, #0x80
        neg r1, r1
        cmp r0, r1
        bgt @@NoDashAttack
        strh r1, [r6, #0x16]

        get_wario_move MoveBit_DashAttack
        cmp r0, #0
        beq @@NoDashAttack

        ldr r0, =0x80106D2
        mov pc, r0

    @@NoDashAttack:
        ldr r0, =0x801073C
        mov pc, r0

    .pool


; Prevent Wario from starting a dash attack without the item (going right)
LimitWarioAbility_DashAttack_Right:
        add r0, r1, #2
        strh r0, [r6, #0x16]  ; Wario_sMvSpeedX
        lsl r0, r0, #0x10
        asr r0, r0, #0x10
        cmp r0, #0x7F
        ble @@NoDashAttack
        mov r0, #0x80
        strh r0, [r6, #0x16]

        get_wario_move MoveBit_DashAttack
        cmp r0, #0
        beq @@NoDashAttack

        ldr r0, =0x80106D2
        mov pc, r0

    @@NoDashAttack:
        ldr r0, =0x801073C
        mov pc, r0

    .pool


; Prevent Wario breaking hard blocks by rolling into them without Dash Attack
LimitWarioAbility_DashAttack_Roll:
        get_wario_move MoveBit_DashAttack
        cmp r0, #0
        bne @@Break
        mov r0, sp
        mov r1, #1
        strb r1, [r0, #0x8]  ; attack strength against this block

    @@Break:
        ldr r0, =0x806ED4A
        mov pc, r0

    .pool


LimitWarioAbility_EnemyJump:
        get_wario_move MoveBit_EnemyJump
        ldr r1, =KeyPressContinuous
        ldrh r1, [r1]
        cmp r0, #0
        beq @@NoHighJump

        mov r0, #1
        and r0, r1
        cmp r0, #0
        beq @@NoHighJump

        ldr r0, =0x8012C6C
        mov pc, r0

    @@NoHighJump:
        ldr r0, =0x8012C74
        mov pc, r0

    .pool


.endautoregion
