#pragma once

#include <gba.h>


#define LONGCALL __attribute__((long_call))

LONGCALL s32 _divsi3(s32, s32);
LONGCALL s32 _modsi3(s32, s32);

LONGCALL void m4aSongNumStart(u32);
LONGCALL u16 MiniRandomCreate(void);

LONGCALL void EnemyChildSet(u8, u8, u8, s16, s16);
LONGCALL void EntityAI_INITIAL_takara_kakera(void);

LONGCALL void MojiCreate(const u8*, u32, u8);
LONGCALL void GmWarioCreate(void);
LONGCALL void EnemyDisplayMain(void);
LONGCALL void ClearOamBuf(void);
LONGCALL void Select_Fade_Init(void);

LONGCALL void GmapSceneCreate(void);
LONGCALL void MmapHekigaChange(void);
LONGCALL void MmapBestScoreSet(void);

u32 W4strlen(const u8* str);
