#pragma once

#include <gba.h>

#define THUMB_FUNCTION(addr, return_t, ...)  ((return_t (*)(__VA_ARGS__)) (addr | 1))

#define _modsi3                        THUMB_FUNCTION(0x8094ED0, s32, s32, s32)

#define m4aSongNumStart                THUMB_FUNCTION(0x8001DA4, void, u32)
#define MiniRandomCreate               THUMB_FUNCTION(0x8089B80, u16, void)

#define EnemyChildSet                  THUMB_FUNCTION(0x801E328, void, u8, u8, u8, s16, s16)
#define EntityAI_INITIAL_takara_kakera THUMB_FUNCTION(0x802932C, void, void)

#define WarioCoinSet                   THUMB_FUNCTION(0x80768B8, void, s32)
#define WarioVoiceSet                  THUMB_FUNCTION(0x8088620, void, s32)

#define MojiCreate                     THUMB_FUNCTION(0x80904DC, void, const u8*, u32, u8)
#define GmWarioCreate                  THUMB_FUNCTION(0x801C5D8, void, void)
#define EnemyDisplayMain               THUMB_FUNCTION(0x801D8C4, void, void)
#define ClearOamBuf                    THUMB_FUNCTION(0x8000A0C, void, void)
#define GmapSceneCreate                THUMB_FUNCTION(0x806C794, void, void)

typedef void (*WarioChngFunc)(u8);
extern WarioChngFunc WarioChng_React[2];

u32 W4strlen(const u8* str);
