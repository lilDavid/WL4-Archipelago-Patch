#pragma once


#define BG_PALETTE_EWRAM ((u16*) 0x2020800)
#define SPRITE_PALETTE_EWRAM ((u16*) 0x2020600)

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

// Length of a string literal, excluding the 0 byte at the end
#define LEN(x) (sizeof(x) - 1)
