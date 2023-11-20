#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
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

u8* boxPosessionVariables[BOX_MAX] = {
    &Has1stGemPiece, &Has2ndGemPiece, &Has3rdGemPiece, &Has4thGemPiece,
    &HasCD, &HasFullHealthItem, &HasFullHealthItem2
};


// The boxes you've checked are in the second byte of the item status word.
// This was originally done with a patch, but now these functions are rewritten
// so this data is handled all in the same place.

void CheckLocations() {
    for (int i = 0; i <= BOX_CD; i++) {
        if (HAS_BOX(i) != 1)
            continue;
        W4ItemStatus[PassageID][InPassageLevelID] |= 1 << (8 + i);
        int item_id = ItemInCurrentLevel(i);
        const ExtData* multiworld_data = ExtDataInCurrentLevel(i);
        if (!(item_id & ITEMBIT_JUNK && multiworld_data == NULL))
            GiveItem(item_id, multiworld_data);
    }

    if (HasKeyzer) {
        W4ItemStatus[PassageID][InPassageLevelID] |= ISB_KEYZER;
    }

    for (int i = BOX_HEART; i < BOX_MAX; i++) {
        if (HAS_BOX(i) != 1)
            continue;
        W4ItemStatus[PassageID][InPassageLevelID] |= 1 << (8 + i + 1);
        int item_id = ItemInCurrentLevel(i);
        const ExtData* multiworld_data = ExtDataInCurrentLevel(i);
        if (!(item_id & ITEMBIT_JUNK && multiworld_data == NULL))
            GiveItem(item_id, multiworld_data);
    }
}

void SetItemCollection() {
    int item_status = W4ItemStatus[PassageID][InPassageLevelID];

    for (int i = 0; i <= BOX_CD; i++) {
        int has_item = item_status & (1 << (8 + i));
        if (has_item)
            has_item = 3;
        HAS_BOX(i) = has_item;
    }
    if (item_status & ISB_KEYZER) {
        HasKeyzer = 2;
    } else {
        HasKeyzer = 0;
    }
    for (int i = BOX_HEART; i < BOX_MAX; i++) {
        int has_item = item_status & (1 << (8 + i + 1));
        if (has_item)
            has_item = 3;
        HAS_BOX(i) = has_item;
    }

    AbilitiesInThisLevel = 0;
}
