#pragma once

#include <gba.h>

#include "unsorted/functions.h"
#include "graphics.h"


typedef enum {
    PSPRITE_BOX_GEM1 = 0x00,
    PSPRITE_BOX_GEM2 = 0x01,
    PSPRITE_BOX_GEM3 = 0x02,
    PSPRITE_BOX_GEM4 = 0x03,
    PSPRITE_BOX_CD = 0x04,
    PSPRITE_BOX_HEART = 0x05,
    PSPRITE_DIAMOND = 0x06,
    // ...
    PSPRITE_TREASURE_GEM1 = 0x86,
    PSPRITE_TREASURE_GEM2 = 0x87,
    PSPRITE_TREASURE_GEM3 = 0x88,
    PSPRITE_TREASURE_GEM4 = 0x89,
    PSPRITE_TREASURE_CD = 0x8A,
    PSPRITE_TREASURE_HEART = 0x8B,
    PSPRITE_LIGHT_GEM1 = 0x8C,
    PSPRITE_LIGHT_GEM2 = 0x8D,
    PSPRITE_LIGHT_GEM3 = 0x8E,
    PSPRITE_LIGHT_GEM4 = 0x8F,
    PSPRITE_LIGHT_CD = 0x90,
    PSPRITE_LIGHT_HEART = 0x91,
    // ...
} PrimarySpriteId;

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
    /* 0x20 */ u8 DrawDistanceBottom;
    /* 0x21 */ u8 DrawDistanceTop;
    /* 0x22 */ u8 DrawDistanceHorizontal;
    /* 0x23 */ u8 HitboxTop;  // Up from sprite position
    /* 0x24 */ u8 HitboxBottom;  // Down from sprite position
    /* 0x25 */ u8 HitboxLeft;  // Left of sprite position
    /* 0x26 */ u8 HitboxRight;  // Right of sprite position
    /* 0x27 */ u8 WorkVariable0;
    /* 0x28 */ u8 WorkVariable1;
    /* 0x29 */ u8 WorkVariable2;
    /* 0x2A */ u8 WorkVariable3;
    /* 0x2B */ u8 padding;
} SpriteData; /* size = 0x2C */

typedef enum {
    SSPRITE_SCORE_1000 = 4,
    SSPRITE_TREASURE_ICONS = 0x41,
    SSPRITE_CD_ICON = 0x42,
    SSPRITE_WARIO_DEATH = 0x4D,
} SecondarySpriteId;

typedef struct {
    /* 0x00 */ u8 ucStatus;
    /* 0x01 */ u8 ucAnmTimer;
    /* 0x02 */ u8 GlobalId;
    /* 0x03 */ u8 ucSeq;
    /* 0x04 */ u8 WorkVariable0;
    /* 0x05 */ u8 WorkVariable1;
    /* 0x06 */ u16 usAnmPat;
    /* 0x07 */ u16 usPosY;
    /* 0x08 */ u16 usPosX;
} SecondarySpriteData; /* size = 0xA */


extern SpriteData gCurrentSprite;
extern SpriteData gSpriteData[24];
extern u8 SpriteLeftoverStateList[16][64];

extern SecondarySpriteData gCurrentSecondarySprite;

LONGCALL void Sprite_SpawnAsChild(u8 global_id, u8 parent_slot, u8 chr_ofs, s16 y_pos, s16 x_pos);
LONGCALL void Sprite_Draw(void);

LONGCALL void Sprite_SpawnSecondary(u16 y_pos, u16 x_pos, u8 global_id);
