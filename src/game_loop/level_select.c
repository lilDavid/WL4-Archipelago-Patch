#include <gba.h>

#include "unsorted/variables.h"
#include "unsorted/functions.h"
#include "item.h"


#ifdef DEBUG

// Ported from debug build (https://tcrf.net/Wario_Land_4/Debug_Build#Controls)
// - L + B: No items or bosses
// - L + Select: All items in Entry Passage, no Spoiled Rotten
// - L + D-pad up: All items + Spoiled Rotten only
// - L + R: All items + bosses
//
// Rando:
// - L + D-pad left: All locations
// - L + D-pad right: No locations
// - L + D-pad down: No abilities
// - L + Start: All abilities
void Debug_SetFlagsWithL() {
    if (KeyPressContinuous & KEY_L) {
        if (usTrg_KeyPress1Frame & KEY_B) {
            for (int i = PASSAGE_ENTRY; i < PASSAGE_MAX; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] &= ~0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0;
            }
            m4aSongNumStart(0x1D0);
        }
        if (usTrg_KeyPress1Frame & KEY_SELECT) {
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_1] |= 0xFF;
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_BOSS] = 0;
            for (int i = PASSAGE_EMERALD; i < PASSAGE_MAX; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] &= ~0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0;
            }
            m4aSongNumStart(0x1D0);
        }
        if (usTrg_KeyPress1Frame & KEY_UP) {
            for (int i = PASSAGE_ENTRY; i < PASSAGE_MAX; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] |= 0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0;
            }
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_BOSS] = 0x27;
            m4aSongNumStart(0x1D0);
        }
        if (usTrg_KeyPress1Frame & KEY_R) {
            for (int i = PASSAGE_ENTRY; i < PASSAGE_GOLDEN; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] |= 0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0x27;
            }
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_1] |= 0xFF;
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_BOSS] = 0;
            m4aSongNumStart(0x1D0);
        }

        // Rando

        // Locations
        if (usTrg_KeyPress1Frame & KEY_LEFT) {
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_1] &= 0xFF;
            for (int i = PASSAGE_EMERALD; i < PASSAGE_GOLDEN; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] &= 0xFF;
                }
            }
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_1] &= 0xFF;
            m4aSongNumStart(0x1D0);
        }
        if (usTrg_KeyPress1Frame & KEY_RIGHT) {
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_1] |= 0xFF << 8;
            for (int i = PASSAGE_EMERALD; i < PASSAGE_GOLDEN; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] |= 0xFF << 8;
                }
            }
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_1] |= 0xFF << 8;
            m4aSongNumStart(0x1D0);
        }

        // Abilities
        if (usTrg_KeyPress1Frame & KEY_START) {
            WarioAbilities = 0xFF;
            m4aSongNumStart(0x1D0);
        }
        if (usTrg_KeyPress1Frame & KEY_DOWN) {
            WarioAbilities = 0;
            m4aSongNumStart(0x1D0);
        }
    }
}

#endif
