#pragma once

#include "unsorted/functions.h"

typedef enum {
    SE_BOX_OPEN = 0xDC,
    SE_THUNDER = 0x115,
    SE_CONFIRM = 0x125,
    SE_KEYZER_GET = 0x135,
    SE_DIAMOND_GET = 0x13A,
    SE_GEM_GET = 0x13B,
    SE_CD_GET = 0x13C,
    SE_HEART_GET = 0x140,
    SE_FULL_HEALTH_ITEM_GET = 0x143,
    SE_KEYZER_GET_UNDERWATER = 0x14A,
    SE_DIAMOND_GET_UNDERWATER = 0x14F,
    SE_ALL_BOXES_GET = 0x156,
    SE_RESULTS_FOUND_CD = 0x1D0,
    SE_MINIGAME_MEDAL_GET = 0x223,
    SE_HIGH_SCORE = 0x252,
    BGM_AFTER_DIVA_ESCAPE = 0x320,
} m4aSongNum;


LONGCALL void m4aSongNumStart(u32);
LONGCALL void m4aSongNumStop(u32);
