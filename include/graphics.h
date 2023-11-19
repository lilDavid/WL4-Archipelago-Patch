#pragma once

#include <gba.h>

#include "item.h"


typedef enum {
    PAL_ENTRY,
    PAL_EMERALD,
    PAL_RUBY,
    PAL_TOPAZ,
    PAL_SAPPHIRE,
    PAL_GOLDEN,
    PAL_AP,
    PAL_GARLIC,
    PAL_HELMET,
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
    TObjDef* uiObjAddr;
    u8 ucTimer;
    u8 unused[3];
} TAnmDef;


typedef struct { u8 bytes[0x20]; } Tile4bpp;

#define TILE_NO_4B(n) ((n) * sizeof(Tile4bpp))
#define TILE_OFFSET(x, y) ((x) + 32 * (y))
#define TILE_COORD_4B(x, y) (TILE_NO_4B(TILE_OFFSET(x, y)))

extern const Tile4bpp BasicElementTiles[];

const u8* LoadSpriteString(const u8* w4Str, Tile4bpp* tiles, u32 length);

extern const Tile4bpp AbilityIconTilesTop[ABILITY_MAX * 2];
extern const Tile4bpp AbilityIconTilesBottom[ABILITY_MAX * 2];

// Allocation of tiles in VRAM in some formerly unused area in the basic sprite
// tiles.
extern Tile4bpp TilesReceived8[];
extern Tile4bpp TilesFrom4[];
extern Tile4bpp TilesSenderA8[];
extern Tile4bpp TilesSenderB8[];
extern Tile4bpp TilesItemA12[];
extern Tile4bpp TilesItemB8[];
extern Tile4bpp TilesItemC8[];


extern u8 ucCntObj;
extern OBJATTR OamBuf[128];
