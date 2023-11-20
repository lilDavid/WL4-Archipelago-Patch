.gba


hook_manual 0x8084B10, 0x8084B1C, @Hook_SelectMmapInit  ; SelectMmapInit()
hook_manual 0x8085C62, 0x8085C6C, @Hook_SelectMmapOamCreate  ; SelectMmapOamCreate()


.autoregion
.align 2


.importobj "obj/game_loop/level_select.o"


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
