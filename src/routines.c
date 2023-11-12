#include <gba.h>

#include "vanilla/functions.h"
#include "vanilla/types.h"
#include "vanilla/variables.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"

#define AutoSave_ExRead_Work THUMB_FUNCTION(0x8073A18, void, void)

void InitializeVariables() {
    DeathLinkEnabled = DeathLinkFlag;
    MultiworldState = 0;
    TextTimer = 0;

    // From vanilla
    KeyPressContinuous = 0;
    KeyPressPrevious = 0;
    usTrg_KeyPress1Frame = 0;
    sGameSeq = 0;
}

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
