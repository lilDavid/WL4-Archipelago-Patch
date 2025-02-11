#include <gba.h>

#include "unsorted/macros.h"
#include "unsorted/variables.h"
#include "graphics.h"


extern const u16 LevelPaletteEntryPassage[16];
extern const u16 LevelPaletteEmeraldPassage[16];
extern const u16 LevelPaletteRubyPassage[16];
extern const u16 LevelPaletteTopazPassage[16];
extern const u16 LevelPaletteSapphirePassage[16];
extern const u16 LevelPaletteGoldenPyramid[16];

#define EXTRA_PALETTE(color1, color2, color3, color4, color5) \
    { 0x42C0, 0x0000, 0x4169, 0x560E, 0x72F5, 0x7FFF, 0x7FFF, 0x02FD, \
      0x1FFF, 0x001F, 0x7B6B, color1, color2, color3, color4, color5, }

const u16 ExtraItemPalettes[][16] = {
    /* Garlic (dash attack) */           EXTRA_PALETTE(0x0000, 0x6BDF, 0x3FBF, 0x22FA, 0x0DAF),
    /* Bumbleprod helmet (head smash) */ EXTRA_PALETTE(0x0000, 0x0000, 0x0000, 0x05DE, 0x24C5),
    /* AP items */                       EXTRA_PALETTE(0x3D9C, 0x327D, 0x2B28, 0x6A3B, 0x6DED),
    /* Minigame medal */                 EXTRA_PALETTE(0x0000, 0x7FFF, 0x0BFF, 0x02FD, 0x0B9D),
};

const u16* ItemPaletteTable[PAL_MAX] = {
    [PAL_ENTRY] = LevelPaletteEntryPassage,
    [PAL_EMERALD] = LevelPaletteEmeraldPassage,
    [PAL_RUBY] = LevelPaletteRubyPassage,
    [PAL_TOPAZ] = LevelPaletteTopazPassage,
    [PAL_SAPPHIRE] = LevelPaletteSapphirePassage,
    [PAL_GOLDEN] = LevelPaletteGoldenPyramid,
    [PAL_GARLIC] = ExtraItemPalettes[0],
    [PAL_HELMET] = ExtraItemPalettes[1],
    [PAL_AP] = ExtraItemPalettes[2],
    [PAL_MINGAME_COIN] = ExtraItemPalettes[3],
};

void SetTreasurePalette(u32 palette) {
    const u16* source = ItemPaletteTable[palette];
    u16* destination;
    if (sGameSeq <= 1 && gColorFading.Kind == 2)
        destination = &SPRITE_PALETTE_EWRAM[4 * 16];
    else
        destination = &SPRITE_PALETTE[4 * 16];
    dmaCopy(source + 16 - 5, destination + 16 - 5, 5 * sizeof(u16));
}

void OamBuf_AddObj(u16 attr0, u16 attr1, u16 attr2) {
    if (ucCntObj >= 0x80)
        return;
    OamBuf[ucCntObj].attr0 = attr0;
    OamBuf[ucCntObj].attr1 = attr1;
    OamBuf[ucCntObj].attr2 = attr2;
    ucCntObj += 1;
}


// ------------- Graphics data ----------------


const TObjDef Item1Obj = {
    1,
    { ANM_OBJ(-8, -8, ATTR0_SQUARE, ATTR1_SIZE_16, 302, 4, 0) }
};

const TObjDef Item2Obj = {
    1,
    { ANM_OBJ(-8, -8, ATTR0_SQUARE, ATTR1_SIZE_16, 990, 4, 0) }
};

const TObjDef HeartObj = {
    1,
    { ANM_OBJ(-8, -8, ATTR0_SQUARE, ATTR1_SIZE_16, 234, 7, 0) }
};

const TObjDef GoldenTreasureObj = {
    2,
    { ANM_OBJ(-16, -16, ATTR0_WIDE, ATTR1_SIZE_32, 226, 1, 0),
      ANM_OBJ(-16,   0, ATTR0_WIDE, ATTR1_SIZE_32, 230, 1, 0) },
};

const TObjDef EmptyObj = {
    1,
    { { ATTR0_SQUARE | ATTR0_COLOR_16 | ATTR0_DISABLED | OBJ_Y(0),
        ATTR1_SIZE_8 | OBJ_X(0),
        ATTR2_PALETTE(0) | ATTR2_PRIORITY(0) | OBJ_CHAR(0), },
    }
};


const TAnmDef Item1Anm[] = {
    ANM_DEF(&Item1Obj, 0xFF),
    ANM_END
};

const TAnmDef Item2Anm[] = {
    ANM_DEF(&Item2Obj, 0xFF),
    ANM_END
};

const TAnmDef HeartAnm[] = {
    ANM_DEF(&HeartObj, 0xFF),
    ANM_END
};

const TAnmDef GoldenTreasureAnm[] = {
    ANM_DEF(&GoldenTreasureObj, 0xFF),
    ANM_END
};

const TAnmDef EmptyAnm[] = {
    ANM_DEF(&EmptyObj, 0xFF),
    ANM_END
};
