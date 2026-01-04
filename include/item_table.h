#pragma once

#include <gba.h>

#include "unsorted/types.h"
#include "graphics.h"
#include "item.h"

typedef enum {
    BOX_GEM1,
    BOX_GEM2,
    BOX_GEM3,
    BOX_GEM4,
    BOX_CD,
    KEYZER,
    BOX_HEART,
    BOX_HEART2,
    DIAMOND_1,
    DIAMOND_2,
    DIAMOND_3,
    DIAMOND_4,
    DIAMOND_5,
    DIAMOND_6,
    DIAMOND_7,
    DIAMOND_8,
    DIAMOND_9,
    DIAMOND_10,
    DIAMOND_11,
    LOCATION_MAX
} LocationType;

typedef struct {
    const u8* receiver;
    const u8* item_name;
} MultiworldData;

// Number of levels that have random locations
#define RANDO_LEVEL_COUNT (LEVEL_BOSS + 1)

// Maps locations to the 8-bit IDs of the items they contain.
// After Archipelago patches the ROM, the value 0xFE should be in every
// location that doesn't exist and only those locations.
extern const u8 ItemLocationTable[PASSAGE_MAX][RANDO_LEVEL_COUNT][LOCATION_MAX];

// Maps locations to pointers toward the item's multiworld data.
extern const MultiworldData* MultiworldDataTable[PASSAGE_MAX][RANDO_LEVEL_COUNT][LOCATION_MAX];

// Starting inventory.
extern const u8 StartingInventoryItemStatus[PASSAGE_MAX][LEVEL_MAX];
extern const u8 StartingInventoryJunkCounts[JUNK_MAX];
extern const u8 StartingInventoryWarioAbilities;


extern u8* boxPosessionVariables[BOX_CD + 1];
#define HAS_BOX(box) (*boxPosessionVariables[box])
extern u8 LastCollectedItemIndex;
extern u32 CollectedItems;
extern u8 gStoredMultiworldDiamonds;

u8 ItemInCurrentLevel(u32 boxtype);
const MultiworldData* MultiworldDataInCurrentLevel(u32 boxtype);

void SetTreasurePalette(u32 item_palette);
void GiveItem(u8 item_id, const MultiworldData* ext_data);
void GiveItem_InGame(u8 item_id, const MultiworldData* ext_data);
void GiveItem_LevelEnd(u8 item_id, const MultiworldData* ext_data);
void SpawnCollectionIndicator(u32 is_cd, u32 is_permanent);

u8 HasItemInLevel(u8 index);
u8 ItemChooseFakeSprite(void);
const TAnmDef* ItemLoadInGameGraphics(u8 index);
const TAnmDef* ItemLoadInGameGraphicsForID(u8 item_id);
void CollectItemInLevel(u8 index);
void ItemSetHitboxAndDrawDistance(u8 item_id);
