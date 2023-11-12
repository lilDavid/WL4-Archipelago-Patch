#pragma once

#include <gba.h>

#include "vanilla/types.h"

typedef struct {
    const u8* receiver;
    const u8* item_name;
} ExtData;

// Maps locations to the 8-bit IDs of the items they contain.
// After Archipelago patches the ROM, the value 0xFE should be in every
// location that doesn't exist and only those locations.

extern const u8 Jewel1LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const u8 Jewel2LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const u8 Jewel3LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const u8 Jewel4LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const u8 CDLocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const u8 HealthLocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const u8 Health2LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL];

// Maps locations to pointers toward the item's multiworld data.

extern const ExtData* Jewel1ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const ExtData* Jewel2ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const ExtData* Jewel3ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const ExtData* Jewel4ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const ExtData* CDExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const ExtData* HealthExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];
extern const ExtData* Health2ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL];

// Starting inventory.

extern const u8 StartingInventoryItemStatus[PASSAGE_MAX][LEVEL_MAX];
extern const u8 StartingInventoryJunkCounts[4];
extern const u8 StartingInventoryWarioAbilities;
