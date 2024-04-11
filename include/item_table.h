#pragma once

#include <gba.h>

#include "unsorted/types.h"
#include "item.h"

typedef enum {
    BOX_GEM1,
    BOX_GEM2,
    BOX_GEM3,
    BOX_GEM4,
    BOX_CD,
    BOX_HEART,
    BOX_HEART2,
    BOX_MAX
} BoxType;

typedef struct {
    const u8* receiver;
    const u8* item_name;
} ExtData;

// Maps locations to the 8-bit IDs of the items they contain.
// After Archipelago patches the ROM, the value 0xFE should be in every
// location that doesn't exist and only those locations.
extern const u8 ItemLocationTable[PASSAGE_MAX][LEVEL_BOSS + 1][BOX_MAX];

// Maps locations to pointers toward the item's multiworld data.
extern const ExtData* ItemExtDataTable[PASSAGE_MAX][LEVEL_BOSS + 1][BOX_MAX];

// Starting inventory.
extern const u8 StartingInventoryItemStatus[PASSAGE_MAX][LEVEL_MAX];
extern const u8 StartingInventoryJunkCounts[JUNK_MAX];
extern const u8 StartingInventoryWarioAbilities;


extern u8* boxPosessionVariables[BOX_MAX];
#define HAS_BOX(box) (*boxPosessionVariables[box])
extern u8 BoxContents[BOX_MAX];
extern const ExtData* BoxExtData[BOX_MAX];
extern u8 LastCollectedBox;

u8 ItemInCurrentLevel(u32 boxtype);
const ExtData* ExtDataInCurrentLevel(u32 boxtype);

void SetTreasurePalette(u32 item_palette);
void GiveItem(u8 item_id, const ExtData* ext_data);
void SpawnCollectionIndicator(u32 is_cd, u32 is_permanent);
