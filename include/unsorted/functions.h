#pragma once

#include <gba.h>

#define THUMB_FUNCTION(addr, return_t, ...)  ((return_t (*)(__VA_ARGS__)) (addr | 1))

#define _modsi3 THUMB_FUNCTION(0x8094ED0, s32, s32, s32)

#define m4aSongNumStart  THUMB_FUNCTION(0x8001DA4, void, u32)
#define MiniRandomCreate THUMB_FUNCTION(0x8089B80, u16, void)
#define MojiCreate       THUMB_FUNCTION(0x80904DC, void, const u8*, u32, u8)

#define EnemyChildSet                  THUMB_FUNCTION(0x801E328, void, u8, u8, u8, s16, s16)
#define EntityAI_INITIAL_takara_kakera THUMB_FUNCTION(0x802932C, void, void)

typedef void (*WarioChngFunc)(u8);
extern WarioChngFunc WarioChng_React[2];
