#include <gba.h>

#include "vanilla/functions.h"
#include "vanilla/types.h"
#include "item.h"
#include "item_table.h"

#define AutoSave_ExRead_Work THUMB_FUNCTION(0x8073A18, void, void)

void CreateStartingInventory() {
    AutoSave_ExRead_Work();

    for (int i = 0; i < PASSAGE_MAX; i++) {
        for (int j = 0; j < LEVEL_MAX; j++) {
            W4ItemStatus[i][j] |= StartingInventoryItemStatus[i][j];
        }
    }

    QueuedFullHealthItem = StartingInventoryJunkCounts[0];
    QueuedFormTraps = StartingInventoryJunkCounts[1];
    QueuedHearts = StartingInventoryJunkCounts[2];
    QueuedLightningTraps = StartingInventoryJunkCounts[3];

    WarioAbilities = StartingInventoryWarioAbilities;
}
