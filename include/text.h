#pragma once

#include <gba.h>

#include "unsorted/functions.h"


// Ghidra repo comments the branch taken when usMojiCount is set to this as "fixed text"
#define TEXT_FIXED 1000

extern u16 usMojiCount;
LONGCALL void MojiCreate(const u8*, u32, u8);

extern u8 sRandoSlashTiles[32 * 2];

u32 W4strlen(const u8* str);
