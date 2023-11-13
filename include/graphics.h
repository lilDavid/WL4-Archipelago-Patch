#pragma once

#include <gba.h>


typedef struct { u8 bytes[0x20]; } Tile4bpp;

u8* LoadSpriteString(const u8* w4Str, Tile4bpp* tiles, u32 length);

// Allocation of tiles in VRAM in some formerly unused area in the basic sprite
// tiles.
extern Tile4bpp TilesReceived8[];
extern Tile4bpp TilesFrom4[];
extern Tile4bpp TilesSenderA8[];
extern Tile4bpp TilesSenderB8[];
extern Tile4bpp TilesItemA12[];
extern Tile4bpp TilesItemB8[];
extern Tile4bpp TilesItemC8[];
