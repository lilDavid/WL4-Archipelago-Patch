#include <gba.h>

#include "unsorted/macros.h"
#include "color.h"
#include "game_state.h"
#include "graphics.h"


extern const u16 sLevelEntryPal[16];
extern const u16 sLevelEmeraldPal[16];
extern const u16 sLevelRubyPal[16];
extern const u16 sLevelTopazPal[16];
extern const u16 sLevelSapphirePal[16];
extern const u16 sLevelGoldenPal[16];

#define EXTRA_PALETTE(color1, color2, color3, color4, color5) \
    { 0x42C0, 0x0000, 0x4169, 0x560E, 0x72F5, 0x7FFF, 0x7FFF, 0x02FD, \
      0x1FFF, 0x001F, 0x7B6B, color1, color2, color3, color4, color5, }

const u16 sGarlicPal[16] = EXTRA_PALETTE(0x6BDF, 0x573C, 0x3A56, 0x25B2, COLOR_BLACK);
const u16 sGlovePal[16] = EXTRA_PALETTE(0x6BDF, 0x23DF, 0x7D29, 0x50A5, 0x1274);
const u16 sHelmetPal[16] = EXTRA_PALETTE(COLOR_BLACK, CONVERT_COLOR_HEX(0xF07008), CONVERT_COLOR_HEX(0xAB4D06), CONVERT_COLOR_HEX(0x2B354C), CONVERT_COLOR_HEX(0x1B212F));
const u16 sAPItemPal[16] = EXTRA_PALETTE(0x3D9C, 0x327D, 0x2B28, 0x6A3B, 0x6DED);
const u16 sMedalPal[16] = EXTRA_PALETTE(0x0000, 0x7FFF, 0x0BFF, 0x02FD, 0x0B9D);

const u16* ItemPaletteTable[PAL_MAX] = {
    [PAL_ENTRY] = sLevelEntryPal,
    [PAL_EMERALD] = sLevelEmeraldPal,
    [PAL_RUBY] = sLevelRubyPal,
    [PAL_TOPAZ] = sLevelTopazPal,
    [PAL_SAPPHIRE] = sLevelSapphirePal,
    [PAL_GOLDEN] = sLevelGoldenPal,
    [PAL_GARLIC] = sGarlicPal,
    [PAL_GLOVES] = sGlovePal,
    [PAL_HELMET] = sHelmetPal,
    [PAL_AP] = sAPItemPal,
    [PAL_MEDAL] = sMedalPal,
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
    if (ucCntObj >= ARRAY_SIZE(OamBuf))
        return;
    OamBuf[ucCntObj].attr0 = attr0;
    OamBuf[ucCntObj].attr1 = attr1;
    OamBuf[ucCntObj].attr2 = attr2;
    ucCntObj += 1;
}


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
