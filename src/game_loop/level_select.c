#include <gba.h>

#include "unsorted/functions.h"
#include "graphics.h"
#include "item.h"
#include "randomizer.h"


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

    if (GoalType == GOAL_TREASURE_HUNT || GoalType == GOAL_DIVA_HUNT) {
        u16 goal_chest = GoalType == GOAL_DIVA_HUNT ? 32 : 0;
        u16* top = (u16*) 0x600C000;
        top[0] = 0xD0C0 + goal_chest;
        top[1] = 0xD0C1 + goal_chest;
        top[32] = 0xD0C2 + goal_chest;
        top[33] = 0xD0C3 + goal_chest;
        for (int i = 0; i < 5; i++) {
            top[i + 2] = 0xE100 + i;
            top[i + 34] = 0xE120 + i;
        }

        u16* bottom = (u16*) 0x600C480;
        bottom[1] = 0xC105;
        bottom[2] = 0xC106;
        bottom[33] = 0xC125;
        bottom[34] = 0xC126;
        for (int i = 0; i < 3; i++) {
            bottom[i + 3] = 0xE107 + i;
            bottom[i + 35] = 0xE127 + i;
        }

        int treasures_required = GoldenTreasuresNeeded;
        int treasure_count = 0;
        for (int i = PASSAGE_EMERALD; i <= PASSAGE_SAPPHIRE; i++) {
            for (int j = 0; j < 3; j++) {
                int flag = 1 << j;
                if (W4ItemStatus[i][LEVEL_BOSS] & flag)
                    treasure_count += 1;
            }
        }

        Tile4bpp* screenblock = (Tile4bpp*) 0x6000000;
        for (int i = 0; i < 2; i++) {
            int digit = _modsi3(treasure_count, 10);
            treasure_count = _divsi3(treasure_count, 10);
            dmaCopy(screenblock + TILE_OFFSET(digit, 10),
                    screenblock + TILE_OFFSET(1 - i, 8),
                    sizeof(Tile4bpp));
            dmaCopy(screenblock + TILE_OFFSET(digit, 11),
                    screenblock + TILE_OFFSET(1 - i, 9),
                    sizeof(Tile4bpp));

            digit = _modsi3(treasures_required, 10);
            treasures_required = _divsi3(treasures_required, 10);
            dmaCopy(screenblock + TILE_OFFSET(digit, 10),
                    screenblock + TILE_OFFSET(4 - i, 8),
                    sizeof(Tile4bpp));
            dmaCopy(screenblock + TILE_OFFSET(digit, 11),
                    screenblock + TILE_OFFSET(4 - i, 9),
                    sizeof(Tile4bpp));
        }

        // Copy treasure chest palette
        dmaCopy((void*) (GoalType == GOAL_DIVA_HUNT ? 0x83B14F0 : 0x83B29B0),
                &BG_PALETTE[16 * 13],
                16 * sizeof(u16));
    }

    // Replaced code
    MmapHekigaChange();
    MmapBestScoreSet();
    Select_Fade_Init();
}


void LevelSelect_CreateAbilityOAM() {
    if (HAS_ABILITY_PERMANENT(ABILITY_SUPER_GROUND_POUND)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 8),
            ATTR2_PALETTE(0x4) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x08C)
        );
    } else if (HAS_ABILITY_PERMANENT(ABILITY_GROUND_POUND)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 8),
            ATTR2_PALETTE(0x4) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x080)
        );
    }
    if (HAS_ABILITY_PERMANENT(ABILITY_SWIM)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 10),
            ATTR2_PALETTE(0x8) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x082)
        );
    }
    if (HAS_ABILITY_PERMANENT(ABILITY_HEAD_SMASH)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 12),
            ATTR2_PALETTE(0xA) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x084)
        );
    }
    if (HAS_ABILITY_PERMANENT(ABILITY_HEAVY_GRAB)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 14),
            ATTR2_PALETTE(0x6) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x08E)
        );
    } else if (HAS_ABILITY_PERMANENT(ABILITY_GRAB)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 14),
            ATTR2_PALETTE(0x8) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x086)
        );
    }
    if (HAS_ABILITY_PERMANENT(ABILITY_DASH_ATTACK)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 16),
            ATTR2_PALETTE(0x9) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x088)
        );
    }
    if (HAS_ABILITY_PERMANENT(ABILITY_ENEMY_JUMP)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(0),
            ATTR1_SIZE_16 | OBJ_X(8 * 18),
            ATTR2_PALETTE(0x5) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x08A)
        );
    }
}
