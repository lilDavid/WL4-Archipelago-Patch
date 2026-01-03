#include <gba.h>

#include "unsorted/macros.h"
#include "color.h"

extern u16 sLocalSeq;

extern const u8 sTitleBackgroundGfx[];
extern const u8 sTitleGfx[];

extern const u8 sRandoTitleGfx[2304];
extern const u16 sRandoTitlePal[16];
extern const u16 sRandoTitleTilemap[4][18];

void LoadWarioLandLogo(void) {
    // Vanilla title
    LZ77UnCompVram(sTitleBackgroundGfx, CHAR_BASE_BLOCK(0));
    LZ77UnCompVram(sTitleGfx, CHAR_BASE_BLOCK(1));

    // Randomizer title
    dmaCopy(sRandoTitleGfx, (void*) VRAM + 0x6460, sizeof(sRandoTitleGfx));
    dmaCopy(sRandoTitlePal, BG_PALETTE + 16 * 10, sizeof(sRandoTitlePal));
    for (u32 i = 0; i < ARRAY_SIZE(sRandoTitleTilemap); i++) {
        dmaCopy(
            sRandoTitleTilemap[i],
            SCREEN_BASE_BLOCK(16) + 2 * (32 * (i + 5) + 6),
            sizeof(sRandoTitleTilemap[0])
        );
    }

    sLocalSeq++;
}
