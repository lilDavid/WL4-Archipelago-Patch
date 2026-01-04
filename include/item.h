#pragma once

#include <gba.h>

#include "unsorted/types.h"

extern u8 Has1stGemPiece;
extern u8 Has2ndGemPiece;
extern u8 Has3rdGemPiece;
extern u8 Has4thGemPiece;
extern u8 HasCD;
extern u8 HasKeyzer;

typedef enum {
    ISB_GEM1 = 1 << 0,
    ISB_GEM2 = 1 << 1,
    ISB_GEM3 = 1 << 2,
    ISB_GEM4 = 1 << 3,
    ISB_CD = 1 << 4,
    ISB_KEYZER = 1 << 5,
    ISB_HEART = 1 << 6,
    ISB_HEART2 = 1 << 7,
    ISB_DIAMOND1 = 1 << 8,
    ISB_DIAMOND2 = 1 << 9,
    ISB_DIAMOND3 = 1 << 10,
    ISB_DIAMOND4 = 1 << 11,
    ISB_DIAMOND5 = 1 << 12,
    ISB_DIAMOND6 = 1 << 13,
    ISB_DIAMOND7 = 1 << 14,
    ISB_DIAMOND8 = 1 << 15,
    ISB_DIAMOND9 = 1 << 16,
    ISB_DIAMOND10 = 1 << 17,
    ISB_DIAMOND11 = 1 << 18,
} ItemStatusBit;

typedef enum {
    ITEMTYPE_GEM,
    ITEMTYPE_CD,
    ITEMTYPE_KEYZER,
    ITEMTYPE_ABILITY,
    ITEMTYPE_TREASURE,
    ITEMTYPE_JUNK,
    ITEMTYPE_AP = 0xFE,
    ITEMTYPE_NONE = 0xFF
} ItemType;

typedef enum {
    JEWELPIECE_NE,
    JEWELPIECE_SE,
    JEWELPIECE_SW,
    JEWELPIECE_NW,
    JEWELPIECE_MAX
} JewelQuadrant;

typedef enum {
    ABILITY_GROUND_POUND,
    ABILITY_SWIM,
    ABILITY_HEAD_SMASH,
    ABILITY_GRAB,
    ABILITY_DASH_ATTACK,
    ABILITY_STOMP_JUMP,
    ABILITY_SUPER_GROUND_POUND,
    ABILITY_HEAVY_GRAB,
    ABILITY_MAX,
} Ability;

#define _HAS_ABILITY(collection, ability) ((collection) & (1 << ability))
#define HAS_ABILITY_PERMANENT(ability) _HAS_ABILITY(WarioAbilities, ability)
#define HAS_ABILITY_TEMPORARY(ability) _HAS_ABILITY(WarioAbilities | AbilitiesInThisLevel, ability)

typedef enum {
    JUNK_FULL_HEALTH,
    JUNK_WARIO_FORM_TRAP,
    JUNK_HEART,
    JUNK_LIGHTNING_TRAP,
    JUNK_MINIGAME_MEDAL,
    JUNK_DIAMOND,
    JUNK_MAX,
} JunkItem;

typedef enum {
    AP_IC_FILLER,
    AP_IC_PROGRESSION,
    AP_IC_USEFUL,
    AP_IC_TRAP,
} APItemClassification;

#define ITEM_JEWEL(passage, quadrant) ((passage << 2) | quadrant)
#define ITEM_CD(passage, level) ((1 << 5) | (passage << 2) | level)
#define ITEM_KEYZER(passage, level) ((3 << 6) | (passage << 2) | level)
#define ITEM_ABILITY(ability) ((1 << 6) | ability)
#define ITEM_JUNK(junk) ((1 << 7) | junk)
#define ITEM_AP(classification) (0xF0 | classification)

typedef enum {
    ITEM_GROUND_POUND     = ITEM_ABILITY(ABILITY_GROUND_POUND),
    ITEM_SWIM             = ITEM_ABILITY(ABILITY_SWIM),
    ITEM_HEAD_SMASH       = ITEM_ABILITY(ABILITY_HEAD_SMASH),
    ITEM_GRAB             = ITEM_ABILITY(ABILITY_GRAB),
    ITEM_DASH_ATTACK      = ITEM_ABILITY(ABILITY_DASH_ATTACK),
    ITEM_STOMP_JUMP       = ITEM_ABILITY(ABILITY_STOMP_JUMP),

    ITEM_FULL_HEALTH_ITEM = ITEM_JUNK(JUNK_FULL_HEALTH),
    ITEM_WARIO_FORM_TRAP  = ITEM_JUNK(JUNK_WARIO_FORM_TRAP),
    ITEM_HEART            = ITEM_JUNK(JUNK_HEART),
    ITEM_LIGHTNING_TRAP   = ITEM_JUNK(JUNK_LIGHTNING_TRAP),
    ITEM_MINIGAME_MEDAL   = ITEM_JUNK(JUNK_MINIGAME_MEDAL),
    ITEM_DIAMOND          = ITEM_JUNK(JUNK_DIAMOND),

    ITEM_AP_FILLER        = ITEM_AP(AP_IC_FILLER),
    ITEM_AP_PROGRESSION   = ITEM_AP(AP_IC_PROGRESSION),
    ITEM_AP_USEFUL        = ITEM_AP(AP_IC_USEFUL),
    ITEM_AP_TRAP          = ITEM_AP(AP_IC_TRAP),
    ITEM_NONE             = 0xFF
} ItemID;


extern const u16 AbilityPaletteTable[];

extern u32 W4ItemStatus[PASSAGE_MAX][LEVEL_MAX];
extern u8 WarioAbilities;
extern u16 gMedalCount;
extern s32 iGmTotalScore;

extern u8 LastCollectedItemID;
extern u8 LastCollectedItemStatus;

extern u8 QueuedFullHealthItem;
extern u8 QueuedFormTraps;
extern u8 QueuedHearts;
extern u8 QueuedLightningTraps;
extern s8 LightningTrapTimer;
extern u8 AbilitiesInThisLevel;

ItemType Item_GetType(u8 item_id);

void GiveWarioHearts(u32 hearts);
void GiveTransformTrap(void);
void GiveLightningTrap(void);
void ApplyLightningTrap(void);
