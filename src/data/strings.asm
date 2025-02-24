.gba

.autoregion
.align 2
.sym off


.loadtable "data/charset.tbl"

.arm
LetterToSpriteTile:
    .word Text8x8_0, Text8x8_1, Text8x8_2, Text8x8_3, Text8x8_4, Text8x8_5, Text8x8_6, Text8x8_7
    .word Text8x8_8, Text8x8_9, Text8x8_A, Text8x8_B, Text8x8_C, Text8x8_D, Text8x8_E, Text8x8_F
    .word Text8x8_G, Text8x8_H, Text8x8_I, Text8x8_J, Text8x8_K, Text8x8_L, Text8x8_M, Text8x8_N
    .word Text8x8_O, Text8x8_P, Text8x8_Q, Text8x8_R, Text8x8_S, Text8x8_T, Text8x8_U, Text8x8_V
    .word Text8x8_W, Text8x8_X, Text8x8_Y, Text8x8_Z, Text8x8_A, Text8x8_B, Text8x8_C, Text8x8_D
    .word Text8x8_E, Text8x8_F, Text8x8_G, Text8x8_H, Text8x8_I, Text8x8_J, Text8x8_K, Text8x8_L
    .word Text8x8_M, Text8x8_N, Text8x8_O, Text8x8_P, Text8x8_Q, Text8x8_R, Text8x8_S, Text8x8_T
    .word Text8x8_U, Text8x8_V, Text8x8_W, Text8x8_X, Text8x8_Y, Text8x8_Z, Text8x8_Period, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, Emptytile, Text8x8_Comma, Text8x8_Period, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile
    .word EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile, EmptyTile

StrScreenFiller: .fill (TextBoxCharCount - 9), 0xFF
StrItemSent: .string "Sent "
StrItemTo: .string " to "
StrItemReceived: .string "Received "
StrItemFrom: .string "from "

; The MultiworldData tables will point into this area, which is intended to take up the rest of the space in the ROM.
.align 4
.sym on
MultiworldStringDump: .byte 0


.endautoregion
