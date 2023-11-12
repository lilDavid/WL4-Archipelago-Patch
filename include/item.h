#pragma once

#include <gba.h>

#include "vanilla/types.h"

extern u8 Has1stGemPiece;
extern u8 Has2ndGemPiece;
extern u8 Has3rdGemPiece;
extern u8 Has4thGemPiece;
extern u8 HasCD;
extern u8 HasKeyzer;
extern u8 HasFullHealthItem;
extern u8 HasFullHealthItem2;

typedef enum {
    ISB_GEM1 = 1 << 0,
    ISB_GEM2 = 1 << 1,
    ISB_GEM3 = 1 << 2,
    ISB_GEM4 = 1 << 3,
    ISB_CD = 1 << 4,
    ISB_KEYZER = 1 << 5,
    ISB_HEART = 1 << 6,
    ISB_HEART2 = 1 << 7
} ItemStatusBit;

typedef enum {
    ITEMBIT_CD = 1 << 5,
    ITEMBIT_ABILITY = 1 << 6,
    ITEMBIT_JUNK = 1 << 7
} ItemIdBit;

extern u32 W4ItemStatus[PASSAGE_MAX][LEVEL_MAX];
extern u8 WarioAbilities;

extern u8 QueuedFullHealthItem;
extern u8 QueuedFormTraps;
extern u8 QueuedHearts;
extern u8 QueuedLightningTraps;
