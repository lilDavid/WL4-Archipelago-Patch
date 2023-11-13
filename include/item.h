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

typedef enum {
    ABILITY_GROUND_POUND,
    ABILITY_SWIM,
    ABILITY_HEAD_SMASH,
    ABILITY_GRAB,
    ABILITY_DASH_ATTACK,
    ABILITY_ENEMY_JUMP,
    ABILITY_SUPER_GROUND_POUND,
    ABILITY_HEAVY_GRAB
} Ability;

typedef enum {
    JUNK_FULL_HEALTH,
    JUNK_WARIO_FORM_TRAP,
    JUNK_HEART,
    JUNK_LIGHTNING_TRAP
} JunkItem;

#define ITEM_JEWEL(passage, quadrant) ((passage << 2) | quadrant)
#define ITEM_CD(passage, level) (ITEMBIT_CD | (passage << 2) | level)
#define ITEM_ABILITY(ability) (ITEMBIT_ABILITY | ability)
#define ITEM_JUNK(junk) (ITEMBIT_JUNK | junk)

typedef enum {
    ITEM_GROUND_POUND = ITEM_ABILITY(ABILITY_GROUND_POUND),
    ITEM_SWIM = ITEM_ABILITY(ABILITY_SWIM),
    ITEM_HEAD_SMASH = ITEM_ABILITY(ABILITY_HEAD_SMASH),
    ITEM_GRAB = ITEM_ABILITY(ABILITY_GRAB),
    ITEM_DASH_ATTACK = ITEM_ABILITY(ABILITY_DASH_ATTACK),
    ITEM_ENEMY_JUMP = ITEM_ABILITY(ABILITY_ENEMY_JUMP),

    ITEM_FULL_HEALTH_ITEM = ITEM_JUNK(JUNK_FULL_HEALTH),
    ITEM_WARIO_FORM_TRAP = ITEM_JUNK(JUNK_WARIO_FORM_TRAP),
    ITEM_HEART = ITEM_JUNK(JUNK_HEART),
    ITEM_LIGHTNING_TRAP = ITEM_JUNK(JUNK_LIGHTNING_TRAP),

    ITEM_ARCHIPELAGO_ITEM = 0xF0,
    ITEM_NONE = 0xFF
} ItemID;


extern u32 W4ItemStatus[PASSAGE_MAX][LEVEL_MAX];
extern u8 WarioAbilities;

extern u8 QueuedFullHealthItem;
extern u8 QueuedFormTraps;
extern u8 QueuedHearts;
extern u8 QueuedLightningTraps;
extern u8 AbilitiesInThisLevel;
