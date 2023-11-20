#include <gba.h>

#include "unsorted/functions.h"
#include "graphics.h"
#include "item.h"


void LevelSelect_InitIcons() {
    Tile4bpp* tile_dest = (Tile4bpp*) 0x6010000;
    dmaCopy(AbilityIconTilesTop,
            tile_dest + TILE_OFFSET(0, 4),
            16 * sizeof(Tile4bpp));
    dmaCopy(AbilityIconTilesBottom,
            tile_dest + TILE_OFFSET(0, 5),
            16 * sizeof(Tile4bpp));

    dmaCopy(PassageTreasurePalettes,
            SPRITE_PALETTE + 4 * 16,
            5 * 16 * sizeof(u16));
    dmaCopy(ExtraAbilityPalettes,
            SPRITE_PALETTE + 9 * 16,
            2 * 16 * sizeof(u16));

    // Replaced code
    MmapHekigaChange();
    MmapBestScoreSet();
    Select_Fade_Init();
}


void LevelSelect_CreateAbilityOAM() {
    if (WarioAbilities & (1 << ABILITY_SUPER_GROUND_POUND)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 8),
            ATTR2_PALETTE(0x4) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x08C)
        );
    } else if (WarioAbilities & (1 << ABILITY_GROUND_POUND)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 8),
            ATTR2_PALETTE(0x4) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x080)
        );
    }
    if (WarioAbilities & (1 << ABILITY_SWIM)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 10),
            ATTR2_PALETTE(0x8) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x082)
        );
    }
    if (WarioAbilities & (1 << ABILITY_HEAD_SMASH)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 12),
            ATTR2_PALETTE(0xA) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x084)
        );
    }
    if (WarioAbilities & (1 << ABILITY_HEAVY_GRAB)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 14),
            ATTR2_PALETTE(0x6) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x08E)
        );
    } else if (WarioAbilities & (1 << ABILITY_GRAB)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 14),
            ATTR2_PALETTE(0x8) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x086)
        );
    }
    if (WarioAbilities & (1 << ABILITY_DASH_ATTACK)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 16),
            ATTR2_PALETTE(0x9) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x088)
        );
    }
    if (WarioAbilities & (1 << ABILITY_ENEMY_JUMP)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 18),
            ATTR2_PALETTE(0x5) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x08A)
        );
    }
}
