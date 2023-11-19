
; GameMain() case 2
.org 0x801B8EC
.word @Hook_GameMain_begin

; GameMain() case 2 (near the end)
hook 0x801BB7A, 0x801BB90, GameMain_RandoGraphics


.autoregion


.importobj "obj/game_loop/game_main.o"


; Receive multiworld items and collect junk (in level)
.align 2
@Hook_GameMain_begin:
        bl GameMain_Rando
        ldr r0, =0x801B950
        mov pc, r0
    .pool


.endautoregion
