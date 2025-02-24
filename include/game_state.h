#pragma once

#include <gba.h>


typedef struct {
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 queuedFill;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 timer;
} WarioLifeData;

extern s16 GlobalGameMode;
extern s16 sGameSeq;

extern u8 PassageID;
extern u8 InPassageLevelID;
extern u8 CurrentRoomId;

extern u8 GlobalTimer;
extern u8 ucTimeUp;
extern u8 gPauseFlag[];


#define MAX_WARIO_HEARTS 8

extern WarioLifeData WarioHeart;
