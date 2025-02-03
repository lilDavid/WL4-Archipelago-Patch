#pragma once

#include <gba.h>

#include "graphics.h"


typedef enum {
    ENTITY_BOX_GEM1 = 0x00,
    ENTITY_BOX_GEM2 = 0x01,
    ENTITY_BOX_GEM3 = 0x02,
    ENTITY_BOX_GEM4 = 0x03,
    ENTITY_BOX_CD = 0x04,
    ENTITY_BOX_HEART = 0x05,
    ENTITY_DIAMOND = 0x06,
    // ...
    ENTITY_TREASURE_GEM1 = 0x86,
    ENTITY_TREASURE_GEM2 = 0x87,
    ENTITY_TREASURE_GEM3 = 0x88,
    ENTITY_TREASURE_GEM4 = 0x89,
    ENTITY_TREASURE_CD = 0x8A,
    ENTITY_TREASURE_HEART = 0x8B,
    ENTITY_LIGHT_GEM1 = 0x8C,
    ENTITY_LIGHT_GEM2 = 0x8D,
    ENTITY_LIGHT_GEM3 = 0x8E,
    ENTITY_LIGHT_GEM4 = 0x8F,
    ENTITY_LIGHT_CD = 0x90,
    ENTITY_LIGHT_HEART = 0x91,
    // ...
} EntityId;


typedef struct {
    /* 0x00 */ u16 usStatus;
    /* 0x02 */ u8 field2;
    /* 0x03 */ u8 field3;
    /* 0x04 */ const TAnmDef* OAMDataPackPointerForCurrentAnimation;
    /* 0x08 */ u16 YPos;
    /* 0x0A */ u16 XPos;
    /* 0x0C */ u16 AffinePA;
    /* 0x0E */ u16 AffinePB;
    /* 0x10 */ u16 AffinePC;
    /* 0x12 */ u16 AffinePD;
    /* 0x14 */ u16 CurrentAnimationFrameId;
    /* 0x16 */ u8 RealFrameCountForCurrentAnimationFrame;
    /* 0x17 */ u8 GlobalId;
    /* 0x18 */ u8 RoomEntitySlotId;
    /* 0x19 */ u8 ucChrOfs;
    /* 0x1A */ u8 Prio;
    /* 0x1B */ u8 paletteId;
    /* 0x1C */ u8 CurrentAnimationId;
    /* 0x1D */ u8 Life;
    /* 0x1E */ u8 GuardAndDamageParam;
    /* 0x1F */ u8 DisableInteractionWithWarioCountdown;
    /* 0x20 */ u8 SizeY0_Bottom;
    /* 0x21 */ u8 SizeY1_Top;
    /* 0x22 */ u8 SizeX_LeftRight;
    /* 0x23 */ u8 HitboxY0;
    /* 0x24 */ u8 HitboxY1;
    /* 0x25 */ u8 HitboxX0;
    /* 0x26 */ u8 HitboxX1;
    /* 0x27 */ u8 TWork0;
    /* 0x28 */ u8 TWork1;
    /* 0x29 */ u8 TWork2;
    /* 0x2A */ u8 TWork3;
    /* 0x2B */ u8 padding;
} EnemyDataStructure; /* size = 0x2C */


extern EnemyDataStructure CurrentEnemyData;
extern u8 EntityLeftOverStateList[16][64];
