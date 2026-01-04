#pragma once

#include <gba.h>


#define LONGCALL __attribute__((long_call))
#define UNUSED __attribute__((unused))

LONGCALL s32 _divsi3(s32, s32);
LONGCALL s32 _modsi3(s32, s32);

LONGCALL u16 MiniRandomCreate(void);

LONGCALL void GmStScoreCalc(u32);

LONGCALL void Select_Fade_Init(void);

LONGCALL void GmapSceneCreate(void);
LONGCALL void MmapHekigaChange(void);
LONGCALL void MmapBestScoreSet(void);

LONGCALL void T_NoCorrectionBgAttack(void);
