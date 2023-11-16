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


.endautoregion
