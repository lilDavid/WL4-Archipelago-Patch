.gba

; Implement checking items.


; Give the player their progression items from what they collected in the level.


.autoregion


.align 2
; Combine the current and temporary-in-level abilities to get the abilities
; Wario is currently carrying.
; Returns:
;     r0: Abilities Wario will have if and when he escapes this level
MixTemporaryAbilities:
        ldr r0, =AbilitiesInThisLevel
        ldr r1, =WarioAbilities
        ldrb r0, [r0]
        ldrb r1, [r1]
        orr r0, r1

        mov pc, lr

    .pool


; Identify a full health item box based on what room the player is in.
; Returns:
;     r0: 1 if the player is in Pinball Zone but not the FHB pink room; 0 otherwise
GetFullHealthBoxID:
        ldr r2, =PassageID
        ldrb r2, [r2]
        cmp r2, #2  ; Ruby Passage
        bne @@Box1
        ldr r2, =InPassageLevelID
        ldrb r2, [r2]
        cmp r1, #4  ; Pinball Zone
        bne @@Box1
        ldr r2, =CurrentRoomId
        ldrb r2, [r2]
        cmp r1, #8  ; Pink room with the heart box in the center
        beq @@Box1

        mov r0, #1
        mov pc, lr

    @@Box1:
        mov r0, #0
        mov pc, lr
    .pool


.endautoregion
