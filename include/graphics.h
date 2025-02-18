#pragma once

#include <gba.h>

#include "unsorted/functions.h"
#include "item.h"


typedef enum {
    PAL_ENTRY,
    PAL_EMERALD,
    PAL_RUBY,
    PAL_TOPAZ,
    PAL_SAPPHIRE,
    PAL_GOLDEN,
    PAL_GARLIC,
    PAL_GLOVES,
    PAL_HELMET,
    PAL_AP,
    PAL_MINGAME_COIN,
    PAL_MAX
} PassagePalette;

typedef struct {
    u16 attr0;
    u16 attr1;
    u16 attr2;
} TObjDef_Attr;

typedef struct {
    u16 length;
    TObjDef_Attr objects[];
} TObjDef;

typedef struct {
    const TObjDef* uiObjAddr;
    u8 ucTimer;
    u8 unused[3];
} TAnmDef;

typedef struct {
    u8 Kind;
    u8 Cnt;
    u8 Dat;
    u8 Dip;
    u8 DMAf;
    u8 SW;
} ColorFading;


typedef struct { u8 bytes[0x20]; } Tile4bpp;

#define TILE_NO_4B(n) ((n) * sizeof(Tile4bpp))
#define TILE_OFFSET(x, y) ((x) + 32 * (y))
#define TILE_COORD_4B(x, y) (TILE_NO_4B(TILE_OFFSET(x, y)))

extern const Tile4bpp BasicElementTiles[];
extern const Tile4bpp* GoldenTreasureTiles[15];
extern const u16 GoldenTreasurePalette[16];

void SetTextColor(u16 color);
const u8* LoadSpriteString(const u8* w4Str, Tile4bpp* tiles, u32 length);

extern const Tile4bpp AbilityIconTilesTop[ABILITY_MAX * 2];
extern const Tile4bpp AbilityIconTilesBottom[ABILITY_MAX * 2];
extern const Tile4bpp APLogoTilesTop[2];
extern const Tile4bpp APLogoTilesBottom[2];
extern const Tile4bpp MinigameCoinTiles[4];


#define ANM_OBJ_ATTR(shape, y, size, x, palette, priority, tile) {\
    (shape) | ATTR0_COLOR_16 | OBJ_Y(y),\
    (size) | OBJ_X(x),\
    ATTR2_PALETTE(palette) | ATTR2_PRIORITY(priority) | OBJ_CHAR(tile)\
}
#define ANM_OBJ(x, y, shape, size, tile, palette, priority) \
    ANM_OBJ_ATTR(shape, y, size, x, palette, priority, tile)

#define ANM_DEF(obj_ptr, timer) {(obj_ptr), (timer), {0}}
#define ANM_END ANM_DEF(NULL, 0)

extern const TAnmDef takara_Anm_00[];  // CD
extern const TAnmDef takara_Anm_01[];  // Full health item
extern const TAnmDef DiamondAnm[];
extern const TAnmDef Item1Anm[];
extern const TAnmDef Item2Anm[];
extern const TAnmDef HeartAnm[];
extern const TAnmDef EmptyAnm[];
extern const TAnmDef GoldenTreasureAnm[];

extern u8 ucCntObj;
extern OBJATTR OamBuf[128];

void OamBuf_AddObj(u16 attr0, u16 attr1, u16 attr2);
LONGCALL void ClearOamBuf(void);

extern ColorFading gColorFading;

extern const u16* ItemPaletteTable[PAL_MAX];
