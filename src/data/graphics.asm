.gba
.arm


; Store letters and AP logo in some unused space in the tile data near the
; jewels and such. WL4 stores graphics data in 4bpp uncompressed 2D format.

sizeof_tile equ 0x20
.expfunc tile_no_4b(n), n * sizeof_tile
.expfunc tile_coord_4b(x, y), tile_no_4b(x + 32 * y)

; Spaces that will be used by text are now filled with empty tiles for clarity.
.org BasicElementTiles + tile_coord_4b(12, 4)
EmptyTile: .fill 12 * sizeof_tile, 0
.org BasicElementTiles + tile_coord_4b(14, 5)
.fill 10 * sizeof_tile, 0
.org BasicElementTiles + tile_coord_4b(14, 6)
.fill 10 * sizeof_tile, 0

.org 0x0869FC88
Text8x8_0: .skip sizeof_tile
Text8x8_1: .skip sizeof_tile
Text8x8_2: .skip sizeof_tile
Text8x8_3: .skip sizeof_tile
Text8x8_4: .skip sizeof_tile
Text8x8_5: .skip sizeof_tile
Text8x8_6: .skip sizeof_tile
Text8x8_7: .skip sizeof_tile
Text8x8_8: .skip sizeof_tile
Text8x8_9: .skip sizeof_tile

.org 0x08749870
Text8x8_A: .skip sizeof_tile
Text8x8_B: .skip sizeof_tile
Text8x8_C: .skip sizeof_tile
Text8x8_D: .skip sizeof_tile
Text8x8_E: .skip sizeof_tile
Text8x8_F: .skip sizeof_tile
Text8x8_G: .skip sizeof_tile
Text8x8_H: .skip sizeof_tile
Text8x8_I: .skip sizeof_tile
Text8x8_J: .skip sizeof_tile
Text8x8_K: .skip sizeof_tile
Text8x8_L: .skip sizeof_tile
Text8x8_M: .skip sizeof_tile
Text8x8_N: .skip sizeof_tile
Text8x8_O: .skip sizeof_tile
Text8x8_P: .skip sizeof_tile
Text8x8_Q: .skip sizeof_tile
Text8x8_R: .skip sizeof_tile
Text8x8_S: .skip sizeof_tile
Text8x8_T: .skip sizeof_tile
Text8x8_U: .skip sizeof_tile
Text8x8_V: .skip sizeof_tile
Text8x8_W: .skip sizeof_tile
Text8x8_X: .skip sizeof_tile
Text8x8_Y: .skip sizeof_tile
Text8x8_Z: .skip sizeof_tile
Text8x8_Period: .skip sizeof_tile
Text8x8_Comma: .skip sizeof_tile

; Repurpose the save tutorial and format it for arbitrary messages.
;
; The box is 14 tiles wide and 11 tiles tall.
; The first hexit of each halfword is the palette number (here always BGP 6),
; the last three are the index of the tile it displays.
; The text generating function places the bottom tile immediately after the top
; in memory, so each row gets alternating parity.

TextBoxCharCount equ 14 * (11 / 2)

.org SaveTutorialTilemap + 0x190
    .halfword 0x6080, 0x6082, 0x6084, 0x6086, 0x6088, 0x608A, 0x608C
    .halfword 0x608E, 0x6090, 0x6092, 0x6094, 0x6096, 0x6098, 0x609A

.org SaveTutorialTilemap + 0x1D0
    .halfword 0x6081, 0x6083, 0x6085, 0x6087, 0x6089, 0x608B, 0x608D
    .halfword 0x608F, 0x6091, 0x6093, 0x6095, 0x6097, 0x6099, 0x609B

.org SaveTutorialTilemap + 0x210
    .halfword 0x609C, 0x609E, 0x60A0, 0x60A2, 0x60A4, 0x60A6, 0x60A8
    .halfword 0x60AA, 0x60AC, 0x60AE, 0x60B0, 0x60B2, 0x60B4, 0x60B6

.org SaveTutorialTilemap + 0x250
    .halfword 0x609D, 0x609F, 0x60A1, 0x60A3, 0x60A5, 0x60A7, 0x60A9
    .halfword 0x60AB, 0x60AD, 0x60AF, 0x60B1, 0x60B3, 0x60B5, 0x60B7

.org SaveTutorialTilemap + 0x290
    .halfword 0x60B8, 0x60BA, 0x60BC, 0x60BE, 0x60C0, 0x60C2, 0x60C4
    .halfword 0x60C6, 0x60C8, 0x60CA, 0x60CC, 0x60CE, 0x60D0, 0x60D2

.org SaveTutorialTilemap + 0x2D0
    .halfword 0x60B9, 0x60BB, 0x60BD, 0x60BF, 0x60C1, 0x60C3, 0x60C5
    .halfword 0x60C7, 0x60C9, 0x60CB, 0x60CD, 0x60CF, 0x60D1, 0x60D3

.org SaveTutorialTilemap + 0x310
    .halfword 0x60D4, 0x60D6, 0x60D8, 0x60DA, 0x60DC, 0x60DE, 0x60E0
    .halfword 0x60E2, 0x60E4, 0x60E6, 0x60E8, 0x60EA, 0x60EC, 0x60EE

.org SaveTutorialTilemap + 0x350
    .halfword 0x60D5, 0x60D7, 0x60D9, 0x60DB, 0x60DD, 0x60DF, 0x60E1
    .halfword 0x60E3, 0x60E5, 0x60E7, 0x60E9, 0x60EB, 0x60ED, 0x60EF

.org SaveTutorialTilemap + 0x390
    .halfword 0x60F0, 0x60F2, 0x60F4, 0x60F6, 0x60F8, 0x60FA, 0x60FC
    .halfword 0x60FE, 0x6100, 0x6102, 0x6104, 0x6106, 0x6108, 0x610A

.org SaveTutorialTilemap + 0x3D0
    .halfword 0x60F1, 0x60F3, 0x60F5, 0x60F7, 0x60F9, 0x60FB, 0x60FD
    .halfword 0x60FF, 0x6101, 0x6103, 0x6105, 0x6107, 0x6109, 0x610B

; Not enough space, fill row with empty tiles
.org SaveTutorialTilemap + 0x410
    .fill 14 * 2, 0


; Add ability icons to the top of the screen

.definelabel PassageScreenTiles, 0x869CE48
.org PassageScreenTiles + tile_coord_4b(0, 6)
    .incbin "data/graphics/chest.gfx"
.org PassageScreenTiles + tile_coord_4b(0, 7)
    .incbin "data/graphics/jewel_box.gfx"
.org PassageScreenTiles + tile_coord_4b(16, 6)
    .incbin "data/graphics/ability_icons.gfx", 0x00, 12 * sizeof_tile
.org PassageScreenTiles + tile_coord_4b(16, 7)
    .incbin "data/graphics/ability_icons.gfx", 16 * sizeof_tile, 12 * sizeof_tile
.org PassageScreenTiles + tile_coord_4b(2, 8)
    .incbin "data/graphics/slash.gfx", 0x00, sizeof_tile
.org PassageScreenTiles + tile_coord_4b(2, 9)
    .incbin "data/graphics/slash.gfx", sizeof_tile, sizeof_tile

; Ground Pound, Grab, Dash Attack
.org 0x86A2648 + 0x402
    .halfword 0x50D0, 0x50D1, 0x50D6, 0x50D7, 0x50D8, 0x50D9
.org 0x86A2648 + 0x442
    .halfword 0x50F0, 0x50F1, 0x50F6, 0x50F7, 0x50F8, 0x50F9

; Swim, Head Smash, Stomp Jump
.org 0x86A2648 + 0x482
    .halfword 0x50D2, 0x50D3, 0x50D4, 0x50D5, 0x50DA, 0x50DB
.org 0x86A2648 + 0x4C2
    .halfword 0x50F2, 0x50F3, 0x50F4, 0x50F5, 0x50FA, 0x50FB

; "Steaks! Eat as much as you can!!"

; Upper tiles
.org 0x82D825E
    ; Clear time    XX:XX:XX
    .halfword 0x62E0, 0x62E2, 0x62E4, 0x62E6, 0x62E8, 0x03FF, 0x62EA, 0x62EC, 0x62EE, 0x62F0, 0x03FF
    .halfword 0x03FF, 0x03FF, 0x03FF, 0x6314, 0x6316, 0x62FB, 0x6318, 0x631A, 0x62FB, 0x631C, 0x631E
    ; Clear rate     XXX/XXX
    .halfword 0x62E0, 0x62E2, 0x62E4, 0x62E6, 0x62E8, 0x03FF, 0x62F2, 0x62F4, 0x62F6, 0x62F8, 0x03FF
    .halfword 0x03FF, 0x03FF, 0x03FF, 0x03FF, 0x6300, 0x6302, 0x6304, 0x62FC, 0x6306, 0x6308, 0x630A

; Lower tiles
    ; Clear time    XX:XX:XX
    .halfword 0x62E1, 0x62E3, 0x62E5, 0x62E7, 0x62E9, 0x03FF, 0x62EB, 0x62ED, 0x62EF, 0x62F1, 0x03FF
    .halfword 0x03FF, 0x03FF, 0x03FF, 0x6315, 0x6317, 0x62FB, 0x6319, 0x631B, 0x62FB, 0x631D, 0x631F
    ; Clear rate     XXX/XXX
    .halfword 0x62E1, 0x62E3, 0x62E5, 0x62E7, 0x62E9, 0x03FF, 0x62F3, 0x62F5, 0x62F7, 0x62F9, 0x03FF
    .halfword 0x03FF, 0x03FF, 0x03FF, 0x03FF, 0x6301, 0x6303, 0x6305, 0x62FD, 0x6307, 0x6309, 0x630B

.autoregion

.align 4
AbilityIconTilesTop:
    .incbin "data/graphics/ability_icons.gfx", 32 * sizeof_tile, 16 * sizeof_tile
AbilityIconTilesBottom:
    .incbin "data/graphics/ability_icons.gfx", 48 * sizeof_tile, 16 * sizeof_tile

APLogoTilesTop:
    .incbin "data/graphics/ap_logo.gfx", 0x00, 2 * sizeof_tile
APLogoTilesBottom:
    .incbin "data/graphics/ap_logo.gfx", 2 * sizeof_tile, 6 * sizeof_tile

EmptyAbilityTiles:
    .incbin "data/graphics/ability_get.gfx", 0, 16 * sizeof_tile
.org EmptyAbilityTiles
EmptyGroundPound1Tile: .skip sizeof_tile
EmptyGroundPound2Tile: .skip sizeof_tile
CarryingGroundPound1Tile: .skip sizeof_tile
CarryingGroundPound2Tile: .skip sizeof_tile
HasGroundPound1Tile: .skip sizeof_tile
HasGroundPound2Tile: .skip sizeof_tile

EmptyGrab1Tile: .skip sizeof_tile
EmptyGrab2Tile: .skip sizeof_tile
CarryingGrab1Tile: .skip sizeof_tile
CarryingGrab2Tile: .skip sizeof_tile
HasGrab1Tile: .skip sizeof_tile
HasGrab2Tile: .skip sizeof_tile

EmptyNonProgressiveAbilityTiles:
EmptySwimTile: .skip sizeof_tile
EmptyHeadSmashTile: .skip sizeof_tile
EmptyDashAttackTile: .skip sizeof_tile
EmptyEnemyJumpTile: .skip sizeof_tile

FilledNonProgressiveAbilityTiles:
    .incbin "data/graphics/ability_get.gfx", 18 * sizeof_tile, 4 * sizeof_tile
.org FilledNonProgressiveAbilityTiles
HasSwimTile: .skip sizeof_tile
HasHeadSmashTile: .skip sizeof_tile
HasDashAttackTile: .skip sizeof_tile
HasEnemyJumpTile: .skip sizeof_tile

.sym on
MinigameCoinTiles: .fill 4 * sizeof_tile
.sym off

sRandoTitleGfx: .incbin "data/graphics/randomizer.gfx"
sRandoTitlePal: .incbin "data/graphics/randomizer.pal"
sRandoTitleTilemap:
    .halfword 0x72EF, 0xA324, 0xA325, 0xA326, 0xA327, 0xA328, 0xA329, 0xA32A, 0xA32B, 0xA32C, 0xA32D, 0xA32E, 0xA32F, 0xA330, 0xA331, 0x72FE, 0x82FF, 0x8300
    .halfword 0xA335, 0xA336, 0xA337, 0xA338, 0xA339, 0xA33A, 0xA33B, 0xA33C, 0xA33D, 0xA33E, 0xA33F, 0xA340, 0xA341, 0xA342, 0xA343, 0xA344, 0xA345, 0xA346
    .halfword 0xA347, 0xA348, 0xA349, 0xA34A, 0xA34B, 0xA34C, 0xA34D, 0xA34E, 0xA34F, 0xA350, 0xA351, 0xA352, 0xA353, 0xA354, 0xA355, 0xA356, 0xA357, 0xA358
    .halfword 0x03FF, 0xA35A, 0xA35B, 0xA35C, 0xA35D, 0xA35E, 0xA35F, 0x03FF, 0x03FF, 0xA362, 0xA363, 0xA364, 0xA365, 0xA366, 0xA367, 0xA368, 0xA369, 0xA36A

.align 4
sRandoSlashTiles: .incbin "data/graphics/slash.gfx"
sRandoClearText: .string "Cleartimerate.                  "

.endautoregion
