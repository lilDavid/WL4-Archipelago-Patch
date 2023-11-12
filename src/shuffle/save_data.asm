
; Override the end of this function to instead jump to our code
.org 0x8074068
.area 0x8074070-.
        ldr r0, =@Hook_EXimage_Clear_Work_2Mode | 1
        bx r0
    .pool
.endarea

; SeisanSave()
hook 0x80811BA, 0x8081284, CheckLocations

; ItemGetFlgSet_LoadSavestateInfo2RAM()
hook_branch 0x8075E4C, 0x8075F10, 0x8075F38, SetItemCollection


.autoregion

@Hook_EXimage_Clear_Work_2Mode:
        bl CreateStartingInventory
        pop {pc}  ; Returning from hooked function, LR already pushed

.importobj "obj/shuffle/save_data.o"

.endautoregion
