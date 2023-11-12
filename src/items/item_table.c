#include <gba.h>

#include "vanilla/types.h"
#include "item_table.h"

const u8 Jewel1LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};
const u8 Jewel2LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};
const u8 Jewel3LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};
const u8 Jewel4LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};
const u8 CDLocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};
const u8 HealthLocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};
const u8 Health2LocationTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{0}};

const ExtData* Jewel1ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};
const ExtData* Jewel2ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};
const ExtData* Jewel3ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};
const ExtData* Jewel4ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};
const ExtData* CDExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};
const ExtData* HealthExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};
const ExtData* Health2ExtDataTable[PASSAGE_MAX][LEVEL_MAXLEVEL] = {{NULL}};

const u8 StartingInventoryItemStatus[PASSAGE_MAX][LEVEL_MAX] = {{0}};
const u8 StartingInventoryJunkCounts[4] = {0};
const u8 StartingInventoryWarioAbilities = 0;
