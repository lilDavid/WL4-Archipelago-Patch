#pragma once

#include <gba.h>

#include "unsorted/functions.h"


typedef enum {
    WV_TREASURE = 1,
    WV_KEYZER = 2,
    WV_HURT = 4,
    WV_SORRY = 9,
} WarioVoiceID;

typedef enum {
    REACT_NORMAL,
    REACT_WATER,
    REACT_FIRE,
    REACT_FAT,
    REACT_FROZEN,
    REACT_ZOMBIE,
    REACT_SNOWMAN,
    REACT_BOUNCY,
    REACT_PUFFY,
    REACT_BAT,
    REACT_FLAT,
    REACT_MASK,
    REACT_MAX
} WarioReact;

typedef struct {
    /* 0x00 */ u8 ucReact;
    /* 0x01 */ u8 ucStat;
    /* 0x02 */ u8 ucDokan;
    /* 0x03 */ u8 ucTurn;
    /* 0x04 */ u8 ucMiss;
    /* 0x05 */ u8 ucMuteki;
    /* 0x06 */ u8 ucTumiki;
    /* 0x07 */ u8 ucJpNext;
    /* 0x08 */ u8 ucJpFlg;
    /* 0x09 */ u8 ucJTimer;
    /* 0x0A */ u8 ucTimer;
    /* 0x0B */ u8 uc_unused;
    /* 0x0C */ u16 usTimer;
    /* 0x0E */ u16 usMukiX;
    /* 0x10 */ u16 usMukiY;
    /* 0x12 */ u16 usPosX;
    /* 0x14 */ u16 usPosY;
    /* 0x16 */ s16 sMvSpeedX;
    /* 0x18 */ s16 sMvSpeedY;
    /* 0x1A */ u8 ucPlace;
    /* 0x1B */ u8 uc_unused2;
    /* 0x1C */ s16 usSlope;
    /* 0x1E */ u8 ucAnmTimer;
    /* 0x1F */ u8 ucAnmPat;
    /* 0x20 */ void* pObjData;
    /* 0x24 */ void* pChrAddr1;
    /* 0x28 */ u16 usChrByte1;
    /* 0x2A */ u8 uc_unused3[2];
    /* 0x2C */ void* pChrAddr2;
    /* 0x30 */ u16 usChrByte2;
    /* 0x32 */ s16 sHitDX1;
    /* 0x34 */ s16 sHitDY1;
    /* 0x36 */ s16 sHitDX2;
    /* 0x38 */ s16 sHitDY2;
    /* 0x3A */ u8 ucAwaCount;
    /* 0x3B */ u8 ucAwaKabe;
} Wario_t; /* size: 0x3C */

extern Wario_t Wario;
// extern Wario_t WarBk;

typedef void (*WarioChngFunc)(u8);
extern WarioChngFunc WarioChng_React[2];

extern u16 usWarStopFlg;
extern u8 ucWarOffFlg;

LONGCALL void WarioCoinSet(s32);
LONGCALL void WarioVoiceSet(s32);
LONGCALL void GmWarioCreate(void);
