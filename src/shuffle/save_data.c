#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"

LONGCALL void AutoSave_ExRead_Work(void);

void CreateStartingInventory() {
    AutoSave_ExRead_Work();

    for (int i = 0; i < PASSAGE_MAX; i++) {
        for (int j = 0; j < LEVEL_MAX; j++) {
            W4ItemStatus[i][j] |= StartingInventoryItemStatus[i][j];
        }
    }

    QueuedFullHealthItem = StartingInventoryJunkCounts[JUNK_FULL_HEALTH];
    QueuedFormTraps = StartingInventoryJunkCounts[JUNK_WARIO_FORM_TRAP];
    QueuedHearts = StartingInventoryJunkCounts[JUNK_HEART];
    QueuedLightningTraps = StartingInventoryJunkCounts[JUNK_LIGHTNING_TRAP];
    MiniGameCoinNum = StartingInventoryJunkCounts[JUNK_MINIGAME_COIN];

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
        if (!((Item_GetType(item_id) == ITEMTYPE_JUNK && item_id != ITEM_MINIGAME_COIN) &&
              multiworld_data == NULL))
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
        if (!((Item_GetType(item_id) == ITEMTYPE_JUNK && item_id != ITEM_MINIGAME_COIN) &&
              multiworld_data == NULL))
            GiveItem(item_id, multiworld_data);
    }
}

void CheckBossLocations() {
    unsigned int current_status = W4ItemStatus[PassageID][InPassageLevelID];
    unsigned int new_status = 0;
    if (Has1stGemPiece) {
        new_status |= ISB_GEM1;
    }
    if (Has2ndGemPiece) {
        new_status |= ISB_GEM2;
    }
    if (Has3rdGemPiece) {
        new_status |= ISB_GEM3;
    }
    if (HasKeyzer) {
        W4ItemStatus[PassageID][InPassageLevelID] |= ISB_KEYZER;
    }
    LastCollectedBox = (current_status >> 8) ^ new_status;
    W4ItemStatus[PassageID][InPassageLevelID] |= new_status << 8;

    if (LastCollectedBox) {
        MultiworldState = MW_TEXT_FOUND_BOSS_ITEMS;
        TextTimer = 15;
        VblkStatus = VBLK_DMAP_UPDATE;
    } else {
        return;
    }

    for (int i = 0; i < 3; i++) {
        int flag = (1 << i);
        if (!(LastCollectedBox & flag))
            continue;

        int item = ItemLocationTable[PassageID][InPassageLevelID][i];
        const ExtData* multi = ItemExtDataTable[PassageID][InPassageLevelID][i];
        GiveItem(item, multi);
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
