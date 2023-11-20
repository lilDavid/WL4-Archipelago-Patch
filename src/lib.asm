.gba


.autoregion
.align 2


; Count up to the next 0xFE byte.
; Parameters:
;  r0: Pointer to a WL4 encoded, 0xFE-terminated string
; Returns:
;  r0: The length of the string
W4strlen:
        mov r1, #0

    @@Next:
        ldrb r2, [r0]
        cmp r2, #0xFE
        beq @@Return
        add r0, #1
        add r1, #1
        b @@Next

    @@Return:
        mov r0, r1
        mov pc, lr
    .pool


.endautoregion
