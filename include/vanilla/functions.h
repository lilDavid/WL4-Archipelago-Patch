#pragma once

#include <gba.h>

#define THUMB_FUNCTION(addr, return_t, ...)  ((return_t (*)(__VA_ARGS__)) (addr | 1))

#define _modsi3 THUMB_FUNCTION(0x8094ED0, s32, s32, s32)

#define m4aSongNumStart THUMB_FUNCTION(0x8001DA4, void, u32)
#define MiniRandomCreate THUMB_FUNCTION(0x8089B80, u16, void)

typedef void (*WarioChngFunc)(u8);
extern WarioChngFunc WarioChng_React[2];
